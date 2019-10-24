#pragma once
#include<iostream>
#include<Windows.h>
#include<bitset>
#include<ctime>
#include<cstdlib>

#define PCBsize 10		//PCB���ֵ
#define SCHED_NORMAL 0		//���ȷ�ʽ->��ͨ����
#define SCHED_RR 1			//���ȷ�ʽ->ʵʱ����
#define SCHED_FIFO 2		//���ȷ�ʽ->ʵʱ����
#define ARRAY_SIZE 140		//���ȼ�����
#define MIN_TIMESLICE 2		//��Сʱ��Ƭ
#define MAX_RT_PRIO 100		//ʵʱ����������ȼ�
#define MAX_SLEEPTIME 50	//�˯��ʱ��
#define resKind 3


extern int TotalResource[3];	//ȫ�ֱ���--��Դ�ܺ�
extern int TotalTime;
extern int count;
typedef struct PCB {
	int name;
	int NeedTime;
	int RunTime;
	char status;					//״̬��W--wait, E--end,  R--Ready��  N--None
	struct PCB *prior;
	struct PCB *next;
	unsigned int prio;				//0~139 -- ���ȼ�����Խ�����ȼ�Խ�ͣ���������
	int Time_slice;					//ʱ��Ƭ
	unsigned int static_prio;		//��̬���ȼ�100~139 -- ����ʱ��Ƭ��С
	int rt_priority;				//ʵʱ���̵����ȼ�
	unsigned int sleep_avg;			//�ȴ�ʱ�䣬Խ�ñ�ʾԽ������Ҫ����
	int policy;						//���ȷ�ʽ
	int Claim[3];					//��Ҫ�������Դ
	int Allocation[3];		
}PCB;

typedef struct prio_array_t {
	std::bitset<ARRAY_SIZE> bitmap;
	struct PCB *queue[ARRAY_SIZE];		//ָ������
}prio_array_t;

typedef struct Runqueue {
	struct prio_array_t *Active;
	struct prio_array_t *Expired;
	struct prio_array_t array[2];
}Runqueue;

using namespace std;

/*
���м��㷨��
	1�����̽���󱸶��У��ж��Ƿ���ڰ�ȫ���У�
	2�������������̣���������ȴ���ÿһ��ʱ����ٲ��ԡ�

class BankerCal
{
private:

public:
	BankerCal() {}
	~BankerCal() {}
	static bool IsSave(PCB pcb);
};*/
class BankerCal
{
private:
	PCB * PCBTable[PCBsize];
	int Available[resKind];				//������Դ
	int Claim[PCBsize][resKind];		//�����ܹ�������Դ
	int Allocation[PCBsize][resKind];   //�����ѷ�����Դ
	int Need[PCBsize][resKind];			 //������Դ��
	int tempAvail[resKind];				//��ʱ������Դ��, ����isSave�ж��Ƿ���ڰ�ȫ����
	int stack[PCBsize + 1];				//��ȫ�Լ���� ��ȫ����ջ

public:
	BankerCal() {};
	~BankerCal() {};
	bool allotRes(int PCBName, int request[]);	//Ҫ����PCBName �� ��Դ��������
	bool  isSave();								//���뵱ǰ�������н������� 
//	void resetTA();
	void freeRes(int PCBid);
	void __init__(PCB pcb[]);
};

/*
O(1)�����㷨��
	1����ʼ��������˫������
	2������ǰ����ʱ��Ƭ��
	3������λͼ�ж��Ƿ��н��̣���������Active������ʱ��Ƭ��ŵ�Expired��(����ָ��ֱ�ָ��ͬ��prios_array_t)
	4������Active��Expired���ظ�3��������ȫ���������;
	5�����½��̽����������ʱ������Ӧλ�ã����������н��̱Ƚ����ȼ�����ǰ�����У��ں���ȴ����С�

 bitset˵����
	bitset::any()->�Ƿ����1��
	bitset::set(pos)->����ĳ��λ��Ϊ1��
	bitset::reset(pos)->����ĳ��λ��Ϊ0��
	bitset��ֱ����������ʽ����ĳ��λ�á�

���ȼ����㣨2.4nice = 2.6static_prio����
	ʵʱ����prio =  MAX_RT_PRIO-1 - p->rt_priority;   // ������һ��
	��ͨ����prio = max(100 , min(��̬���ȼ� �C bonus + 5) , 139))
ʱ��Ƭ�����㷨
	��̬���ȼ�<120��time_slice = max((140-��̬���ȼ�)*20, MIN_TIMESLICE)  --(800,420)
	��̬���ȼ�>=120��time_slice = max((140-��̬���ȼ�)*5, MIN_TIMESLICE)  --(20,5)
*/
class Processing
{
private:
	Runqueue Runq;
	BankerCal banker;
public:
	PCB pcb[8];
	Processing() { Inital(); }
	~Processing() {}
	int SetTimeSlice(PCB pcb);
	int GetBonus(PCB pcb);
	void Inital();
	void Execute();
	void Insert_PCB(PCB p);
	void print();
	void print(PCB *q);
	bool distriRes(PCB p, int pos);
	PCB operator = (PCB p)
	{
		PCB wpcb;
		wpcb.next = NULL;
		wpcb.prior = NULL;
		//wpcb.name = i + 1;
		wpcb.RunTime = 0;
		wpcb.sleep_avg = 0;
		//wpcb.status = 'R';
		wpcb.NeedTime = p.NeedTime;
		wpcb.static_prio = p.status;
		wpcb.rt_priority = p.rt_priority;
		wpcb.policy = p.policy;
		if (wpcb.policy == SCHED_NORMAL)
		{
			//��ͨ����
			wpcb.prio = max(100, min(wpcb.static_prio - GetBonus(wpcb) + 5, 139));
			wpcb.Time_slice = SetTimeSlice(wpcb);
		}
		else
		{
			//ʵʱ����
			wpcb.Time_slice = SetTimeSlice(wpcb);
			wpcb.prio = MAX_RT_PRIO - 1 - wpcb.rt_priority;		//0-99
		}
		wpcb.Claim[0] = p.Claim[0];
		wpcb.Claim[1] = p.Claim[1];
		wpcb.Claim[2] = p.Claim[2];
		wpcb.Allocation[0] = 0;
		wpcb.Allocation[1] = 0;
		wpcb.Allocation[2] = 0;
		return wpcb;
	}
};


