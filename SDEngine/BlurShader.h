#ifndef _BLUR_SHADER_H
#define _BLUR_SHADER_H

#include<Windows.h>
#include"Macro.h"
#include<d3d11_1.h>
#include<d3dcompiler.h>
#include<DirectXMath.h>
#include<fstream>
#include"MathTool.h"
#include"D3DClass.h"
#include"Shader.h"
#include"Camera.h"
#include"Light.h"
#include "CommonConstantBufferStruct.h"
using namespace std;
using namespace DirectX;


class BlurShader
{
private:

	ID3D11VertexShader* md3dVertexShader;
	ID3D11PixelShader* md3dPixelShader;
	ID3D11InputLayout* md3dInputLayout; 
	ID3D11SamplerState *mSamplerLinearWrap;
	ID3D11SamplerState *mSamplerLinearClamp;

public:
	bool virtual Initialize(WCHAR* vsFilenPath, WCHAR* psFilenPath);

	//��ʼ��Shader,���ڴ���InputLayout,VertexShader,PixelShader,��������
	bool virtual InitializeShader(WCHAR*, WCHAR*);

	//�ͷ�Shader
	void virtual ShutDown();

	//���Shader�����ļ��Ĵ�����Ϣ
	void virtual OutputShaderErrorMessage(ID3D10Blob*, WCHAR*);

public:
	BlurShader(WCHAR* vsFilenPath, WCHAR* psFilenPath);
	BlurShader(const BlurShader& other);
	virtual ~BlurShader();

public:
	bool SetShaderCB(ID3D11ShaderResourceView* screenRT);
	bool SetShaderState();
	bool SetShaderParams(ID3D11ShaderResourceView* screenRT);
};
#endif 