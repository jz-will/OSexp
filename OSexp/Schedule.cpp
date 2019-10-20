#include"Myhead.h"

int Processing::SetTimeSlice(PCB pcb)
{
	if (pcb.static_prio >= 120)
		pcb.Time_slice = (140 - pcb.static_prio) * 5;
	else
		pcb.Time_slice = (140 - pcb.static_prio) * 20;
	return pcb.Time_slice;
}

int Processing::GetBonus(PCB pcb)
{
	int bonus;
	if (pcb.sleep_avg >= 0 && pcb.sleep_avg < 10)
	{
		bonus = 0;
	}
	else if (pcb.sleep_avg >= 10 && pcb.sleep_avg < 20)
	{
		bonus = 1;
	}
	else if (pcb.sleep_avg >= 20 && pcb.sleep_avg < 30)
	{
		bonus = 2;
	}
	else if (pcb.sleep_avg >= 30 && pcb.sleep_avg < 40)
	{
		bonus = 3;
	}
	else if (pcb.sleep_avg >= 40 && pcb.sleep_avg < 50)
	{
		bonus = 4;
	}
	else if (pcb.sleep_avg >= 50 && pcb.sleep_avg < 60)
	{
		bonus = 5;
	}
	else if (pcb.sleep_avg >= 60 && pcb.sleep_avg < 70)
	{
		bonus = 6;
	}
	else if (pcb.sleep_avg >= 70 && pcb.sleep_avg < 80)
	{
		bonus = 7;
	}
	else if (pcb.sleep_avg >= 80 && pcb.sleep_avg < 90)
	{
		bonus = 8;
	}
	else if (pcb.sleep_avg >= 90 && pcb.sleep_avg < 100)
	{
		bonus = 9;
	}
	else if (pcb.sleep_avg == 100)
	{
		bonus = 10;
	}
	return bonus;
}

//初始化：PCB，数组，位图
void  Processing::Inital()
{
	Runq.array[0].bitmap.reset();
	int NeedTime[8] = { 2,6,5,8,7,4,9,3 };
	int rt_priority[8] = { 2,15,65,99,-1,-1,-1,-1 };
	int static_prio[8] = { 100,123,130,132,130,122,134,125 };
	int policy[8] = { SCHED_RR,SCHED_FIFO,SCHED_RR,SCHED_FIFO,SCHED_NORMAL ,SCHED_NORMAL, SCHED_NORMAL, SCHED_NORMAL };
	int NeedResource[8][3] = { {2,1,5},{1,3,2},{3,5,4},{6,4,1},{5,3,4},{2,5,6},{8,5,3},{5,6,7} };	//A:32, B:32, C:32 -->(8, 8, 8)
	for (int i = 0; i < 8; i++)
	{
		pcb[i].next = NULL;
		pcb[i].prior = NULL;
		pcb[i].name = i + 1;
		pcb[i].RunTime = 0;
		pcb[i].sleep_avg = 0;
		pcb[i].status = 'R';
		pcb[i].NeedTime = NeedTime[i];
		pcb[i].static_prio = static_prio[i];
		Runq.array[0].bitmap[static_prio[i]] = 1;
		pcb[i].rt_priority = rt_priority[i];
		if (pcb[i].policy == SCHED_NORMAL)
		{
			//普通进程
			pcb[i].Time_slice = SetTimeSlice(pcb[i]);
			pcb[i].prio = max(100, min(pcb[i].static_prio - GetBonus(pcb[i]) + 5, 139));
		}
		else
		{
			//实时进程
			pcb[i].prio = MAX_RT_PRIO - 1 - pcb[i].rt_priority;
		}
		pcb[i].NeedResource[0] = NeedResource[i][0];
		pcb[i].NeedResource[1] = NeedResource[i][1];
		pcb[i].NeedResource[2] = NeedResource[i][2];
	}
}

void Processing::Execute()
{

}

void Processing::Insert_To_Array(PCB pcb)
{
}

void Processing::print()
{
	for (int i = 0; i < 8; i++)
	{
		cout << pcb[i].Time_slice<<endl;
	}
}

