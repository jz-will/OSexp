#include"Myhead.h"

/*
用于实现位图操作：
	1、从第一个位置开始遍历；
	2、判断该队列是否有进程；
	3、
*/
class BitMap {
public:
	BitMap(){}
	~BitMap() {}
	bool IsEmpty(prio_array_t array);
	void Execute();
};

bool BitMap::IsEmpty(prio_array_t array)
{
	return false;
}

void BitMap::Execute()
{
}
