#include"Myhead.h"


/*
O(1)�����㷨��
	1����ʼ��������˫������
	2������ǰ����ʱ��Ƭ��
	3������λͼ�ж��Ƿ��н��̣���������Active������ʱ��Ƭ��ŵ�Expired��(����ָ��ֱ�ָ��ͬ��prios_array_t)
	4������Active��Expired���ظ�3��������ȫ���������;
	5�����½��̽����������ʱ������Ӧλ�ã����������н��̱Ƚ����ȼ�����ǰ�����У��ں���ȴ����С�
*/
/* bitset˵����
	bitset::any()->�Ƿ����1��
	bitset::set(pos)->����ĳ��λ��Ϊ1
	bitset::reset(pos)->����ĳ��λ��Ϊ0
	bitset��ֱ����������ʽ����ĳ��λ��

*/
class Processing {
private:
	prio_array_t first_array[ARRAY_SIZE];
	prio_array_t second_array[ARRAY_SIZE];
	Runqueue Runq;
	bitset<BITMAP_SIZE> bitmap;

public:
	Processing();
	~Processing() {}
	void Add_To_Array(PCB pcb);
	void Execute();
};
/*��ʼ����first_array����Ϊԭ�ͣ�second_*/
Processing::Processing()
{

}

void Processing::Add_To_Array(PCB pcb)
{
}
