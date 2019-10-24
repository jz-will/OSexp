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

//��ʼ����8��PCB�����飬λͼ�����м�
void  Processing::Inital()
{
	Runq.array[0].bitmap.reset();
	int NeedTime[8] = { 3000,600,500,800,700,400,900,2000 };
	int rt_priority[8] = { 2,15,65,99,-1,-1,-1,-1 };
	int static_prio[8] = { 100,123,130,132,130,122,122,125 };
	int policy[8] = { SCHED_RR,SCHED_FIFO,SCHED_RR,SCHED_FIFO,SCHED_NORMAL ,SCHED_NORMAL, SCHED_NORMAL, SCHED_NORMAL };
	int Claim[8][3] = { {2,1,5},{1,3,2},{3,5,4},{6,4,1},{5,3,4},{2,5,6},{8,5,3},{5,6,7} };	//A:32, B:32, C:32 -->(8, 8, 8)
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
			//��ͨ����
			pcb[i].prio = max(100, min(pcb[i].static_prio - GetBonus(pcb[i]) + 5, 139));
			pcb[i].Time_slice = SetTimeSlice(pcb[i]);
		}
		else
		{
			//ʵʱ����
			pcb[i].Time_slice = SetTimeSlice(pcb[i]);
			pcb[i].prio = MAX_RT_PRIO - 1 - pcb[i].rt_priority;		//0-99
		}
		pcb[i].Claim[0] = Claim[i][0];
		pcb[i].Claim[1] = Claim[i][1];
		pcb[i].Claim[2] = Claim[i][2];
		pcb[i].Allocation[0] = 0;
		pcb[i].Allocation[1] = 0;
		pcb[i].Allocation[2] = 0;
	}
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		Runq.array[0].queue[i] = NULL;
		Runq.array[1].queue[i] = NULL;
	}
	Runq.Active = &Runq.array[0];
	Runq.Expired = &Runq.array[1];
	//���ݶ�̬���ȼ��趨����
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
	//��ʼ����pcb��statusΪN
	pcb[8].name = -1;
	pcb[8].status = 'N';
	pcb[8].Claim[0] = 0;
	pcb[8].Claim[1] = 0;
	pcb[8].Claim[2] = 0;
	pcb[9].name = -1;
	pcb[9].status = 'N';
	pcb[9].Claim[0] = 0;
	pcb[9].Claim[1] = 0;
	pcb[9].Claim[2] = 0;
	banker.__init__(pcb);
}

