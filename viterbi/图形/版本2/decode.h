//deoutcode.h
#include <math.h>
#include <iostream.h>
#include <stdlib>
extern void initialmatrix(int* generate,int M,int N);
struct fencenode{
	 int currentstate;//���ƼĴ�����ǰ״̬(...000,...001,...010,...)
	 int incode[2];//�������ݺ��Ⱥ󵽴�˵��������ֱ����ɵľ����
	 int outcode[2];//outcode[0]:����0�����ɵľ����;outcode[1]:����1�����ɵľ����
	 fencenode* in[2];//�������ݺ󵽴�˵��������
	 fencenode* out[2];//out[0]:����0��Ӵ˵㵽��ĵ�;out[1]:����1��Ӵ˵㵽��ĵ�
};
struct survive{
	 int hamingdist;//�ۻ���������
	 int* sufferstate;//����״̬
	 int* outcode;//��Ӧ��ԭ�ź�
};
fencenode* initstatediag(int* generate,int M,int N,int statenum);
 int dist( int dist1, int* dist2, int N);
 int distcomp( int dist1, int dist2, int* dist0, int N,int ham1,int ham2);
void copysur(survive* mysurvivenew,survive* survive,int i,int statenum);
void copysur1(survive* surv1,int m,survive* surv2,int n,int i);
 survive* findmin( survive* surv, int i);
