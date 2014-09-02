//main.cpp
//�����(N,1,M)��viterbi����
#include <iostream.h>
#include "decode.h"
#include <stdlib.h>
#include <math.h>
#include <iomanip.h> 
#define length 10
#define M 3
#define N 3
#define statenum (int)pow(2,M-1)
#include <time.h>
int generate[N]={1,5,7};//001,101,111
int origin[length]={1,0,1,1,0,0,1,1,1,0};
//encodeori[length][N]����Ϣλ���ɾ����,����Ϊ1/N,һ����Ԫ��N����
//int encodeori[length][N];
//PSKmaker[length][N]���������Ƴ�PSK�ź�
//int PSKmaker[length][N];
//send[length][N]�����˹����������ź�
double send[length][N]={{-1.1,-0.7,-0.8},{1.3,0.8,-0.8},{-1.2,0.4,1.6},{-1.1,-0.5,2.4},{-0.1,-1.7,0.96},{1.8,-1.6,-1.4},{-0.7,-0.7,-1.7},{-2.6,0.2,0.8},{-0.8,0.3,-0.4},{1.2,-0.6,1}};
//���ŵ�����յ��ź�
int demodule[length][N];
//�������ź�;��statenum���Ҵ�·��ɸѡ�ó�
int recover[length];
int viterbikey(double DX,int mode)
{
	demodulate(demodule,send);//���
	survive* sur=viterbidecoder(demodule,recover,generate);
	int PE=pe(origin,recover);
	return PE;
}
double viterbi()
{
	int mode=3,sum=0,i,j,k;
	double DX=1;
	for(i=0;i<10;i++)
	{
		sum=sum+viterbikey(DX,mode);
		cout<<"origin: ";
		for(j=0;j<length;j++) cout<<origin[j];
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
	return sum;
}
void main()
{
	unsigned long seed;
	time((long*)&seed);
	srand(seed);
	cout<<"������Ϊ�� "<<viterbi()<<endl;
	cout<<statenum;
}