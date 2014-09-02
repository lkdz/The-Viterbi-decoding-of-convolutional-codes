//decode.h
#include <math.h>
#include <iostream.h>
#include <stdlib>
#define length 10
#define M 3
#define N 3
#define statenum (int)pow(2,M-1)
struct fencenode{
	int currentstate;//���ƼĴ�����ǰ״̬(...000,...001,...010,...)
	int incode[2];//�������ݺ��Ⱥ󵽴�˵��������ֱ����ɵľ����
	int outcode[2];//outcode[0]:����0�����ɵľ����;outcode[1]:����1�����ɵľ����
	fencenode* in[2];//�������ݺ󵽴�˵��������
	fencenode* out[2];//out[0]:����0��Ӵ˵㵽��ĵ�;out[1]:����1��Ӵ˵㵽��ĵ�
};
struct survive{
	int hamingdist;//�ۻ���������
	int sufferstate[length+1];//����״̬
	int outcode[length];//��Ӧ��ԭ�ź�
};
fencenode* initstatediag(int* generate);
int dist(int dist1,int* dist2);
int distcomp(int dist1,int dist2,int* dist0,int ham1,int ham2);
void copysur(survive* mysurvivenew,survive* survive,int i);
void copysur1(survive* surv1,int m,survive* surv2,int n,int i);
survive* findmin(survive* surv,int i);
void demodulate(int demodule[length][N],double receive[length][N])//���2PSK�ź�
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
survive* viterbidecoder(int demodule[length][N],int* recover,int* generate)
{
	int i,j;
	survive* mysurvive=new survive[statenum];//�����״̬��(statenum)���Ҵ�·��
	survive* mysurvivenew=new survive[statenum];//�����״̬��(statenum)���Ҵ�·��
	fencenode* myfence=initstatediag(generate);
	for(i=0;i<statenum;i++)//���Ҵ�·���ĳ�ʼ��
	{
		mysurvive[i].hamingdist=0;//���ú�����ֵ
		mysurvive[i].sufferstate[0]=0;//��ʼ״̬Ϊ0
		mysurvivenew[i].hamingdist=0;
		mysurvivenew[i].sufferstate[0]=0;
		for(j=0;j<length;j++)
		{
			mysurvive[i].sufferstate[j+1]=0;
			mysurvivenew[i].outcode[j]=0;
			mysurvive[i].sufferstate[j+1]=0;
			mysurvivenew[i].outcode[j]=0;
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
				mysurvive[j].hamingdist+=dist(myfence[temp1].outcode[temp2],demodule[i-1]);
				mysurvive[j].outcode[i-1]=temp2;
			}
		}
		else
		{//�����ͼ������ÿ��״̬����2��·������,ȡ���������С��Ϊ��ʱ����ڴ�״̬���Ҵ�·��
			copysur(mysurvivenew,mysurvive,i);
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
					mysurvive[j].hamingdist+=dist(code1,demodule[i-1]);
					mysurvive[j].outcode[i-1]=j&1;
				}
				else if(1==k)
				{//node2�ľ���С
					copysur1(mysurvive,j,mysurvivenew,node2->currentstate,i);//������·��
					mysurvive[j].sufferstate[i]=j;
					mysurvive[j].hamingdist+=dist(code2,demodule[i-1]);
					mysurvive[j].outcode[i-1]=j&1;
				}
				else
				{
					if(rand()<RAND_MAX/2)
					{
						copysur1(mysurvive,j,mysurvivenew,node1->currentstate,i);//������·��
						mysurvive[j].sufferstate[i]=j;
						mysurvive[j].hamingdist+=dist(code1,demodule[i-1]);
						mysurvive[j].outcode[i-1]=j&1;
					}
					else
					{
						copysur1(mysurvive,j,mysurvivenew,node2->currentstate,i);//������·��
						mysurvive[j].sufferstate[i]=j;
						mysurvive[j].hamingdist+=dist(code2,demodule[i-1]);
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
	delete []mysurvive;
	delete []mysurvivenew;
	delete []myfence;
	return mysurvive;
}
fencenode* initstatediag(int* generate)
{
	fencenode* trellisdiagram=new fencenode[statenum];//����ͼ����
	for(int i=0;i<statenum;i++) trellisdiagram[i].currentstate=i;
	trellisdiagram[0].in[0]=&trellisdiagram[0];trellisdiagram[0].incode[0]=0;
	trellisdiagram[0].in[1]=&trellisdiagram[2];trellisdiagram[0].incode[1]=3;
	trellisdiagram[0].out[0]=&trellisdiagram[0];trellisdiagram[0].outcode[0]=0;
	trellisdiagram[0].out[1]=&trellisdiagram[1];trellisdiagram[0].outcode[1]=7;
	trellisdiagram[1].in[0]=&trellisdiagram[0];trellisdiagram[1].incode[0]=7;
	trellisdiagram[1].in[1]=&trellisdiagram[2];trellisdiagram[1].incode[1]=4;
	trellisdiagram[1].out[0]=&trellisdiagram[2];trellisdiagram[1].outcode[0]=1;
	trellisdiagram[1].out[1]=&trellisdiagram[3];trellisdiagram[1].outcode[1]=6;
	trellisdiagram[2].in[0]=&trellisdiagram[1];trellisdiagram[2].incode[0]=1;
	trellisdiagram[2].in[1]=&trellisdiagram[3];trellisdiagram[2].incode[1]=2;
	trellisdiagram[2].out[0]=&trellisdiagram[0];trellisdiagram[2].outcode[0]=3;
	trellisdiagram[2].out[1]=&trellisdiagram[1];trellisdiagram[2].outcode[1]=4;
	trellisdiagram[3].in[0]=&trellisdiagram[1];trellisdiagram[3].incode[0]=6;
	trellisdiagram[3].in[1]=&trellisdiagram[3];trellisdiagram[3].incode[1]=5;
	trellisdiagram[3].out[0]=&trellisdiagram[2];trellisdiagram[3].outcode[0]=2;
	trellisdiagram[3].out[1]=&trellisdiagram[3];trellisdiagram[3].outcode[1]=5;
	return trellisdiagram;
}
int dist(int dist1,int* dist2)//dist1:��������,dist2:ʮ��������,���ΪN
{
	int sum=0;
	switch(4*dist2[0]+2*dist2[1]+dist2[2])
	{
		case 0:
			switch(dist1)
			{
				case 0:sum=0;break;
				case 1: case 2: case 4:sum=1;break;
				case 3: case 5: case 6:sum=2;break;
				case 7:sum=3;break;
			}
			break;
		case 1:
			switch(dist1)
			{
				case 1:sum=0;break;
				case 0: case 3: case 5:sum=1;break;
				case 2: case 4: case 7:sum=2;break;
				case 6:sum=3;break;
			}
			break;
		case 2:
			switch(dist1)
			{
				case 2:sum=0;break;
				case 0: case 3: case 6:sum=1;break;
				case 1: case 4: case 7:sum=2;break;
				case 5:sum=3;break;
			}
			break;
		case 3:
			switch(dist1)
			{
				case 3:sum=0;break;
				case 1: case 2: case 7:sum=1;break;
				case 0: case 5: case 6:sum=2;break;
				case 4:sum=3;break;
			}
			break;
		case 4:
			switch(dist1)
			{
				case 4:sum=0;break;
				case 0: case 5: case 6:sum=1;break;
				case 1: case 2: case 7:sum=2;break;
				case 3:sum=3;break;
			}
			break;
		case 5:
			switch(dist1)
			{
				case 5:sum=0;break;
				case 1: case 4: case 7:sum=1;break;
				case 0: case 3: case 6:sum=2;break;
				case 2:sum=3;break;
			}
			break;
		case 6:
			switch(dist1)
			{
				case 6:sum=0;break;
				case 2: case 4: case 7:sum=1;break;
				case 0: case 3: case 5:sum=2;break;
				case 1:sum=3;break;
			}
			break;
		case 7:
			switch(dist1)
			{
				case 7:sum=0;break;
				case 3: case 5: case 6:sum=1;break;
				case 1: case 2: case 4:sum=2;break;
				case 0:sum=3;break;
			}
			break;
		default:break;
	}
/*
	int temp,sum=0,i;//��λ���
	for(i=0;i<N;i++)
	{
		temp=((dist1>>i)&1)^dist2[N-1-i];
		sum=sum+temp;
	}//��������*/
	return sum;
}
int distcomp(int dist1,int dist2,int* dist0,int ham1,int ham2)
{
	if((dist(dist1,dist0)+ham1)<(dist(dist2,dist0)+ham2)) return 0;
	else if((dist(dist1,dist0)+ham1)>(dist(dist2,dist0)+ham2)) return 1;
	else return 2;
}
void copysur(survive* mysurvivenew,survive* mysurvive,int i)
{//��·������mysurvive��ÿ��·����ǰi�����ݸ���mysurvivene�е�ÿ��·��
	for(int j=0;j<statenum;j++)
	{
		for(int l=1;l<i;l++)//�ҵ���ͬ·����Դͷ
		{
			if(mysurvivenew[j].sufferstate[l]!=mysurvive[j].sufferstate[l]) break;
		}
		mysurvivenew[j].hamingdist=mysurvive[j].hamingdist;
		for(int k=l;k<i;k++)
		{
			mysurvivenew[j].sufferstate[k]=mysurvive[j].sufferstate[k];
			mysurvivenew[j].outcode[k-1]=mysurvive[j].outcode[k-1];
		}
	}
}
void copysur1(survive* surv1,int m,survive* surv2,int n,int i)
{//��·��surv2��ǰi�����ݸ���surv1,����µ�surv1
	for(int l=1;l<i;l++)//�ҵ���ͬ·����Դͷ
	{
		if(surv1[m].sufferstate[l]!=surv2[n].sufferstate[l]) break;
	}
	surv1[m].hamingdist=surv2[n].hamingdist;
	for(int k=l;k<i;k++)
	{
		surv1[m].sufferstate[k]=surv2[n].sufferstate[k];
		surv1[m].outcode[k-1]=surv2[n].outcode[k-1];
	}
}
survive* findmin(survive* surv,int i)//��i��·�����Һ���������С��
{
	int j,k=0,best=0,smaller=surv[0].hamingdist;
	for(j=0;j<i;j++)
	{
		if(surv[j].hamingdist<smaller) {smaller=surv[j].hamingdist;best=j;}
/*
		else if(surv[j].hamingdist=smaller)
		{
			if(rand()&1) best=j;
		}*/
	}
	return &surv[best];
}
int pe(int* origin,int* recover)
{
	int i,sum=0;
	for(i=0;i<length;i++)
		if(origin[i]!=recover[i]) return 1;
	return 0;
}