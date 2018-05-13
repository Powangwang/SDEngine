#include "RenderDepthToTexure.h"

RenderDepthToTexture::RenderDepthToTexture()
{
	mShaderResourceView = NULL;
	mDepthStencilTexture = NULL;
	mDepthStencilView = NULL;
	
}


RenderDepthToTexture::RenderDepthToTexture(const RenderDepthToTexture&other)
{

}

RenderDepthToTexture::~RenderDepthToTexture()
{
	ShutDown();
}


bool RenderDepthToTexture::Initialize(ID3D11Device* d3dDevice, int TextureSize)
{

	//��һ,��������ͼ��2D�������ݽṹ��,������2D��Ⱦ����
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = TextureSize;
	depthBufferDesc.Height = TextureSize;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; //24λ��Ϊ����Ȼ��棬8λ��Ϊ��ģ�建��
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;  //ע����Ȼ���(����)�İ󶨱�־
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	HR(d3dDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilTexture));
	

	//�ڶ�,�����Ȼ�����ͼ���ݽṹ��,��������Ȼ�����ͼ
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	HR(d3dDevice->CreateDepthStencilView(mDepthStencilTexture, &depthStencilViewDesc, &mDepthStencilView));


	//����,�����ɫ����Դ��ͼ������,�����д�����ɫ����Դ��ͼ,ע����������Ȼ���(����)�������ģ���������ȾĿ�껺��(����)������
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS; //��ʱ��Ϊ�ǽ����������д����������ɫд�����Դ�ʱShader��Դ��ʽ����Ȼ�����һ����
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = depthBufferDesc.MipLevels;
	HR(d3dDevice->CreateShaderResourceView(mDepthStencilTexture, &shaderResourceViewDesc, &mShaderResourceView));
    
	//���ģ������ӿڵ�����
	mViewPort.Width = (float)TextureSize;
	mViewPort.Height = (float)TextureSize;
	mViewPort.MinDepth = 0.0f;
	mViewPort.MaxDepth = 1.0f;
	mViewPort.TopLeftX = 0.0f;
	mViewPort.TopLeftY = 0.0f;


	return true;

}


void RenderDepthToTexture::ShutDown()
{
	ReleaseCOM(mDepthStencilTexture);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mShaderResourceView);
}


//�ô�ʱ����ͼ����Ⱦ�����Ŀǰ��Ⱦ��λ��
void RenderDepthToTexture::SetRenderTarget(ID3D11DeviceContext* deviceContext)
{
	ID3D11RenderTargetView* renderTarget[1] = { 0 };

	//����ȾĿ����ͼ�����ģ����ͼ�������Ⱦ����
	deviceContext->OMSetRenderTargets(1,renderTarget, mDepthStencilView);

	//�����ӿ�
	deviceContext->RSSetViewports(1, &mViewPort);
}


//����������󻺴�,��Ϊ����Ҫ����ɫд(ColorWrite),�����������д
void RenderDepthToTexture::ClearDepth(ID3D11DeviceContext* deviceContext)
{

	//�����Ȼ����ģ�建��
	deviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

// ��������Ⱦģ�͵���������������ΪShaderResourceView��Դ���أ������Դ�����������ShaderResourceView��Դһ��������Shader�����.
ID3D11ShaderResourceView* RenderDepthToTexture::GetShaderResourceView()
{
	return mShaderResourceView;
}
