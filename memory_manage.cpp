#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct memory_unit{
    int size;
    void* star_address;
    bool state;
    char workname[256];
    struct memory_unit* next;
    struct memory_unit* last;
}mslist;
mslist* table;
mslist* creat()
{
    mslist* head=NULL;
    head = (mslist*)malloc(sizeof(mslist));
    head->last = NULL;
    head->star_address = malloc(1024);
    head->next = NULL;
    head->size = 1024;
    head->state = false;
    strcpy(head->workname, "\0");
    return head;
}
int request_mem(int r_size,char workname[])
{
    return 0;
}
int release_mem(char workname[])
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
    char fun[5][256] = { "request","release" };
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
                printf("work name error.");
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
                printf("work name error.");
                continue;
            }
            release_mem(work);
        }
        else {
            printf("Erorr! Can't find the instruction.\n");
            continue;
        }
    }
}

