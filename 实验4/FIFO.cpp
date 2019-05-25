//#include "stdafx.h"
#include"stdio.h"
#include"stdlib.h"
#include"time.h"
#define M 32    //物理内存块数
#define N 64   //虚拟内存块数
typedef struct node
{
    int num;
    node* next;
} Node, *pNode;

typedef struct queue
{
    int n;
    pNode front;
    pNode rear;
    
} Queue, *pQueue;


void initQueue (pQueue q);
void push (pQueue q, int num);
void pop (pQueue q);
void destroy (pQueue q);
bool findInQueue (pQueue q, int num);
void generate();
void fifoTest();
void fifo (pQueue q, int num);

int access[32];//访问序列
int size = 3;//给进程分配的内存的大小
int lost = 0;//缺页数

int main (int argc, char* argv[])
{
    //generate();
    fifoTest();
    getchar();
    getchar();
    return 0;
}

void initQueue (pQueue q)
{
    q->rear = (pNode) malloc (sizeof (Node));
    
    if (q->rear == NULL)
    {
        printf ("failed\n");
    }
    
    else
    {
        q->front = q->rear;
        q->rear->next = NULL;
        q->front->next = NULL;
        q->n = 0;
    }
}

void push (pQueue q, int num)
{
    pNode p = (pNode) malloc (sizeof (Node));
    
    if (p == NULL)
    {
        printf ("failed");
    }
    
    else
    {
        p->next = NULL;
        p->num = num;
        
        if (q->front == q->rear)
        {
            q->front->next = p;
            q->rear = p;
        }
        
        else
        {
            q->rear->next = p;
            q->rear = p;
        }
        
        q->n++;
    }
}

void pop (pQueue q)
{
    pNode p;
    
    if (q->front != q->rear)
    {
        p = q->front->next;
        q->front->next = p->next;
        
        if (p == q->rear)
        {
            q->front = q->rear;
        }
        
        q->n--;
        free (p);
    }
}

void destroy (pQueue q)
{
    while (q->front != q->rear)
    {
        pop (q);
    }
}


bool findInQueue (pQueue q, int num)
{
    pNode p;
    
    if (q->front != q->rear)
    {
        p = q->front->next;
        
        while (p)
        {
            if (p->num == num)
            {
                return true;
            }
            
            else
            {
                p = p->next;
            }
        }
    }
    
    return false;
}



/*
生成具有局部访问特点的访问序列
*/
void generate()
{
    srand ( (unsigned) time (NULL)); //用时间做种，每次产生随机数不一样
    int p = rand() % 64;
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


void fifoTest()
{
    Queue q;
    pNode p;
    initQueue (&q);
    int i = 0;
    printf ("先进先出置换算法\n");
    
    for (; i < 32; i++)
    {
        fifo (&q, access[i]);
        p = q.front->next;
        
        while (p)
        {
            printf ("%d ", p->num);
            p = p->next;
        }
        
        printf ("\n");
    }
    
    printf ("先进先出算法缺页率：%f  %d\n", lost / 32.0, lost);
    destroy (&q);
}

void fifo (pQueue q, int num)
{
    if (findInQueue (q, num))
    {
        printf ("已装入内存\n");
    }
    
    else
    {
        if (q->n == size)
        {
            pop (q);
            push (q, num);
            lost++;
        }
        
        else
        {
            push (q, num);
        }
    }
}
