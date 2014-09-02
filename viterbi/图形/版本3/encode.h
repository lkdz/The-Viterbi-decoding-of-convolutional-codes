//encode.h
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define PI 3.14
#define M 3
#define N 3
#define length 2000
void gensignal(int* origin,int mode)//�����ź�
{//�����������������Ϊ����ǰ����Ϣ����
	if(mode==0) for(int i=0;i<length;i++) origin[i]=0;//����һλ��Ϣλ
	if(mode==1) for(int i=0;i<length;i++) origin[i]=1;//����һλ��Ϣλ
	if(mode==2) for(int i=0;i<length;i++) origin[i]=i&1;//����һλ��Ϣλ
	if(mode==3)
	{
		for(int i=0;i<length;i++) origin[i]=rand()&1;//����һλ��Ϣλ
	}
}
void encode(int encodeori[length][N],int* origin,int* generate)//������
{//���ɾ����,����Ϊ1/N,һ����Ԫ��N����
	int i,j,k,temp=0;//ģ��������λ�Ĵ�����ǰ״̬[b(M-1),b(M-2),b(M-3),...,b(1)],����ʾ16λ
	for(k=0;k<length;k++)
	{//�ɴ���Ϣλ����N bit��Ԫ
		temp=((temp<<1)^origin[k])&((int)pow(2,M)-1);//ģ����λ�Ĵ�������
		for(i=0;i<N;i++)
		{//���ɴ˾������Ԫ��1bit
			int sum=0,temp1=temp&generate[i];
			encodeori[k][i]=0;
			for(j=0;j<M;j++) sum^=(temp1>>j)&1;
			//ʵ�־��������·���������ϵ,�˾�����к���ģ2��
			encodeori[k][i]=sum;//��ϳɾ������Ԫ
		}
	}
}
void PSK(int encodeori[length][N],int PSKmaker[length][N])//PSK����(1-->-1;0-->1)
{
	for(int i=0;i<length;i++) 
		for(int j=0;j<N;j++)
			{
				if(encodeori[i][j]) PSKmaker[i][j]=-1;
				else PSKmaker[i][j]=1;
			}
}
double gaosimaker(double DX)
{
	double rand1,rand2,result;
	rand1=(double)(rand())/RAND_MAX;//���ȷֲ�
	rand2=(double)(rand())/RAND_MAX;//���ȷֲ�
	result=sqrt((-2)*log(rand1))*sin(2*PI*rand2);
	//	result=sqrt((-2)*log(rand1))*sin(2*PI*rand2);
	//�任����,���ȷֲ���Ϊ��̬�ֲ�
	return DX*result;
}
void addgaosi(double send[length][N],int PSKmaker[length][N],double DX)
{
	for(int i=0;i<length;i++) 
		for(int j=0;j<N;j++) 
			send[i][j]=PSKmaker[i][j]+gaosimaker(DX);
}
int result(int x)
{
	if(x=1) return 1;
	else if(x>1)
	{
		return result(x-1)+(int)pow(10,x-1);
	}
	else return 65535;
}