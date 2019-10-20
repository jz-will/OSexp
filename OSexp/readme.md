# 操作系统小组实验
**目的与技术**
- 暂定使用O(1)调度算法和银行家算法；
- 各个算法模块分开以便分工共同开发。

**数据结构**
- 具体请查看代码注释。

**进程调度算法说明**
- 实时进程
>1、实时进程共有两种调度方式（FIFO、RR）每次运行先判断调度方式，FIFO方式实际就是单纯的FCFS算法；
>RR就要计算时间片。
>2、实时进程中的静态优先级仅用于计算RR算法时的时间片，动态优先级直接由rt_priority反向映射得出。
>rt_priority越大，优先级越高，prio越低，优先级越高，调度优先级统一仅看prio。
- 普通进程
>rt_priority全部设置为-1，调度方式统一，prio决定调度算法，static_prio决定时间片大小，每次运行之后
>会根据睡眠时间确定bonus再调整优先级。

**关于test**
- 测试数据使用的是自定义数据，并在运行之后再模拟插入一个数据，通过银行家算法检测；
- test的数据可控，能够可视化即可。

**下载说明**
- git clone或者直接下载；
- 下载后用vs2017打开.sln项目文件即可；
- Myhead.h是唯一头文件，更新头或结构体请及时推送。

**参考资料**
- [O(n)、O(1)和CFS调度器](http://www.wowotech.net/process_management/scheduler-history.html)
- [【转载】非实时进程、实时进程、静态优先级、动态优先级、实时优先级](https://blog.csdn.net/liuxingen/article/details/46574689)
- [linux2.6 O(1)调度系统分析](https://blog.csdn.net/xuao20060793/article/details/47305651)
- **[Linux2.6 内核进程调度分析](https://blog.csdn.net/DLUTBruceZhang/article/details/8694793)**
- [Linux进程调度策略的发展和演变--Linux进程的管理与调度(十六）](https://blog.csdn.net/gatieme/article/details/51701149)
- [Linux进程调度-------O(1)调度和CFS调度器](https://blog.csdn.net/a2796749/article/details/47101533)