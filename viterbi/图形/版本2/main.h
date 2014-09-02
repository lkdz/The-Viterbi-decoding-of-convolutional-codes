//main.cpp
//�����(N,1,M)��viterbi����
#include <iostream.h>
#include "encode.h"
#include "decode.h"
#include <stdlib.h>
#include <math.h>
extern int result(int x);
double viterbi(double DX,int mode)
{
	int length=1000,M=3,N=3;//double DX;
	int* generate=new int[N];generate[0]=001;generate[1]=101;generate[2]=111;
	//generate[N]={001,101,111}��·���������ؾ���M��N��,��ʱN=3
/*-----------------------��ʼ��������̬����-----------------------------*/
	int* origin=new int[length];//origin[length]��ʼ��Ϣλ
	//encodeori[length][N]����Ϣλ���ɾ����,����Ϊ1/N,һ����Ԫ��N����
	int** encodeori=new int*[length];
	for(int temp=0;temp<length;temp++) encodeori[temp]=new int[N];
	//PSKmaker[length][N]���������Ƴ�PSK�ź�
	int** PSKmaker=new int*[length];
	for(temp=0;temp<length;temp++) PSKmaker[temp]=new int[N];
	//send[length][N]�����˹����������ź�
	double** send=new double*[length];
	for(temp=0;temp<length;temp++) send[temp]=new double[N];
	//���ŵ�����յ��ź�
/*
	double** receive=new double*[length];
	for(temp=0;temp<length;temp++) receive[temp]=new double[N];*/
	//��2PSK�������ź�
	int** demodule=new int*[length];
	for(temp=0;temp<length;temp++) demodule[temp]=new int[N];
	//�������ź�;��statenum���Ҵ�·��ɸѡ�ó�
	int* recover=new int[length];
/*-----------------------��̬�����ʼ������-----------------------------*/
	int i,statenum=(int)pow(2,M-1);
	initialmatrix(generate,M,N);//��ʼ����ؾ���
	gensignal(origin,length,mode);
	encode(encodeori,origin,generate,length,M,N);//�����ź�
	PSK(encodeori,PSKmaker,length,N);//PSK����
	addgaosi(send,PSKmaker,length,N,DX);//�Ӹ�˹������
//	transmit(receive,send,length,N);//�����ŵ�,�������ն�
	demodulate(demodule,send,length,N);//���
	survive* sur=viterbidecoder(demodule,recover,generate,M,N,statenum,length);
	double PE=pe(origin,recover,length);
/*-----------------------�ͷŶ�̬������ռ�ռ�-----------------------------*/
	for(i=0;i<length;i++) delete []encodeori[i];delete []encodeori;
	for(i=0;i<length;i++) delete []PSKmaker[i];delete []PSKmaker;
	for(i=0;i<length;i++) delete []send[i];delete []send;
//	for(i=0;i<length;i++) delete []receive[i];delete []receive;
	for(i=0;i<length;i++) delete []demodule[i];delete []demodule;
	delete []generate;
	delete []origin;
	delete []recover;
/*------------------------------�ͷŽ���----------------------------------*/
	return PE;
}
