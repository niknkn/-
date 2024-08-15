#include "res.h"

CAR* head=NULL;
CAR* tail=NULL;
int carnum=0;

Jcar* waithead=NULL;
Jcar* waittail=NULL;
int waitcarnum=0;

//创建节点
CAR* newlist(char* carid){

    CAR* new=(CAR*)malloc(sizeof(CAR));
    if (NULL == new) {  
        printf("newlist函数中申请内存失败:%m\n");  
        return NULL;  
    }
    memset(new, 0, sizeof(CAR)); 
    strncpy(new->carid, carid, sizeof(new->carid));  
    return new;
}
//创建排队节点
Jcar* newwaitlist(char* carid){

    Jcar* new=(Jcar*)malloc(sizeof(Jcar));
    if (NULL == new) {  
        printf("newwaitlist函数中申请内存失败:%m\n");  
        return NULL;  
    }
    memset(new, 0, sizeof(Jcar)); 
    strncpy(new->carid, carid, sizeof(new->carid));  
    return new;
}
//读取文件内容初始化链表
void newfilelist(CAR** head,CAR** tail){
    //如果没有文件则生成一个空文件
    FILE* fq=fopen("heihei","a");
    fclose(fq);

    FILE* fp=fopen("heihei","r");
    fseek(fp,0,SEEK_END);
    int len=ftell(fp);
    char a[]={0};
    fseek(fp,0,SEEK_SET);
    CAR* temp=newlist(a);

    while (len)
    {   //新节点写入
        len-=17;
        CAR* new=newlist(a);
        fread(&(new->carid),1,9,fp);
        fread(&(new->start),1,8,fp);
        //头为空

        if(len==0&&*head!=NULL){
            *tail=new;
            temp->next=new;
            new->prev=temp;
            carnum++;
            break;
        }  
        if(*head==NULL){
            *head=*tail=new;
            temp=*head;
            carnum++;
        }else{
            temp->next=new;
            new->prev=temp;  
            temp=temp->next; 
            carnum++; 
        }
            
    }
    fclose(fp);

}

//排队车辆进入排队队列 尾插  
void waitlist(Jcar** waithead,char* id,Jcar** waittail){
   //不存在节点
    if(*waithead==NULL){
        *waithead=*waittail=newwaitlist(id); 
        waitcarnum++;  
        return;
    }
    //存在节点 尾插 1.新节点指向头结点 2.头结点的前指针指向新节点 3.头结点移动到新节点   
    Jcar* new = newwaitlist(id);
    (*waittail)->next=new;
    new->next=NULL;
    new->prev=*waittail;
    *waittail=new;
    waitcarnum++;
}

//插入节点，头插
void inputlist(CAR** head,char* carid,CAR** tail){ 
    if(carnum>MAXCARNUM){
        printf("停车场所停车辆已达上限\n");
        return;
    }

    //不存在节点
    if(*head==NULL){
        *head=*tail=newlist(carid);

        FILE* fp=fopen("heihei","ab");
        // 获取当前时间
        time(&((*head)->start));
        //先写车牌，再写时间
        fwrite(&((*head)->carid),1,strlen((*head)->carid),fp); //车牌号写入文件
        fwrite(&((*head)->start),1,8,fp); //汽车进入时间写入文件
        fclose(fp);
        carnum++;
        return;
    }     
    //存在节点 尾插 1.新节点指向头结点 2.头结点的前指针指向新节点 3.头结点移动到新节点
        
        CAR* new = newlist(carid);
        (*tail)->next=new;
        new->next=NULL;
        new->prev=*tail;
        *tail=new;
        FILE* fp=fopen("heihei","ab");
        // 获取当前时间
        time(&(new->start));
        fwrite(&(new->carid),1,strlen(new->carid),fp); //车牌号写入文件
        fwrite(&(new->start),1,sizeof(new->start),fp); //汽车进入时间写入文件  
        fclose(fp);
        carnum++;  
}

