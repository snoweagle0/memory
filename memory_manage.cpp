#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void display_freelist();
typedef struct memory_unit{ // 内存块结构体
    int size;
    char* star_address;
    bool state;
    char workname[256];
    struct memory_unit* next;
    struct memory_unit* last;
}mslist;
typedef struct work_unit {  
    char workname[256];
    char* star_address;
    int size;
    struct work_unit* next;
}wklist;
mslist* table;//内存块表
mslist* f_table;//空闲分区表
wklist* wkunit;
wklist* wk_creat_node()
{
    return (wklist*)malloc(sizeof(wklist));
}
void display_mslist()//显示内存块表
{
    int i = 0;
    mslist* p = table;
    printf("内存块表\n");
    //printf("ID 进程名 起始地址 占有空间 状态\t\n");
    while (p != NULL)
    {
        if(p->state==false)
        printf("ID:%d\t进程名:%s 起始地址:%u 占有空间:%d 状态:空闲\n",i,p->workname,p->star_address,p->size);
        else
            printf("ID:%d\t进程名:%s 起始地址:%u 占有空间:%d 状态:占用\n", i, p->workname, p->star_address, p->size);
        p = p->next;
        i++;
    }
    display_freelist();
}
void display_freelist()//显示空闲分区表
{
    int i = 0;
    mslist* p = f_table;
    printf("空闲分区表\n");
    //printf("ID 进程名 起始地址 占有空间 状态\t\n");
    while (p != NULL)
    {
        if (p->state == false)
            printf("ID:%d\t \t起始地址:%u 占有空间:%d 状态:空闲\n", i, p->star_address, p->size);
        else
            printf("ID:%d\t \t起始地址:%u 占有空间:%d 状态:占用\n", i, p->star_address, p->size);
        p = p->next;
        i++;
    }
}
mslist* creat_node()
{
    return (mslist*)malloc(sizeof(mslist));
}
mslist* free_init(mslist* head)
{
    return NULL;
}
mslist* creat()
{
    mslist* head=(mslist*)malloc(sizeof(mslist));
    head->star_address = (char*)malloc(100);
    head->next = NULL;
    head->last = NULL;
    head->size = 100;
    head->state = false;
    strcpy(head->workname, "NULL\0");
    f_table= (mslist*)malloc(sizeof(mslist));
    f_table->last = NULL;
    f_table->next = NULL;
    f_table->star_address = head->star_address;
    f_table->size = head->size;
    f_table->state = head->state;
    return head;
}
int request_mem(int r_size,char workname[])
{
    
    
    mslist* p, *q,* pnew;
    mslist* f_last, * f_next;
    p = f_table;
    q = table;
    while (p != NULL)
    {
        if (p->size >= r_size)
        {
            while (q != NULL)
            {
                if (q->star_address == p->star_address&&q->state==false&&q->size>=r_size)
                {
                    if (q->size == r_size)
                    {
                        strcpy(q->workname, workname);
                        q->state = true;
                        f_last = p->last;
                        f_next = p->next;
                        f_last->next = p->next;
                        f_next->last = p->last;
                        free(p);
                        return 0;
                   }
                    else {
                        p->size = p->size - r_size;
                        pnew = creat_node();
                        strcpy(pnew->workname, workname);
                        pnew->star_address = q->star_address + q->size - r_size;
                        pnew->size = r_size;
                        pnew->state = true;
                        pnew->last = q;
                        pnew->next = q->next;
                        q->next = pnew;
                        q->size =q->size - r_size;
                        return 0;
                    }
                }
            }
            if (q == NULL)
            {
                printf("error 请检查内存分区表和空闲分区表！\n");
                return -1;
            }
        }
        p = p->next;
    }
    if (p == NULL)
    {
        printf("空间不足!\n");
    }
    return 0;
}
int release_mem(char workname[],mslist* rhead)
{
   
    return 0;
}
int list_sort(mslist* head)
{
    return 0;
}
int main()
{
    char str[256] = {"\0"};
    char fun[5][256] = { "request","release","display","exit"};
    char work[256];
    int r_size = 0;
    table = creat();
    bool flag = false;
    while(1)
    {
        flag = false;
        printf("___>");
        scanf("%s",str);
        if (strcmp(str, fun[0]) == 0)
        {
            printf("Enter the process name and required memory space.\n");
            printf("___>");
            scanf("%s",work);
            scanf("%d",&r_size);
            if (strlen(work) <= 0)
            {
                printf("work name error.\n");
                continue;
            }
            if (r_size <= 0)
            {
                printf("Error! The memory space must be greater than 0.\n");
                continue;
            }
            request_mem(r_size, work);
        }
        else if (strcmp(str, fun[1]) == 0)
        {
            printf("Enter the process name.\n");
            printf("___>");
            scanf("%s",work);
            if (strlen(work) <= 0)
            {
                printf("work name error.\n");
                continue;
            }
            release_mem(work,table);
        }
        else if(strcmp(str,fun[2])==0)
        {
            display_mslist();
        }
        else if (strcmp(str, fun[3]) == 0)
        {
            return 0;
        }
        else {
            printf("Erorr! Can't find the instruction.\n");
            continue;
        }
    }
}

