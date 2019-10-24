#include"Myhead.h"

void BankerCal::__init__(PCB pcb[])
{
	Available[0] = 10;
	Available[1] = 10;
	Available[2] = 10;
	//初始化资源需求表
	for (int i = 0; i < PCBsize; i++)
	{
		PCBTable[i] = &pcb[i];
		for (int j = 0; j < resKind; j++)
		{
			Claim[i][j] = PCBTable[i]->Claim[j];
			Need[i][j] = PCBTable[i]->Claim[j] - pcb[i].Allocation[j];
		}
	}
}
bool BankerCal::isSave()
{
	stack[0] = 0;
	int m = 0;
	int size = 0;
	int work[resKind];
	bool finish[PCBsize];
	int secureNum = 0;
	for (int i = 0; i < resKind; i++)
	{
		work[i] = Available[i];
	}

	for (int i = 0; i < PCBsize; i++)
	{
		finish[i] = false;					//初始化finish向量
		if (PCBTable[i]->status == 'R' || PCBTable[i]->status == 'W')		// 查找就绪/等待状态进程数
			size++;
	}
	while (secureNum < size)
	{
		for (int i = 0; i < PCBsize; i++)
		{
			if (PCBTable[i] == NULL)continue;
			else if (PCBTable[i]->status == 'E' || PCBTable[i]->status == 'N' || finish[i] == true)  continue;	//寻找Finish == false 且 未结束进程

			else if (Need[i][0] > work[0] || Need[i][1] > work[1] || Need[i][2] > work[2]) continue; //检查Need 是否小于 空闲

			else
			{
				for (int j = 0; j < resKind; j++)
				{
					work[j] += PCBTable[i]->Allocation[j];
				}
				finish[i] = true;
				secureNum++;
				stack[stack[0] + 1] = i;
				stack[0] = stack[0] + 1;
				break; //找到符合进程,退出当前for循环,进行下一次寻找
			}
			
		}
		m++;
		if (m == size)
		{
			if (secureNum == size) return true;
			else return false;
		}
	}
}

bool BankerCal::allotRes(int PCBName, int request[])
{
	int PCBid = PCBName - 1;
	for (int i = 0; i < resKind; i++)
	{
		if (request[i] > Need[PCBid][i])   //检查请求资源合法性
		{
			cout << endl << "当前进程请求大于需求" << endl;
			return false;
		}
		else if (request[i] > Available[i])   //检查资源剩余量
		{
			cout << endl << "当前请求大于空闲" << endl;
			//PCBTable[PCBid+1]->status = 'W';   //剩余资源小于请求资源 进程进入等待
			return false;
		}
	}
	//循环正常结束,试分配资源
	for (int j = 0; j < resKind; j++)
	{
		/* code */
		Available[j] = Available[j] - request[j];
		Allocation[PCBid][j] = Allocation[PCBid][j] + request[j];
		Need[PCBid][j] = Need[PCBid][j] - request[j];
	}

	if (isSave())
	{
		printf("安全序列：");
		for (int i = 1; i <= stack[0]; i++)
		{
			if (i == stack[0])
				printf("P%d", stack[i]);
			else
				printf("P%d->",stack[i]);
		}
		//唤醒, 分配
		PCBTable[PCBid]->status = 'R';
		PCBTable[PCBid]->Allocation[0] += request[0];
		PCBTable[PCBid]->Allocation[1] += request[1];
		PCBTable[PCBid]->Allocation[2] += request[2];
		printf("\n");
		return true;   //检查是否有安全序列 有则分配资源
	}
	else //否则收回资源,返回false
	{
		for (int j = 0; j < resKind; j++)
		{
			/* code */
			Available[j] = Available[j] + request[j];
			Allocation[PCBid][j] = Allocation[PCBid][j] - request[j];
			Need[PCBid][j] = Need[PCBid][j] + request[j];
		}
		return false;
	}
}

//释放资源
void BankerCal::freeRes(int PCBName)
{
	for (int i = 0; i < resKind; i++)
	{
		// PCBid == PCBname -1;
		Available[i] = Available[i] + PCBTable[PCBName - 1]->Allocation[i];
		PCBTable[PCBName - 1]->Allocation[i] = 0;
		//Need[PCBName - 1][i] = Claim[PCBName - 1][i];
	}
}