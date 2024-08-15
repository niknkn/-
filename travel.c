#include"res.h"


//遍历链表
void alllist(CAR* head){
    if (NULL == head) return; 
    while(head){    
        printf("车牌号为:%s\n",head->carid);
        head=head->next;
    }
    printf("停车场剩余总车数为:%d\n",carnum);
}

//遍历排队链表
void allwaitlist(Jcar* waithead){
    if (NULL == waithead) return;
    printf("目前排队车辆为\n"); 
    while(waithead){    
        printf("车牌号为:%s\n",waithead->carid);
        waithead=waithead->next;
    }
    printf("目前排队总车数为%d\n",waitcarnum);
}

//查找数据
void onelist(CAR* head,char* id){
    if (NULL == head) return; 
    while(head){
        if(strcmp(head->carid, id) == 0){
            printf("车牌号为:%s\n",head->carid);
            return;
        }
        head=head->next;
    }
    if (NULL == head){
        printf("没有该车\n");
    }
}