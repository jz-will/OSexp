#pragma once
#include<iostream>
#include<Windows.h>
#include<bitset>
#include<ctime>
#include<cstdlib>

#define SCHED_NORMAL 0		//调度方式->普通进程
#define SCHED_RR 1			//调度方式->实时进程
#define SCHED_FIFO 2		//调度方式->实时进程
#define ARRAY_SIZE 140		//优先级个数
#define MIN_TIMESLICE 2		//最小时间片
#define MAX_RT_PRIO 100		//实时进程最大优先级
#define MAX_SLEEPTIME 50	//最长睡眠时间


extern int TotalResource[3];	//全局变量--资源总和
typedef struct PCB {
	unsigned int name;
	int NeedTime;
	int RunTime;
	char status;
	struct PCB *prior;
	struct PCB *next;
	unsigned int prio;				//100~139 -- 优先级数字越大，优先级越低，决定调度
	unsigned int Time_slice;
	unsigned int static_prio;		//静态优先级100~139 -- 决定时间片大小
	unsigned int rt_priority;		//实时进程的优先级
	unsigned int sleep_avg;			//等待时间，越久表示越急迫需要运行
	int policy;						//调度方式
	unsigned int NeedResource[3];	//需要分配的资源
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
O(1)调度算法：
	1、初始化队列与双向链表；
	2、运行前计算时间片；
	3、先由位图判断是否有进程，有则运行Active，用完时间片后放到Expired；(两个指针分别指向不同的prios_array_t)
	4、交换Active和Expired，重复3，到进程全部运行完毕;
	5、有新进程进入就绪队列时插入相应位置，与正在运行进程比较优先级，在前则运行，在后则等待运行。

 bitset说明：
	bitset::any()->是否存在1；
	bitset::set(pos)->设置某个位置为1；
	bitset::reset(pos)->设置某个位置为0；
	bitset可直接用数组形式访问某个位置。

优先级计算（2.4nice = 2.6static_prio）：
	实时进程prio =  MAX_RT_PRIO-1 - p->rt_priority;   // 仅做归一化
	普通进程prio = max(100 , min(静态优先级 – bonus + 5) , 139))
时间片分配算法
	静态优先级<120，time_slice = max((140-静态优先级)*20, MIN_TIMESLICE)  --(800,420)
	静态优先级>=120，time_slice = max((140-静态优先级)*5, MIN_TIMESLICE)  --(20,5)
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
银行家算法：
	1、进程进入后备队列，判断是否存在安全序列；
	2、存在则插入进程，不存在则等待，每一个时间后再测试。
*/
class BankerCal
{
private:

public:
	BankerCal() {}
	~BankerCal() {}
	bool IsSave(PCB pcb);
};