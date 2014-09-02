//main.cpp
//�����(N,1,M)��viterbi����
#include <iostream.h>
#include "encode.h"
#include "decode.h"
#include <stdlib.h>
#include <math.h>
#include <iomanip.h> 
#define length 10
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
double viterbi()
{
	int mode=3,sum=0,i,j,k;
	double DX=1.0;
	for(i=0;i<100;i++)
	{
		if(viterbikey(DX,mode)==1)
		{
			sum=sum+1;
			cout<<"origin: ";
			for(j=0;j<length;j++) cout<<origin[j];
			cout<<endl<<"encodeori: ";
			for(j=0;j<length;j++)
			{
				for(k=0;k<N;k++) cout<<encodeori[j][k];
				cout<<" ";
			}
			cout<<endl<<"PSKmaker: ";
			for(j=0;j<length;j++)
			{
				for(k=0;k<N;k++) cout<<PSKmaker[j][k];
				cout<<" ";
			}
			cout<<endl<<"send ";
			for(j=0;j<length;j++)
			{
				for(k=0;k<N;k++) cout<<setprecision(2)<<send[j][k]<<" ";
				cout<<"  ";
			}
			cout<<endl<<"demodule: ";
			for(j=0;j<length;j++)
			{
				for(k=0;k<N;k++) cout<<demodule[j][k];
				cout<<" ";
			}
			cout<<endl<<"recover: ";
			for(j=0;j<length;j++) cout<<recover[j];
			cout<<endl<<endl;
		}
	}
	return 1.0*sum/1000;
}
void main()
{
	unsigned long seed;
	time((long*)&seed);
	srand(seed);
	cout<<"������Ϊ�� "<<viterbi()<<endl;
}