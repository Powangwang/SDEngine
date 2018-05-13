#include"MathTool.h"

//����һ�������������ת��
XMMATRIX GetInvenseTranspose(CXMMATRIX ma)
{
	XMMATRIX A = ma;

	//������A�ĵ���������,��Ϊ�������޷�����ƽ�Ƶ�
	A.r[3] = { 0.0f, 0.0f, 0.0f, 1.0f };

	XMVECTOR det = XMMatrixDeterminant(A);
	XMMATRIX MaInvense = XMMatrixInverse(&det, A);
	return XMMatrixTranspose(MaInvense);
}