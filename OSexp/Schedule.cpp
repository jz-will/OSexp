#include"Myhead.h"


/*
O(1)�����㷨��
	1����ʼ��������˫������
	2������ǰ����ʱ��Ƭ��
	3������λͼ�ж��Ƿ��н��̣���������Active������ʱ��Ƭ��ŵ�Expired��(����ָ��ֱ�ָ��ͬ��prios_array_t)
	4������Active��Expired���ظ�3��������ȫ���������;
	5�����½��̽����������ʱ������Ӧλ�ã����������н��̱Ƚ����ȼ�����ǰ�����У��ں���ȴ����С�
*/
class Processing {
private:
	prio_array_t prio_array_t[ARRAY_SIZE];
	Runqueue Runqueue;
public:
	Processing() {}
	~Processing() {}
	//
};