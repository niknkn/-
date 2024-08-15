#ifndef _RES_H_
#define _RES_H_

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h>

#define MAXCARNUM 5

typedef struct car{
    struct car* prev; 
    //车牌，进入时间，离开时间
    char        carid[10];
    time_t      start;
    struct car* next;     
}CAR;

//进入时的队列
typedef struct jcar{
    struct jcar* prev; 
    char        carid[10];
    time_t      start;
    struct jcar* next; 
}Jcar;


//显示界面
void show();
//进入界面
void showjinru();
//退出界面
void showtuichu();
//查询界面
void showchaxun();



//创建节点
CAR* newlist(char* carid);
//创建排队节点
Jcar* newwaitlist(char* carid);
//插入节点，头插
void inputlist(CAR** head,char* carid,CAR** tail);



//查找数据
void onelist(CAR* head,char* id);
//遍历链表
void alllist(CAR* head);
//遍历排队链表
void allwaitlist(Jcar* waithead);



//删除数据
void dellist(CAR** head,CAR** tail);
//对新文件的写入
void fileadd(CAR* head);
//读取文件内容初始化链表
void newfilelist(CAR** head,CAR** tail);
//排队车辆进入排队队列
void waitlist(Jcar** waithead,char* id,Jcar** waittail);
//排队车辆进入停车场
//需要对排队队列进行头删，并且将节点尾插到停车场链表中
void waitcargoto(Jcar** waithead,CAR** head,CAR** tail,char* id);


#include "push.c"
#include "show.c"
#include "travel.c"

#endif