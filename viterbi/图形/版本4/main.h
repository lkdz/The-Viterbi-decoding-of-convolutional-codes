//main.cpp
//�����(N,1,M)��viterbi����
#include <iostream.h>
#include "encode.h"
#include "decode.h"
#include <stdlib.h>
#include <math.h>
#include <iomanip.h> 
#define length 5000
#define M 3
#define N 3
#define statenum (int)pow(2,M-1)
int generate[N]={1,5,7};//001,101,111
int origin[length];
//encodeori[length][N]����Ϣλ���ɾ����,����Ϊ1/N,һ����Ԫ��N����
int encodeori[length][N];
//PSKmaker[length][N]���������Ƴ�PSK�ź�
int PSKmaker[length][N];
//send[length][N]�����˹����������ź�
double send[length][N];
//���ŵ�����յ��ź�
int demodule[length][N];
//�������ź�;��statenum���Ҵ�·��ɸѡ�ó�
	int recover[length];
int viterbikey(double DX,int mode)
{
	gensignal(origin,mode);
	encode(encodeori,origin,generate);//�����ź�
	PSK(encodeori,PSKmaker);//PSK����
	addgaosi(send,PSKmaker,DX);//�Ӹ�˹������
	demodulate(demodule,send);//���
	survive* sur=viterbidecoder(demodule,recover,generate);
	int PE=pe(origin,recover);
	return PE;
}
double viterbi(double DX,int mode)
{
	int sum=0;
	sum=viterbikey(DX,mode);
	return 1.0*sum/length;
}