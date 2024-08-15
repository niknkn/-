#include "res.h"

//显示界面
void show(){
    printf("---欢迎使用停车管理系统---\n");
    printf("办理停车程序请按------> *1*\n");
    printf("办理结算程序请按------> *2*\n");
    printf("查询车辆信息请按------> *3*\n");
    printf("查询排队信息请按------> *4*\n");
    printf("退出停车程序请按------> *5*\n");
    if(carnum<MAXCARNUM && waitcarnum>0){
        //符合停车场还有空位，队列不为空
        waitcargoto(&waithead,&head,&tail,(*waithead).carid);
    }
    int n;
    scanf("%d",&n);
    switch (n)
    {
    case 1:
        showjinru();
        break;
    case 2:
        showtuichu();
        break;
    case 3:
        showchaxun();
        break;
    case 4:
        system("clear");
        allwaitlist(waithead);
        break;
    case 5:
        printf("欢迎再次使用该程序！\n");
        exit(-1);
    default:
        printf("输入的信息无效\n");
        exit(-1);
    }

}
//进入界面
void showjinru(){
    system("clear");
    printf("请输入您的车牌号\n");
    char id[10];
    scanf("%s",id);
    if(strlen(id)!=9){
        printf("您输入的车牌号不符合规范！\n");
        return;
    }

    CAR* temp=head;
    while(temp){
        if((strcmp(id,temp->carid))==0){
            printf("场内已有该车\n");
            return;
        }
        temp=temp->next;
    }
    
    if(carnum>=MAXCARNUM){
        printf("停车场所停车辆已达上限\n");
        printf("请问是否排队\n排队请输入---1  离开请输入---2\n");
        int n;
        scanf("%d",&n);
        if(n==1){
            //确定排队则头插进排队链表
            waitlist(&waithead,id,&waittail);
            //遍历排队车辆
            allwaitlist(waithead);
        }
        return;
    }
    inputlist(&head,id,&tail);
}

//退出界面
void showtuichu(){
    system("clear");
    dellist(&head,&tail);
}
//查询界面
void showchaxun(){
    int n;
    system("clear");
    printf("查询所有车辆信息请按 >>> *1*\n");
    printf("查询个人车辆信息请按 >>> *2*\n");
    scanf("%d",&n);
    if(n==1){
        alllist(head);
        return;
    }
    if(n==2){
        printf("输入你需要查找的车牌号\n");
        char id[10];
        scanf("%s",id);
        onelist(head,id);
        return;
    }
        printf("输入无效\n");
}