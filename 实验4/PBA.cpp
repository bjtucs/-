//#include "stdafx.h"
#include "stdio.h"
#include"stdlib.h"
#include"time.h"
#define M 32    //物理内存块数
#define N 64   //虚拟内存块数

struct LNode
{
    int data;
    int flag;//访问位
    int modify;//修改位
    LNode* next;
};
struct Link
{
    int num;//当前链表上的结点数
    LNode* next;
};

void generate();//生成访问序列
bool  isInNodes (int n); //
void addToLink (int data, int type);
void emptyIdle();
void emptyModi();
void PBA (int n);
int size = 3;
int p;//工作集的起始位置
int table[32];//物理内存，每一个元素代表一个页面
int access[32]; //访问序列
int memo[3] = { -1, -1, -1 };
int lost = 0;//没找到的页面数
int index = 0;//指示当前下标
LNode* nodes;//改进型Clock置换算法用到的数据结构
Link idle;
Link modified;

int main (int argc, char* argv[])
{
    int i = 0, j = 0;
    generate();
    printf ("页面缓冲置换算法(PBA)\n");
    idle.num = 0;
    idle.next = NULL;
    modified.num = 0;
    modified.next = NULL;
    nodes = (LNode*) malloc (size * sizeof (LNode));
    
    for (i = 0; i < size; i++)
    {
        nodes[i].data = -1;
        nodes[i].flag = 0;
        nodes[i].modify = 0;
        nodes[i].next = NULL;
    }
    
    for (i = 0; i < 32; i++)
    {
        PBA (i);
        
        for (j = 0; j < size; j++)
        {
            printf ("%d ", nodes[j].data);
        }
        
        printf ("\n");
    }
    
    printf ("页面缓冲置换算法（PBA）缺页率：%f   %d\n", lost / 32.0, lost);
    getchar();
    getchar();
    return 0;
}



void generate()
{
    srand ( (unsigned) time (NULL)); //用时间做种，每次产生随机数不一样
    p = rand() % 64;
    int m = 8, e = 8;
    int i, j;
    double t;
    t = rand() % 10 / 10.0;
    
    for (i = 0; i < 4; i++)
    {
        for (j = i * m; j < (i + 1) *m; j++)
        {
            access[j] = (p + rand() % e) % 64;
        }
        
        double r = (rand() % 10) / 10.0;
        
        if (r < t)
        {
            p = rand() % 64;
        }
        
        else
        {
            p = (p + 1) % 64;
        }
    }
}

bool isInNodes (int n)
{
    int i;
    
    for (i = 0; i < 3; i++)
    {
        if (nodes[i].data == access[n])
        {
            return true;
        }
    }
    
    return false;
}

LNode* isinLinks (int n)
{
    LNode*p, *q;
    p = idle.next;
    q = NULL;
    
    while (p)
    {
        if (p->data == access[n])
        {
            if (q != NULL)
            {
                q->next = p->next;
                p->next = NULL;
                idle.num--;
                break;
            }
            
            else
            {
                idle.next = NULL;
            }
        }
        
        q = p;
        p = p->next;
    }
    
    if (p == NULL)
    {
        p = modified.next;
        
        while (p != NULL)
        {
            if (p->data == access[n])
            {
                if (p == modified.next)
                { modified.next = p->next; }
                
                else
                {
                    q->next = p->next;
                    p->next = NULL;
                    modified.num--;
                }
                
                if (modified.num == 0)
                { modified.next = NULL; }
                
                break;
            }
            
            q = p;
            p = p->next;
        }
    }
    
    return p;
}


void PBA (int n)
{
    if (isInNodes (n))
    {
        printf ("已装入内存\n");
    }
    
    else
        if (index == size)
        {
            LNode *p;
            
            if ( (p = isinLinks (n)) != NULL)
            {
                nodes = (LNode*) realloc (nodes, (size + 1) * sizeof (LNode));
                nodes[size] .data = p->data;
                nodes[size].flag = p->flag;
                nodes[size].modify = p->modify;
                nodes[size].next = p->next;
                free (p);
                size++;
                index++;
            }
            
            else
            {
                lost++;//缺页
                
                if (nodes[n % 3].modify == 1)
                {
                    addToLink (nodes[n % 3].data, 1);
                }
                
                else
                {
                    addToLink (nodes[n % 3].data, 0);
                }
                
                nodes[n % 3].data = access[n];
                nodes[n % 3].flag = 1;
                nodes[n % 3].next = NULL;
                
                if (rand() % 10 < 4)
                {
                    nodes[n % 3].modify = 0;
                }
                
                else
                {
                    nodes[n % 3].modify = 1;
                }
            }
        }
        
        else
        {
            nodes[index].data = access[n];
            nodes[index].flag = 1;
            nodes[index].next = NULL;
            
            if (rand() % 10 < 4)
            {
                nodes[index].modify = 1;
            }
            
            else
            {
                nodes[index].modify = 0;
            }
            
            index++;
        }
}

void addToLink (int data, int type)
{
    LNode* p;
    LNode* q;
    q = (LNode*) malloc (sizeof (LNode));
    q->data = data;
    q->flag = 1;
    
    if (type == 1)
    {
        q->modify = 1;
        p = modified.next;
    }
    
    else
    {
        q->modify = 0;
        p = idle.next;
    }
    
    q->next = NULL;
    
    if (p == NULL)
    {
        if (type == 0)
        {
            idle.next = q;
        }
        
        else
        {
            modified.next = q;
        }
    }
    
    else
    {
        while (p)
        {
            if (p->next == NULL)
            {
                p->next = q;
                break;
            }
            
            else
            {
                p = p->next;
            }
        }
    }
    
    if (type == 0)
    {
        idle.num += 1;
        
        if (idle.num == 10)
        {
            emptyIdle();
        }
    }
    
    else
    {
        modified.num += 1;
        
        if (modified.num == 10)
        {
            emptyModi();
        }
    }
}
void emptyIdle ()
{
    LNode* p;
    p = idle.next;
    
    while (p)
    {
        idle.next = p->next;
        free (p);
        p = idle.next;
    }
    
    idle.num = 0;
}
void emptyModi()
{
    LNode* p;
    p = modified.next;
    
    while (p)
    {
        modified.next = p->next;
        free (p);
        p = modified.next;
    }
    
    modified.num = 0;
}

