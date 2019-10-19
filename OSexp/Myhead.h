#pragma once
#include<iostream>
#include<Windows.h>
#include<bitset>
#include<time.h>
#include<stdlib.h>

const int ARRAY_SIZE = 140;
const int BITMAP_SIZE = 140;

typedef struct PCB {
	unsigned int name;
	int NeedTime;
	int RunTime;
	char status;
	struct PCB *prior;
	struct PCB *next;
	unsigned int priority;	//���ȼ�����Խ�����ȼ�Խ��
	unsigned int NeedResource[3];	//��Ҫ�������Դ
}PCB;

typedef struct prio_array_t {
	unsigned int Time_slice;
	struct PCB *next;
}prio_array_t;

typedef struct Runqueue {
	struct prio_array_t *Active;
	struct prio_array_t *Expired;
}Runqueue;

using namespace std;