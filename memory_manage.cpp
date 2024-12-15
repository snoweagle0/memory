#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
using namespace std;
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
mslist* list_sort(mslist* &head);
mslist* table;//内存块表
mslist* f_table;//空闲分区表
wklist* wkunit=NULL;
bool addwkunit(char workname[])
{

    wklist* pnew = wkunit, * plast = NULL;
    if (wkunit != NULL)
    {
        //链表不为空，新的进程在链表中使用尾插
        while (pnew != NULL)
        {
            if (strcmp(pnew->workname, workname) == 0)
                return false; // 如果链表中存在改进程，直接禁止添加
            plast = pnew;
            pnew = pnew->next;
        }
        if (pnew == NULL)
        {
            // 没有改进程分配新节点
            pnew = (wklist*)malloc(sizeof(wklist));
            strcpy(pnew->workname, workname);
            plast->next = pnew;
            pnew->next = NULL;
        }
    }
    else {
        //链表为空，新节点直接作为头节点
        pnew = (wklist*)malloc(sizeof(wklist));
        strcpy(pnew->workname, workname);
        wkunit = pnew;
        pnew->next = NULL;
    }
    return true;
}
bool deletunit(char workname[])//删除进程
{
    wklist* p = wkunit, * plast = NULL;
    if (wkunit == NULL)
    {
        printf("没有改进程且当前没有任何进程!\n");
        return false;
    }
    else {
        while (p != NULL)
        {
            if (strcmp(workname, p->workname) == 0)
            {
                if ( p == wkunit)
                {
                    wkunit = p->next;
                    free(p);
                    return true;
                }
                else if (p->next == NULL)
                {
                    plast->next = NULL;
                    free(p);
                    return true;
                }
                else if (p->next != NULL && plast != NULL)
                {
                    plast->next = p->next;
                    free(p);
                    return true;
                }
            }
            p = p->next;
        }
    }
    printf("没有找到改进程!\n");
    return false;
}
void Listsort(mslist*& head);
mslist* list_sort(mslist*& head);
wklist* wk_creat_node()
{
    return (wklist*)malloc(sizeof(wklist));
}
void display_mslist()//显示内存块表
{
    int i = 0;
    mslist* p = table;
   // Listsort(f_table);
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
       // if (p->state == false)
            printf("ID:%d\t \t起始地址:%u 占有空间:%d 状态:空闲\n", i, p->star_address, p->size);
       // else
       //     printf("ID:%d\t \t起始地址:%u 占有空间:%d 状态:占用\n", i, p->star_address, p->size);
        p = p->next;
        i++;
    }
}
mslist* creat_node()
{
    return (mslist*)malloc(sizeof(mslist));
}
mslist* creat()
{
    mslist* head=(mslist*)malloc(sizeof(mslist));
    head->star_address = (char*)malloc(101);
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
    Listsort(f_table);// 将空闲分区表按空闲块大小排序
    p = f_table;
    q = table;
    /*
    * p指针用于遍历空闲分区链表f_table
    * q指针用于遍历内存块链表table
    */
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
                        Listsort(f_table);
                        return 0;
                        /*
                        * 当内存中有大小正好合适的空闲分区时，直接将其状态进行修改
                        * 并从空闲分区链表当中移除
                        * 移除时注意分删头、删尾、删中间三种情况
                        */
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
                        Listsort(f_table);
                        return 0;
                        /*
                        * 没有正好合适的空闲大小时，我们的内存块链表需要新增一个节点
                        * 用来存放从大的空闲块中分割出来的小块，由于分割从高地址向低地址分割
                        * 所以这个新的块一定会插入在找到的空闲块(p也就是分割前的块)的后面
                        * 考虑插中间和插尾两种情况
                        */
                    }
                }
				q=q->next;
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
        deletunit(workname);
    }
    return 0;
}
int release_mem(char workname[], mslist* rhead, mslist* rtable)
{
    list_sort(f_table);
    mslist* s1 = NULL, * s2 = NULL, * t1 = NULL, * t2 = NULL, * head = rhead, *hlist=rtable, *hlist1=rtable;
	char *address=NULL;
    char* t = workname;
    while (head != NULL)
    {
        if (strcmp(t, head->workname) == 0)
        {
            t1 = head->last;
            t2 = head->next;
            //上为空
            if (t1 == NULL)
            {
                //下为空
                if (t2 == NULL)
                {
                    strcpy(head->workname, "NULL");
                    head->state = 0;
					//空闲表1
                    s1 = creat_node();
                    s1->star_address = head->star_address;
                    s1->size = head->size;
                    s1->state = 0;
                    s1->last = NULL;
                    s1->next = NULL;
                    f_table = s1;
                    list_sort(f_table);
                    printf("释放了！\n");
                    return 1;
                }
                else
                {
                    //下为0
                    if (t2->state == 0)
                    {
						address=t2->star_address;
                        t2->size += head->size;
                        t2->star_address = head->star_address;
                       // strcpy(t2->workname, "NULL");
                        s1 = head;
                        head = t2;
                        free(s1);
						//空闲表2
						while(hlist!=NULL)
						{
							if(hlist->star_address==address)
							{
								hlist->star_address=head->star_address;
								hlist->size=head->size;
								hlist->last=NULL;
								list_sort(rtable);
								break;
							}
							else
							{
								hlist=hlist->next;
							}

						}
                        printf("释放了！\n");
                        return 1;
                    }
                    else
                    {
                        strcpy(head->workname, "NULL");
                        head->state = 0;
						//空闲表3
                        s1 = creat_node();
                        s1->star_address = head->star_address;
                        s1->size = head->size;
                        s1->state = 0;
                        s1->last = NULL;
						if(f_table!=NULL)
						{
						    s1->next = f_table;
						    f_table->last = s1;
						    f_table = s1;
						    list_sort(f_table);
						}
						else
						{
							s1->last=NULL;
							s1->next=NULL;
							f_table=s1;
						}
                        printf("释放了！\n");
                        return 1;
                    }
                    
                }
            }
            else//上不为空
                if (t1->state == 0)
                {
                    if (t2 == NULL)//释放尾节点
                    {
                        t1->size += head->size;
                        t1->next = head->next;
                        s1 = head;
                        head = t1;
                        free(s1);
                        //空闲表4
						while(hlist!=NULL)
						{
							if(hlist->star_address==head->star_address)
							{
								hlist->star_address=head->star_address;
								hlist->size=head->size;
								list_sort(rtable);
								break;
							}
							else
							{
								hlist=hlist->next;
							}

						}
                        printf("释放了！\n");
                        return 1;
                    }
                    else if (t2->state == 0)//合并三个空闲分区
                    {
                        t1->size += head->size + t2->size;
                        s1 = head;
                        s2 = head->next;
						address=s2->star_address;
                        t1->next = s2->next;
                        if(t2->next!=NULL)
                        t2->next->last = t1;
                        head = t1;
                        free(s1);
                        free(s2);
                        //空闲表5
						while(hlist!=NULL)
						{
							if(hlist->star_address==head->star_address)
							{
								hlist->star_address=head->star_address;
								hlist->size=head->size;
								while(hlist1!=NULL)
								{
									if(hlist1->star_address==address)
									{
										s1=hlist1->last;
										s2=hlist1->next;
										s1->next=hlist1->next;
										if(s2!=NULL)
										s2->last=hlist1->last;
										s1=hlist1;
										free(s1);
										list_sort(rtable);
										break;
									}
									else
									{
										hlist1=hlist1->next;
									}
								}
								break;
							}
							else
							{
								hlist=hlist->next;
							}

						}
                        /*s1 = creat_node();
                        s1->star_address = head->star_address;
                        s1->size = head->size;
                        s1->state = 0;
                        s1->next = NULL;
                        while (hlist->next!=NULL)
                        {
                            hlist = hlist->next;
                        }
                        hlist->next = s1;
                        s1->last = hlist;
                        list_sort(rtable);*/
                        printf("释放了！\n");
                        return 1;
                    }
                    else//向上合并
                    {
                        t1->size += head->size;
                        t1->next = head->next;
                        t2->last = t1;
                        s1 = head;
                        head = t1;
                        free(s1);
						//空闲表6
						while(hlist!=NULL)
						{
							if(hlist->star_address==head->star_address)
							{
								hlist->star_address=head->star_address;
								hlist->size=head->size;
								list_sort(rtable);
								break;
							}
							else
							{
								hlist=hlist->next;
							}

						}
                        printf("释放了！\n");
                        return 1;
                    }
                }
                else if (t2 == NULL)//释放尾节点
                {
                    strcpy(head->workname, "NULL");
                    head->state = 0;
					//空闲表7
					s1=creat_node();
					s1->star_address=head->star_address;
					s1->size=head->size;
					s1->state=0;
					if(hlist!=NULL)
					{
						while(hlist->next!=NULL)
						{
							hlist=hlist->next;
						}
						hlist->next=s1;
						s1->last=hlist;
						s1->next=NULL;
						list_sort(rtable);
					}
					else
					{
						s1->next=NULL;
						s1->last=NULL;
						f_table=s1;
						list_sort(rtable);
					}
                    printf("释放了！\n");
                    return 1;
                }
                else if (t2->state == 0)//向下合并
                {
					address=t2->star_address;
                    t2->size += head->size;
                    t2->star_address = head->star_address;
					t1->next=t2;
					t2->last=t1;
                    s1 = head;
					head = t2;
                    free(s1);
					//空闲表8
					while(hlist!=NULL)
						{
							if(hlist->star_address==address)
							{
								hlist->star_address=head->star_address;
								hlist->size=head->size;
								list_sort(rtable);
								break;
							}
							else
							{
								hlist=hlist->next;
							}

						}
                    printf("释放了！\n");
                    return 1;
                }
                else
                {
                    head->state = 0;
                    strcpy(head->workname, "NULL");
					//空闲表9
                    s1 = creat_node();
                    s1->star_address = head->star_address;
                    s1->size = head->size;
                    s1->state = 0;
                    s1->next = NULL;
                    if (hlist != NULL)
                    {
                        while (hlist->next != NULL)
                        {
                            hlist = hlist->next;
                        }
                        hlist->next = s1;
                        s1->last = hlist;
                        list_sort(f_table);
                    }
                    else
                    {
                        s1->last = NULL;
                        f_table = s1;
                        list_sort(f_table);
                    }
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
void Listsort(mslist*& head) {          //按照内存大小排序
    if (head == NULL || head->next == NULL)
        return;
    mslist* p = NULL, * q = head, * q1, * qhead; // p是排序后的
    while (q->next != NULL)
    {
        q1 = q->next;
        q->next = NULL;
        q->last = NULL;
        q1->last = NULL;
        int flag = 0;
        if (p == NULL) {
            p = q;
            q = q1;
            continue;
        }
        while (p->size < q->size && p->next != NULL)
        {
            p = p->next;
        }
        if (p->size < q->size) {
            p->next = q;
            q->last = p;
        }
        else {
            if (p->last != NULL) {
                mslist* t = p->last;
                t->next = q;
                q->last = t;
                q->next = p;
                p->last = q;
            }
            else {
                q->next = p;
                p->last = q;
            }
        }
        q = q1;
        while (p->last != NULL) {
            p = p->last;
        }
    }
    while (p->size < q->size && p->next != NULL) {
        p = p->next;
    }
    if (p->size < q->size) {
        p->next = q;
        q->last = p;
    }
    else {
        if (p->last != NULL) {
            mslist* t = p->last;
            t->next = q;
            q->last = t;
            q->next = p;
            p->last = q;
        }
        else {
            q->next = p;
            p->last = q;
        }
    }
    while (p->last != NULL) {
        p = p->last;
    }
    head = p;
}
mslist* list_sort(mslist*& head)   // 按地址大小排序
{
    if (head == NULL || head->next == NULL)
        return NULL;
    mslist* p = NULL, * q = head, * q1, * qhead; // p是排序后的
    while (q->next != NULL)
    {
        q1 = q->next;
        q->next = NULL;
        q->last = NULL;
        q1->last = NULL;
        int flag = 0;
        if (p == NULL) {
            p = q;
            q = q1;
            continue;
        }
        while (p->star_address < q->star_address && p->next != NULL)
        {
            p = p->next;
        }
        if (p->star_address < q->star_address) {
            p->next = q;
            q->last = p;
        }
        else {
            if (p->last != NULL) {
                mslist* t = p->last;
                t->next = q;
                q->last = t;
                q->next = p;
                p->last = q;
            }
            else {
                q->next = p;
                p->last = q;
            }
        }
        q = q1;
        while (p->last != NULL) {
            p = p->last;
        }
    }
    while (p->star_address < q->star_address && p->next != NULL) {
        p = p->next;
    }
    if (p->star_address < q->star_address) {
        p->next = q;
        q->last = p;
    }
    else {
        if (p->last != NULL) {
            mslist* t = p->last;
            t->next = q;
            q->last = t;
            q->next = p;
            p->last = q;
        }
        else {
            q->next = p;
            p->last = q;
        }
    }
    while (p->last != NULL) {
        p = p->last;
    }
    head = p;
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
   // free_init(f_table);
    while(1)
    {
        flag = false;

        printf("___>");
       scanf("%s",str);
        if (strcmp(str, "") == 0)
            continue;
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
            if(addwkunit(work))
            request_mem(r_size, work);
            else {
                printf("该进程已存在！\n");
                continue;
            }
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
            if(deletunit(work))
            release_mem(work,table,f_table);
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

