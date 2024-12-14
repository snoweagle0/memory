#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
using namespace std;
void display_freelist();
bool cmp();

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
mslist* list_sort(mslist* head);
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
    head->size = 99;
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
                        if (f_last != NULL && f_next != NULL)
                        {
                            f_last->next = p->next;
                            f_next->last = p->last;
                            free(p);
                        }
                        else if (p==f_table)
                        {
                            f_table = f_table->next;
                            free(p);
                        }
                        else if (p->next==NULL)
                        {
                            f_last->next = NULL;
                            free(p);
                        }
                        list_sort(f_table);
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
                        if (q->next != NULL)
                        {
                            q->next->last = pnew;
                        }
                        q->next = pnew;
                        q->size = q->size - r_size;
                        list_sort(f_table);
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
int release_mem(char workname[], mslist* rhead)
{
    mslist* s1 = NULL, * s2 = NULL, * t1 = NULL, * t2 = NULL, * head = rhead;
    char* t = workname;
    while (head != NULL)
    {
        if (strcmp(t, head->workname) == 0)
        {
            t1 = head->last;
            t2 = head->next;
            if (t1->state == 0)
            {
                if (t2 == NULL)
                {
                    t1->size += head->size;
                    t1->next = head->next;
                    s1 = head;
                    head = t1;
                    free(s1);
                    printf("释放了！\n");
                    return 1;
                }
                else if (t2->state == 0)
                {
                    t1->size += head->size + t2->size;
                    s1 = head;
                    s2 = head->next;
                    t1->next = s2->next;
                    head = t1;
                    free(s1);
                    free(s2);
                    printf("释放了！\n");
                    return 1;
                }
                else
                {
                    t1->size += head->size;
                    t1->next = head->next;
                    s1 = head;
                    head = t1;
                    free(s1);
                    printf("释放了！\n");
                    return 1;
                }
            }
            else if (t2 == NULL)
            {
                strcpy(head->workname, "NULL");
                head->state = 0;
                printf("释放了！\n");
                return 1;
            }
            else if (t2->state == 0)
            {
                t2->size += head->size;
                t2->star_address = head->star_address;
                head = t2;
                s1 = head;
                free(s1);
                printf("释放了！\n");
                return 1;
            }
            else
            {
                /*t1->next = head;
                head->last = t1;
                head->next = t2;
                t2->last = head;
                printf("释放了！\n");
                return 1;*/
                head->state = 0;
                strcpy(head->workname,"NULL");
                printf("释放了！\n");
                return 1;
            }
        }
        else
            head = head->next;
    }
    if (head == NULL)
    {
        printf("没有找到！\n");
        return 0;
    }
}
mslist* list_sort(mslist* head)
{
    if (head==NULL || head->next == NULL)
    {
        return head;
    }
    mslist* pn, * pl;
    pl = head;
    head = head->next;
    pl->next = NULL;
    pl->last = NULL;
    while (head != NULL)
    {
        pn = head;
        mslist* p = pl,*flag=NULL;
        while (p != NULL && pn->size > p->size)
        {
            flag = p;
            p = p->next;
        }
        if (p != NULL)
        {
            if (p->last != NULL)
            {
                pn->next = p;
                pn->last = p->last;
                p->last->next = pn;
                p->last = pn;
            }
            else if (p->last == NULL)
            {
                pn->last = NULL;
                pn->next = p;
                p->last = pn;
                pl = pn;
            }
        }
        else {
            flag->next = pn;
            pn->last = flag;
            pn->next = NULL;
        }
        head = head->next;
    }
    return pl;
}
int main()
{
    char str[256] = {"\0"};
    char fun[5][256] = { "request","release","display","exit"};
    char work[256];
    int r_size = 0;
    table = creat();
    list_sort(f_table);
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
            //printf("Enter you ")
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

