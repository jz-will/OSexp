#include"Myhead.h"


/*
O(1)调度算法：
	1、初始化队列与双向链表；
	2、运行前计算时间片；
	3、先由位图判断是否有进程，有则运行Active，用完时间片后放到Expired；(两个指针分别指向不同的prios_array_t)
	4、交换Active和Expired，重复3，到进程全部运行完毕;
	5、有新进程进入就绪队列时插入相应位置，与正在运行进程比较优先级，在前则运行，在后则等待运行。
*/
/* bitset说明：
	bitset::any()->是否存在1；
	bitset::set(pos)->设置某个位置为1
	bitset::reset(pos)->设置某个位置为0
	bitset可直接用数组形式访问某个位置

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
/*初始化：first_array设置为原型，second_*/
Processing::Processing()
{

}

void Processing::Add_To_Array(PCB pcb)
{
}