void demodulate(int** demodule,double** receive,int length,int N)//���2PSK�ź�
{//Ӳ�о�,�о�����0(С��0-->1,����0-->0)
	for(int i=0;i<length;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(receive[i][j]<0) demodule[i][j]=1;
			else demodule[i][j]=0;
		}
	}
}
survive* viterbidecoder( int** demodule, int* recover, int* generate, int M, int N, int statenum, int length)
{
	 int i,j;
	 survive* mysurvive=new survive[statenum];//�����״̬��(statenum)���Ҵ�·��
	 survive* mysurvivenew=new survive[statenum];//�����״̬��(statenum)���Ҵ�·��
	for(i=0;i<statenum;i++)
	{//��ʼ���Ҵ�·������
		mysurvive[i].sufferstate=new int[length+1];
		mysurvivenew[i].sufferstate=new int[length+1];
		mysurvive[i].outcode=new int[length];
		mysurvivenew[i].outcode=new int[length];
	}
	 fencenode* myfence=initstatediag(generate,M,N,statenum);
	for(i=0;i<statenum;i++)//���Ҵ�·���ĳ�ʼ��
	{
		mysurvive[i].hamingdist=0;//���ú�����ֵ
		mysurvive[i].sufferstate[0]=0;//��ʼ״̬Ϊ0
		for(j=0;j<length;j++)
		{
			mysurvive[i].sufferstate[j+1]=0;
			mysurvive[i].outcode[j]=0;
		}
	}
	for(i=1;i<=length;i++)
	{//���ɵ�i��ʱ����ڵ��Ҵ�·��Ƭ��
		if(pow(2,i)<=statenum)//��Ӧ���ͼ��������
		{
			for(j=0;j<statenum;j++)
			{//���ɵ�j״̬��Ӧ���Ҵ�·��
				int temp1=(j>>(M-i))&3,temp2=(j>>(M-1-i))&1;
				mysurvive[j].sufferstate[i]=myfence[temp1].out[temp2]->currentstate;
				mysurvive[j].hamingdist+=dist(myfence[temp1].outcode[temp2],demodule[i-1],N);
				mysurvive[j].outcode[i-1]=temp2;
			}
		}
		else
		{//�����ͼ������ÿ��״̬����2��·������,ȡ���������С��Ϊ��ʱ����ڴ�״̬���Ҵ�·��
			copysur(mysurvivenew,mysurvive,i,statenum);
			//׼����ǰһ�ν�����������Ҵ����,��mysurvive����mysurvivenew
			for(j=0;j<statenum;j++)
			{
				fencenode* node1=myfence[j].in[0];fencenode* node2=myfence[j].in[1];//�ҵ�ת����״̬j������״̬�ĵ�ַ
				int code1=myfence[j].incode[0],code2=myfence[j].incode[1];
				int k=distcomp(code1,code2,demodule[i-1],mysurvivenew[node1->currentstate].hamingdist,mysurvivenew[node2->currentstate].hamingdist);
				if(0==k)
				{//node1�ľ���С
					copysur1(mysurvive,j,mysurvivenew,node1->currentstate,i);//������·��
					mysurvive[j].sufferstate[i]=j;
					mysurvive[j].hamingdist+=dist(code1,demodule[i-1],N);
					mysurvive[j].outcode[i-1]=j&1;
				}
				else if(1==k) 
				{//node2�ľ���С
					copysur1(mysurvive,j,mysurvivenew,node2->currentstate,i);//������·��
					mysurvive[j].sufferstate[i]=j;
					mysurvive[j].hamingdist+=dist(code2,demodule[i-1],N);
					mysurvive[j].outcode[i-1]=j&1;
				}
				else
				{
					if(rand()<RAND_MAX/2)
					{
						copysur1(mysurvive,j,mysurvivenew,node1->currentstate,i);//������·��
						mysurvive[j].sufferstate[i]=j;
						mysurvive[j].hamingdist+=dist(code1,demodule[i-1],N);
						mysurvive[j].outcode[i-1]=j&1;
					}
					else
					{
						copysur1(mysurvive,j,mysurvivenew,node2->currentstate,i);//������·��
						mysurvive[j].sufferstate[i]=j;
						mysurvive[j].hamingdist+=dist(code2,demodule[i-1],N);
						mysurvive[j].outcode[i-1]=j&1;
					}
				}
			}
		}
	}	
	 survive* thebest=findmin(mysurvive,statenum);//�ҳ�statenum��·���к���������С��
	for(i=0;i<statenum;i++)
	{
		for(j=0;j<length;j++) recover[j]=thebest->outcode[j];
	}
	for(i=0;i<statenum;i++)
	{//��ʼ���Ҵ�·������
		delete []mysurvive[i].sufferstate;
		delete []mysurvivenew[i].sufferstate;
		delete []mysurvive[i].outcode;
		delete []mysurvivenew[i].outcode;
	}
	delete []mysurvive;
	delete []mysurvivenew;
	delete []myfence;
	return mysurvive;
}
fencenode* initstatediag(int* generate,int M,int N,int statenum)
{
	int i,j,k;//����״̬��
	fencenode* trellisdiagram=new fencenode[statenum];//����ͼ����
	initialmatrix(generate,M,N);
	for(i=0;i<statenum;i++)
	{
		trellisdiagram[i].outcode[0]=0;
		trellisdiagram[i].outcode[1]=0;
		trellisdiagram[i].currentstate=i;
		trellisdiagram[i].in[0]=&trellisdiagram[(i>>1)&(statenum-1)];//��1������״̬i��״̬
		trellisdiagram[i].in[1]=&trellisdiagram[((i>>1)+statenum/2)&(statenum-1)];//��2������״̬i��״̬
		//(i>>1)+1<<(log(2,statenum)-1)=(i>>1)+statenum/2  ����������λ��ʧ��1
		trellisdiagram[i].out[0]=&trellisdiagram[(i<<1)&(statenum-1)];//����0����״̬
		trellisdiagram[i].out[1]=&trellisdiagram[((i<<1)+1)&(statenum-1)];//����1����״̬
		for(j=0;j<N;j++)
		{//���ɴ˾������Ԫ��1bit
			int sum1=0,sum2=0;
			int temp1=((i<<1)&(2*statenum-1))&generate[j];
			int temp2=(((i<<1)+1)&(2*statenum-1))&generate[j];
			for(k=0;k<M;k++) {sum1^=(temp1>>k)&1;sum2^=(temp2>>k)&1;}
			//ʵ�־��������·���������ϵ,�˾�����к���ģ2��
			trellisdiagram[i].outcode[0]=(trellisdiagram[i].outcode[0]<<1)^sum1;//��ϳɾ������Ԫ
			trellisdiagram[i].outcode[1]=(trellisdiagram[i].outcode[1]<<1)^sum2;//��ϳɾ������Ԫ
		}
	}
	for(i=0;i<statenum;i++)
	{
		trellisdiagram[i].incode[0]=trellisdiagram[i].in[0]->outcode[i&1];
		trellisdiagram[i].incode[1]=trellisdiagram[i].in[1]->outcode[i&1];
	}
	return trellisdiagram;
}
 int dist( int dist1, int* dist2, int N)//dist1:��������,dist2:ʮ��������,���ΪN
{
	 int temp,sum=0,i;//��λ���
	for(i=0;i<N;i++)
	{
		temp=((dist1>>i)&1)^dist2[N-1-i];
		sum=sum+temp;
	}//��������
	return sum;
}
 int distcomp( int dist1, int dist2, int* dist0, int N,int ham1,int ham2)
{
	if((dist(dist1,dist0)+ham1)<(dist(dist2,dist0)+ham2)) return 0;
	else if((dist(dist1,dist0)+ham1)>(dist(dist2,dist0)+ham2)) return 1;
	else return 2;
}
void copysur(survive* mysurvivenew,survive* mysurvive,int i,int statenum)
{//��·������mysurvive��ÿ��·����ǰi�����ݸ���mysurvivene�е�ÿ��·��
	for( int j=0;j<statenum;j++)
	{
		mysurvivenew[j].hamingdist=mysurvive[j].hamingdist;
		mysurvivenew[j].sufferstate[0]=mysurvive[j].sufferstate[0];
		for( int k=0;k<i;k++)
		{
			mysurvivenew[j].sufferstate[k]=mysurvive[j].sufferstate[k];
			mysurvivenew[j].outcode[k-1]=mysurvive[j].outcode[k-1];
		}
	}
}
void copysur1( survive* surv1, int m, survive* surv2, int n, int i)
{//��·��surv2��ǰi�����ݸ���surv1,����µ�surv1
		surv1[m].hamingdist=surv2[n].hamingdist;
		surv1[m].sufferstate[0]=surv2[n].sufferstate[0];
		for( int k=1;k<i;k++)
		{
			surv1[m].sufferstate[k]=surv2[n].sufferstate[k];
			surv1[m].outcode[k-1]=surv2[n].outcode[k-1];
		}
}
 survive* findmin( survive* surv, int i)//��i��·�����Һ���������С��
{
	 int j,best=0,smaller=surv[0].hamingdist;
	for(j=0;j<i;j++)
	{
		if(surv[j].hamingdist<smaller) {smaller=surv[j].hamingdist;best=j;}
	}
	return &surv[best];
}
double pe(int* origin,int* recover,int length)
{
	int i,sum=0;
	for(i=0;i<length;i++)
		if(origin[i]!=recover[i]) sum++;
	return (double)sum/length;
}