//排队车辆进入停车场
//需要对排队队列进行头删，并且将节点尾插到停车场链表中
void waitcargoto(Jcar** waithead,CAR** head,CAR** tail,char* id){
    //排队车辆进入停车场
    inputlist(head,id,tail);
    //对队列删头
    if(NULL == *waithead) return;
    //只有一个
    if(NULL ==(*waithead)->next){
        *waithead=NULL;
        waitcarnum--;
        return;
    }
    //第二个成为新的头
    char a[10]={0};
    Jcar* del=newwaitlist(a);
    del=*waithead;
    *waithead=del->next;    
    free(del);//释放
    del = NULL;
    waitcarnum--;
}

//删除
void dellist(CAR** head,CAR** tail){
    char id[10]={0};
    printf("请输入需要出停车场的车牌号\n");
    scanf("%s",id);

    if (NULL == *head) return;

    //找到要删除的节点
    CAR* del = *head;
    int pianyi=0;
    while(del){
        if(strcmp(del->carid,id) == 0) break;
        del=del->next;
        pianyi++;
    }

    if(del==NULL){
        printf("没有该车\n");
        return;
    } 
    
    //删头
    if(*head==del){
        time_t end;
        FILE* ff=fopen("heihei","r");
        //获取当前时间
        int endtime=time(&end);
        int starttime;
        fseek(ff,(strlen((*head)->carid)),SEEK_CUR);//偏移车牌位置
        fread(&starttime,1,8,ff);//读取进入停车场的时间
        int TCtime=endtime-starttime;//计算停车时长/s 
        int j=0;
        if(TCtime>(3600*j)){
            printf("您本次停车时间为%d秒\n",TCtime);
            printf("您本次消费%g元\n",5*j+TCtime*0.002);
            j++;        
        }
        fclose(ff);

        *head=del->next;
        free(del);
        del=NULL;

        fileadd(*head);

        system("rm -r heihei");
        printf("结算成功\n");
        carnum--;
        if(*head==NULL) return;
        system("mv wenjian heihei");
        
        return;     
    }
    //删尾
    if(*tail==del){

        time_t end;
        FILE* ff=fopen("heihei","r");
        //偏移光标位置
        fseek(ff,-(sizeof((*tail)->start)),SEEK_END);
        //获取当前时间
        int endtime=time(&end);
        int starttime;
        fread(&starttime,1,8,ff);//读取进入停车场的时间
        int TCtime=endtime-starttime;//计算停车时长/s 
        int j=0;
        if(TCtime>(3600*j)){
            printf("您本次停车时间为%d秒\n",TCtime);
            printf("您本次消费%g元\n",5*j+TCtime*0.002);
            j++;        
        }
        fclose(ff);
        
        *tail=del->prev;   
        (*tail)->next=NULL;    
        free(del);
        del=NULL;
        //新文件替代原文件
        fileadd(*head);
        system("rm -r heihei");
        system("mv wenjian heihei");

        printf("结算成功！\n");
        carnum--;
        return;
    } 
    //多个数据删中间
    time_t end;
    FILE* ff=fopen("heihei","r");
        //偏移光标位置
    CAR* temp1=*head;
    while (pianyi--)
    {
        fseek(ff,(sizeof(temp1->start)+strlen(temp1->carid)),SEEK_CUR);
        temp1=temp1->next;
    }
    fseek(ff,strlen(temp1->carid),SEEK_CUR);
    
    //获取当前时间
    int endtime=time(&end); 
    int starttime;
    fread(&starttime,1,8,ff);//读取进入停车场的时间
    int TCtime=endtime-starttime;//计算停车时长/s 
    int j=0;
    if(TCtime>(3600*j)){
        printf("您本次停车时间为%d秒\n",TCtime);
        printf("您本次消费%g元\n",5*j+TCtime*0.002);
        j++;        
    }
        fclose(ff);


    CAR* prevdel = del->prev; 
    CAR* nextdel = del->next;
    prevdel->next=nextdel;
    nextdel->prev=prevdel;
    free(del);
    del=NULL;

    fileadd(*head);

    system("rm -r heihei");
    system("mv wenjian heihei");

    printf("结算成功！\n");
    carnum--;
}

//删除后输入到新文件
void fileadd(CAR* head){
    if (NULL == head) return;
    FILE* fp=fopen("wenjian","ab");
    while(head){    
        fwrite(&(head->carid),1,strlen((head->carid)),fp); //车牌号写入文件
        fwrite(&(head->start),1,8,fp); //汽车进入时间写入文件
        head=head->next;
    }
    fclose(fp);
}
