#include"Myhead.h"


/*
O(1)调度算法：
	1、初始化队列与双向链表；
	2、运行前计算时间片；
	3、先由位图判断是否有进程，有则运行Active，用完时间片后放到Expired；(两个指针分别指向不同的prios_array_t)
	4、交换Active和Expired，重复3，到进程全部运行完毕;
	5、有新进程进入就绪队列时插入相应位置，与正在运行进程比较优先级，在前则运行，在后则等待运行。
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