#include "Myhead.h"

/*
主函数：
	1、模拟运行调度算法，运行默认存在的进程队列；
	2、模拟插入新的进程，经过银行家算法判断后再进入；
	3、插入的新进程中一个存在安全序列一个不存在安全序列，用于比较。
*/
int main()
{
	Processing sched;
	//sched.Inital();
	sched.print();
	sched.Execute();
	system("pause");
	return 0;
}