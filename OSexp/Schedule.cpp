#include"Myhead.h"

int Processing::SetTimeSlice(PCB pcb)
{
	if (pcb.static_prio >= 120)
		pcb.Time_slice = (140 - pcb.static_prio) * 5;
	else
		pcb.Time_slice = (140 - pcb.static_prio) * 10;
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
	else if (pcb.sleep_avg >= 100)
	{
		bonus = 10;
	}
	return bonus;
}

//初始化：PCB，数组，位图
void  Processing::Inital()
{
	Runq.array[0].bitmap.reset();
	int NeedTime[8] = { 3000,600,500,800,700,400,900,2000 };
	int rt_priority[8] = { 2,15,65,99,-1,-1,-1,-1 };
	int static_prio[8] = { 100,123,130,132,130,122,122,125 };
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
		pcb[i].rt_priority = rt_priority[i];
		pcb[i].policy = policy[i];
		if (pcb[i].policy == SCHED_NORMAL)
		{
			//普通进程
			pcb[i].prio = max(100, min(pcb[i].static_prio - GetBonus(pcb[i]) + 5, 139));
			pcb[i].Time_slice = SetTimeSlice(pcb[i]);
		}
		else
		{
			//实时进程
			pcb[i].Time_slice = SetTimeSlice(pcb[i]);
			pcb[i].prio = MAX_RT_PRIO - 1 - pcb[i].rt_priority;		//0-99
		}
		pcb[i].NeedResource[0] = NeedResource[i][0];
		pcb[i].NeedResource[1] = NeedResource[i][1];
		pcb[i].NeedResource[2] = NeedResource[i][2];
	}
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		Runq.array[0].queue[i] = NULL;
		Runq.array[1].queue[i] = NULL;
	}
	Runq.Active = &Runq.array[0];
	Runq.Expired = &Runq.array[1];
	//根据动态优先级设定数组
	for (int i = 0; i < 8; i++)
	{
		Runq.array[0].bitmap[pcb[i].prio] = 1;
/*		PCB *p = new PCB;
		p->name = pcb[i].name;
		p->NeedResource[0] = pcb[i].NeedResource[0];
		p->NeedResource[1] = pcb[i].NeedResource[1];
		p->NeedResource[2] = pcb[i].NeedResource[2];
		p->NeedTime = pcb[i].NeedTime;
		p->policy = pcb[i].policy;
		p->rt_priority = pcb[i].rt_priority;
		p->RunTime = pcb[i].RunTime;
		p->sleep_avg = pcb[i].sleep_avg;
		p->static_prio = pcb[i].static_prio;
		p->status = pcb[i].status;
		p->Time_slice = pcb[i].Time_slice;*/
		if (Runq.array[0].queue[pcb[i].prio] == NULL)
		{
			Runq.array[0].queue[pcb[i].prio] = &pcb[i];
			pcb[i].prior = Runq.array[0].queue[pcb[i].prio];
		}
		else
		{
			PCB *q;
			q = Runq.array[0].queue[pcb[i].prio];
			while (q != NULL)
			{
				q = q->next;
			}
			q = &pcb[i];
			pcb[i].prior = Runq.array[0].queue[pcb[i].prio];
			pcb[i].prior->next = q;
		}
	}
/*	PCB *q;
	q = Runq.array[0].queue[121];
	if (q->next == NULL)
	{
		cout << "failed";
	}
	cout << "?????";*/
//	print(Runq.array[0].queue[122]);
}