void Processing::Execute()
{
	static int pos = 0;
	int PCBNum = 0;
	/*for (int i = 0; i < PCBsize; i++)
	{
		if (pcb[i].status != 'N')
		{
			PCBNum++;
		}
	}*/
	print();
	if (!Runq.Active->bitmap.any())
	{
		cout << "������ȫ�����н�����" << endl;
		return;
	}
	//1.�ҵ�Ϊ1��λ�ã�
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		if (Runq.Active->bitmap[i] == 1)
		{
			//2.�ж����ͺ����з�ʽ;
			PCB *q;
			q = Runq.Active->queue[i];
			while (q != NULL)
			{
				pos = pos + 1;
				//��ȫ�����жϺ���Դ����
				if (q->status=='W' || q->status =='R')
				{
					//while()
					if (!distriRes(*q, pos))
					{
						q->status = 'W';
					}
				}
				/*if ((pos % 5) == 0 && (pcb[pos%PCBNum].status != 'E'))
				{
					if (!distriRes(pcb[pos%PCBNum], pos))
					{
						pcb[pos%PCBNum].status = 'W';
					}
				}*/
				if (q->status == 'W')
				{
					q->sleep_avg = (TotalTime - q->RunTime) / 100;
					if (q->rt_priority == -1)
					{
						q->prio = max(100, min(q->static_prio - GetBonus(*q) + 5, 139));
					}
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
				if (q->policy == SCHED_NORMAL)
				{
					//��ͨ����
					q->sleep_avg = (TotalTime - q->RunTime) / 100;
					if (q->Time_slice >= (q->NeedTime - q->RunTime))
					{
						TotalTime = TotalTime + q->NeedTime - q->RunTime;
						q->RunTime = q->NeedTime;
						q->status = 'E';
						banker.freeRes(q->name);
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
						//	q->prior = Runq.Expired->queue[q->prio];
							PCB *work = Runq.Expired->queue[q->prio];
							while (work != NULL)
							{
								work = work->next;
							}
							PCB *c = work;
							work = q->prior;
							c = work;
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
					//ʵʱ����---��ռ����������˳�
					TotalTime = TotalTime + q->NeedTime;
					q->RunTime = q->NeedTime;
					q->status = 'E';
					banker.freeRes(q->name);
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
					//ʵʱ����----ʱ��Ƭ
					q->sleep_avg = (TotalTime - q->RunTime) / 100;
					if (q->Time_slice >= (q->NeedTime - q->RunTime))
					{
						TotalTime = TotalTime + q->NeedTime - q->RunTime;
						q->RunTime = q->NeedTime;
						q->status = 'E';
						banker.freeRes(q->name);
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
				
//				Sleep(1000);
	//			system("cls");
			}
			Runq.Active->queue[i] = q;
			Runq.Active->bitmap.reset(i);

		}
	}
	prio_array_t *ex;
	ex = Runq.Active;
	Runq.Active = Runq.Expired;
	Runq.Expired = ex;
	Execute();
}

//�����µ�һ��PCB
void Processing::Insert_PCB(PCB p)
{
	for (int i = 0; i < PCBsize; i++)
	{
		if (pcb[i].status == 'N')
		{
			//pcbλ��Ϊ�գ�����벢��ֵ
			pcb[i].name = i + 1;
			pcb[i].status = 'R';
			pcb[i] = p;
			Runq.Active->bitmap[pcb[i].prio] = 1;
			if (Runq.Active->queue[pcb[i].prio] == NULL)
			{
				Runq.Active->queue[pcb[i].prio] = &pcb[i];
				pcb[i].prior = Runq.Active->queue[pcb[i].prio];
			}
			else
			{
				PCB *q;
				q = Runq.Active->queue[pcb[i].prio];
				while (q != NULL)
				{
					q = q->next;
				}
				q = &pcb[i];
				pcb[i].prior = Runq.Active->queue[pcb[i].prio];
				pcb[i].prior->next = q;
			}
			return;
		}
	}

}

void Processing::print()
{
	printf("----------------------------------------------------------------------------\n");
	printf("-������--��Ҫʱ��--����ʱ��--ʱ��Ƭ--����--����--ʵ��--˯��ʱ��--����--״̬--�ѷ�����Դ--������Դ\n");
	for (int i = 0; i < 8; i++)
	{
		printf("   %d    |   %4d  |  %4d   |  %3d  |%3d  | %3d | %3d |  %3d  |  %3d  |  %c |  (%d,%d,%d)  |  (%d,%d,%d)\n", pcb[i].name, pcb[i].NeedTime, pcb[i].RunTime, pcb[i].Time_slice,
			pcb[i].policy, pcb[i].prio, pcb[i].rt_priority,  pcb[i].sleep_avg, pcb[i].static_prio, pcb[i].status,pcb[i].Allocation[0], pcb[i].Allocation[1], pcb[i].Allocation[2],
			pcb[i].Claim[0], pcb[i].Claim[1], pcb[i].Claim[2]);
	}
	printf("----------------------------------------------------------------------------\n");
}

void Processing::print(PCB *p)
{
	
}

//��Դ����
bool Processing::distriRes(PCB p, int pos)
{
	int req[resKind];
	for (int i = 0; i < resKind; i++)
	{
		/*if(p.policy == SCHED_FIFO)
		{
			req[i] = p.Claim[i];
			if(req[])
		}*/
		req[i] = max(0, min((pos % 3), (p.Claim[i] - p.Allocation[i])));//0 1 2
	}
	if (req[0] == 0 && req[1] == 0 && req[2] == 0)
	{
		//cout << "???";
		return true;
	}
	return banker.allotRes(p.name, req);	//1,0,1
}