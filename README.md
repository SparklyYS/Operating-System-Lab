# Operating-System-Lab

标签（空格分隔）： 实验简介

---

This repository will contain all experiments about operating system

###Lab3-进程运行轨迹的跟踪与统计
#####修改文件
 * main.c  
  * 作用：为了尽早访问log文件，将log文件描述符关联到3的代码从init（）到main（）中，同时加入打开log文件的代码。
  * 路径：Lab3/oslab/linux-0.11/init/main.c
 * fork.c  
  * 作用：调用fprintk，将进程状态写入log 
  * 路径：Lab3/oslab/linux-0.11/kernel/fork.c
 * exit.c
  * 作用：调用fprintk，将进程状态写入log
  * 路径：Lab3/oslab/linux-0.11/kernel/exit.c
 * sched.c
  * 作用：调用fprintk，将进程状态写入log 
  * 路径：Lab3/oslab/linux-0.11/kernel/sched.c
 * printk.c 
  * 作用：加入fprintk调用函数
  * 路径: Lab3/oslab/linux-0.11/kernel/printk.c
#####添加文件
 * process.c
   * 作用： 用户程序，模拟添加进程
 * process.log
   * 作用： 进程状态转移轨迹的记录
 * stat_log.py
   * 作用： 读入log原始数据，计算平均周转时间、平均等待时间和吞吐率
 