void Processing::Execute()
{
	int pos = 0;
	if (!Runq.Active->bitmap.any())
	{
		cout << "进程已全部运行结束！" << endl;
		return;
	}
	//1.找到为1的位置；
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		if (Runq.Active->bitmap[i] == 1)
		{
			//2.判断类型和运行方式;
			PCB *q;
			q = Runq.Active->queue[i];
			while (q != NULL)
			{
				if (q->policy == SCHED_NORMAL)
				{
					//普通进程
					q->sleep_avg = (TotalTime - q->RunTime) / 100;
					if (q->Time_slice >= (q->NeedTime - q->RunTime))
					{
						TotalTime = TotalTime + q->NeedTime - q->RunTime;
						q->RunTime = q->NeedTime;
						q->status = 'E';
						if (q->next == NULL)
						{
							/*PCB *p;
							p = q;
							q = q->next;
							delete p;*/
							q->prior = NULL;
							q = q->next;
							break;
						}
						PCB *p;
						p = q->prior;
						q->prior = q->next;
						q->next->prior = p;
						p = q;
						q = q->next;
						//delete p;
					}
					else
					{
						TotalTime = TotalTime + q->Time_slice;
						q->RunTime = q->RunTime + q->Time_slice;
						q->prio = max(100, min(q->static_prio - GetBonus(*q) + 5, 139));
						if (Runq.Expired->queue[q->prio] == NULL)
						{
							Runq.Expired->queue[q->prio] = q;
						}
						else
						{
							PCB *p;
							p = Runq.Expired->queue[q->prio];
							while (p != NULL)
							{
								p = q->next;
							}
							PCB *work = p;
							p = q;
							work = p;
						}
						
						Runq.Expired->bitmap.set(q->prio);
						if (q->next == NULL)
						{
							q->prior = Runq.Expired->queue[q->prio];
							q = q->next;
							break;
						}
						PCB *p;
						p = q->prior;
						q->prior = q->next;
						q = q->next;
						q->prior = p;
					}
				}
				else if (q->policy == SCHED_FIFO)
				{
					//实时进程---抢占运行完毕再退出
					TotalTime = TotalTime + q->NeedTime;
					q->RunTime = q->NeedTime;
					q->status = 'E';
					if (q->next == NULL)
					{
						/*PCB *p;
						p = q;
						q = q->next;
						delete p;*/
						q->prior = NULL;
						q = q->next;
						break;
					}
					else
					{
						PCB *p;
						p = q->prior;
						q->prior = q->next;
						q->next->prior = p;
						p = q;
						q = q->next;
						//delete p;
					}
				}
				else if (q->policy == SCHED_RR)
				{
					//实时进程----时间片
					q->sleep_avg = (TotalTime - q->RunTime) / 100;
					if (q->Time_slice >= (q->NeedTime - q->RunTime))
					{
						TotalTime = TotalTime + q->NeedTime - q->RunTime;
						q->RunTime = q->NeedTime;
						q->status = 'E';
						if (q->next == NULL)
						{
							/*PCB *p;
							p = q;
							q = q->next;
							delete p;*/
							q->prior = NULL;
							q = q->next;
							break;
						}
						PCB *p;
						p = q->prior;
						q->prior = q->next;
						q->next->prior = p;
						p = q;
						q = q->next;
						//delete q;
					}
					else
					{
						TotalTime = TotalTime + q->Time_slice;
						q->RunTime = q->RunTime + q->Time_slice;
//						q->prio = max(100, min(q->static_prio - GetBonus(*q) + 5, 139));
						if (Runq.Expired->queue[q->prio] == NULL)
						{
							Runq.Expired->queue[q->prio] = q;
						}
						else
						{
							PCB *p;
							p = Runq.Expired->queue[q->prio];
							while (p != NULL)
							{
								p = q->next;
							}
							PCB *work = p;
							p = q;
							work = p;
						}
						Runq.Expired->bitmap.set(q->prio);
						if (q->next == NULL)
						{
							q->prior = Runq.Expired->queue[q->prio];
							q = q->next;
							break;
						}
						PCB *p;
						p = q->prior;
						q->prior = q->next;
						q = q->next;
						q->prior = p;
					}
				}
				
			}
			print();
			Runq.Active->queue[i] = q;
			Runq.Active->bitmap.reset(i);
//			Sleep(1000);
//			system("cls");
		}
	}
	prio_array_t *ex;
	ex = Runq.Active;
	Runq.Active = Runq.Expired;
	Runq.Expired = ex;
	Execute();
}

void Processing::Insert_To_Array(PCB pcb)
{
}

void Processing::print()
{
	printf("----------------------------------------------------------------------------\n");
	printf("-进程名--需要时间--运行时间--时间片--策略--动优--实优--睡眠时间--静优--状态-\n");
	for (int i = 0; i < 8; i++)
	{
		printf("   %d    |   %4d  |  %4d   |  %3d  |%3d  | %3d | %3d |  %3d  |  %3d  |  %c\n", pcb[i].name, pcb[i].NeedTime, pcb[i].RunTime, pcb[i].Time_slice,
			pcb[i].policy, pcb[i].prio, pcb[i].rt_priority,  pcb[i].sleep_avg, pcb[i].static_prio, pcb[i].status);
	}
	printf("----------------------------------------------------------------------------\n");
}

void Processing::print(PCB *p)
{
	
}