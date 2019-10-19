#pragma once
#include<iostream>
#include<Windows.h>
#include<bitset>
#include<ctime>
#include<cstdlib>

#define SCHED_NORMAL 0		//���ȷ�ʽ->��ͨ����
#define SCHED_RR 1			//���ȷ�ʽ->ʵʱ����
#define SCHED_FIFO 2		//���ȷ�ʽ->ʵʱ����
#define ARRAY_SIZE 140		//���ȼ�����
#define MIN_TIMESLICE 2		//��Сʱ��Ƭ
#define MAX_RT_PRIO 100		//ʵʱ����������ȼ�
#define MAX_SLEEPTIME 50	//�˯��ʱ��


extern int TotalResource[3];	//ȫ�ֱ���--��Դ�ܺ�
typedef struct PCB {
	unsigned int name;
	int NeedTime;
	int RunTime;
	char status;
	struct PCB *prior;
	struct PCB *next;
	unsigned int prio;				//100~139 -- ���ȼ�����Խ�����ȼ�Խ�ͣ���������
	unsigned int Time_slice;
	unsigned int static_prio;		//��̬���ȼ�100~139 -- ����ʱ��Ƭ��С
	unsigned int rt_priority;		//ʵʱ���̵����ȼ�
	unsigned int sleep_avg;			//�ȴ�ʱ�䣬Խ�ñ�ʾԽ������Ҫ����
	int policy;						//���ȷ�ʽ
	unsigned int NeedResource[3];	//��Ҫ�������Դ
}PCB;

typedef struct prio_array_t {
	std::bitset<ARRAY_SIZE> bitmap;
	struct PCB *queue[ARRAY_SIZE];
}prio_array_t;

typedef struct Runqueue {
	struct prio_array_t *Active;
	struct prio_array_t *Expired;
	struct prio_array_t array[2];
}Runqueue;

using namespace std;

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
	PCB *pcb;
public:
	Processing() {}
	~Processing() {}
	int SetTimeSlice(PCB pcb);
	int GetBonus(PCB pcb);
	
	void Inital();

	//void Execute();
	void Insert_To_Array(PCB pcb);
	void print();
};


/*
���м��㷨��
	1�����̽���󱸶��У��ж��Ƿ���ڰ�ȫ���У�
	2�������������̣���������ȴ���ÿһ��ʱ����ٲ��ԡ�
*/
class BankerCal
{
private:

public:
	BankerCal() {}
	~BankerCal() {}
	bool IsSave(PCB pcb);
};