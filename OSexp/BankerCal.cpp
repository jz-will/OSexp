#include"Myhead.h"

void BankerCal::__init__(PCB pcb[])
{
	Available[0] = 10;
	Available[1] = 10;
	Available[2] = 10;
	//��ʼ����Դ�����
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
		finish[i] = false;					//��ʼ��finish����
		if (PCBTable[i]->status == 'R' || PCBTable[i]->status == 'W')		// ���Ҿ���/�ȴ�״̬������
			size++;
	}
	while (secureNum < size)
	{
		for (int i = 0; i < PCBsize; i++)
		{
			if (PCBTable[i] == NULL)continue;
			else if (PCBTable[i]->status == 'E' || PCBTable[i]->status == 'N' || finish[i] == true)  continue;	//Ѱ��Finish == false �� δ��������

			else if (Need[i][0] > work[0] || Need[i][1] > work[1] || Need[i][2] > work[2]) continue; //���Need �Ƿ�С�� ����

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
				break; //�ҵ����Ͻ���,�˳���ǰforѭ��,������һ��Ѱ��
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
		if (request[i] > Need[PCBid][i])   //���������Դ�Ϸ���
		{
			cout << endl << "��ǰ���������������" << endl;
			return false;
		}
		else if (request[i] > Available[i])   //�����Դʣ����
		{
			cout << endl << "��ǰ������ڿ���" << endl;
			//PCBTable[PCBid+1]->status = 'W';   //ʣ����ԴС��������Դ ���̽���ȴ�
			return false;
		}
	}
	//ѭ����������,�Է�����Դ
	for (int j = 0; j < resKind; j++)
	{
		/* code */
		Available[j] = Available[j] - request[j];
		Allocation[PCBid][j] = Allocation[PCBid][j] + request[j];
		Need[PCBid][j] = Need[PCBid][j] - request[j];
	}

	if (isSave())
	{
		printf("��ȫ���У�");
		for (int i = 1; i <= stack[0]; i++)
		{
			if (i == stack[0])
				printf("P%d", stack[i]);
			else
				printf("P%d->",stack[i]);
		}
		//����, ����
		PCBTable[PCBid]->status = 'R';
		PCBTable[PCBid]->Allocation[0] += request[0];
		PCBTable[PCBid]->Allocation[1] += request[1];
		PCBTable[PCBid]->Allocation[2] += request[2];
		printf("\n");
		return true;   //����Ƿ��а�ȫ���� ���������Դ
	}
	else //�����ջ���Դ,����false
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

//�ͷ���Դ
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