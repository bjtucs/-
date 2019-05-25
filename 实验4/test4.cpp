//#include "stdafx.h"
#include "stdio.h"
#include"stdlib.h"
#include"time.h"
#define R 32    //物理内存块数
#define V 64   //虚拟内存块数

struct LNode
{
    int data;
    int flag;//访问位
    int modify;//修改位
};

void initMemo();
void generate();//生成访问序列
bool  isInMemo (int n); //
void optimal (int n); //
void testOptimal();
void LRU (int n);
void testLRU();
void updated_Clock (int n);
void test_Clock();


int block = 3;
int access[32]; //访问序列
int* memo;
int lost = 0;//没找到的页面数
int index = 0;//指示当前下标
LNode* nodes;//改进型Clock置换算法用到的数据结构


int main (int argc, char* argv[])
{
    generate();
    testOptimal();
    testLRU();
    test_Clock();
    int i = 0;
    
    for (; i < 32; i++)
    {
        printf ("%d, ", access[i]);
    }
    
    getchar();
    getchar();
    return 0;
}

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

void initMemo()
{
    memo = (int*) malloc (block * sizeof (int));
    int i = 0;
    
    for (; i < block; i++)
    {
        memo[i] = -1;
    }
    
    return;
}

void testOptimal()
{
    initMemo();
    int i = 0;
    printf ("最佳置换算法：\n");
    
    for (; i < 32; i++)
    {
        optimal (i);
        printf ("%d %d %d\n", memo[0], memo[1], memo[2]);
    }
    
    printf ("最佳置换算法缺页率： %2f %d\n", lost / 32.0, lost);
    lost = 0;
    free (memo);
    index = 0;
}

bool  isInMemo (int n)
{
    int i = 0;
    
    for (; i < block; i++)
    {
        if (access[n] == memo[i])
        {
            return true;
        }
    }
    
    return false;
}

//最佳适应算法
void optimal (int n)
{
    int i = 0, j = 0;
    
    if (isInMemo (n))
    {
        printf ("页面已被调入\n");
    }
    
    else
        if (index == block)
        {
            lost++;
            int max = 0, pos, tag;
            
            for (i = 0; i < block; i++)
            {
                tag = -1;
                
                for (j = n + 1; j < 32; j++)
                {
                    if (access[j] == memo[i])
                    {
                        tag = j;
                        break;
                    }
                }
                
                if (tag == -1)
                {
                    max = 32;
                    pos = i;
                    break;
                }
                
                else
                {
                    if (max < tag)
                    {
                        max = tag;
                        pos = i;
                    }
                }
            }
            
            memo[pos] = access[n];
        }
        
        else
        {
            memo[index] = access[n];
            index++;
        }
}


////////LRU算法
void LRU (int n)
{
    int i, j;
    
    if (isInMemo (n))
    {
        printf ("已经装入内存\n");
    }
    
    else
        if (index == block)
        {
            int max = n, pos = -1, tag;
            
            for (i = 0; i < block; i++)
            {
                for (j = n - 1; j >= 0; j--)
                {
                    if (access[j] == memo[i])
                    {
                        tag = j;
                        break;
                    }
                }
                
                if (tag < max)
                {
                    max = tag;
                    pos = i;
                    
                    if (max == 0)
                    {
                        break;
                    }
                }
            }
            
            memo[pos] = access[n];
            lost++;
        }
        
        else
        {
            memo[index] = access[n];
            index++;
        }
}

void testLRU()
{
    int i;
    initMemo();
    printf ("最近最久未使用算法\n");
    
    for (i = 0; i < 32; i++)
    {
        LRU (i);
        printf ("%d %d %d\n", memo[0], memo[1], memo[2]);
    }
    
    printf ("最近最久未使用缺页率： %2f %d \n", lost / 32.0, lost);
    lost = 0;
    index = 0;
    free (memo);
}

bool isInNodes (int n)
{
    int i;
    
    for (i = 0; i < block; i++)
    {
        if (nodes[i].data == access[n])
        {
            return true;
        }
    }
    
    return false;
}

void updated_Clock (int n)
{
    if (isInNodes (n))
    {
        printf ("已经装入内存\n");
    }
    
    else
        if (index == block)
        {
            lost++;
            int i = 0, tag = -1;
            
            while (true)
            {
                if ( (i / block) % 2 == 0)
                {
                    if (nodes[i % block].flag == 0 && nodes[i % block].modify == 0)
                    {
                        tag = i % block;
                        break;
                    }
                }
                
                if ( (i / block) % 2 == 1)
                {
                    if (nodes[i % block].flag == 0 && nodes[i % block].modify == 1)
                    {
                        tag = i % block;
                        break;
                    }
                    
                    else
                    {
                        nodes[i % block].flag = 0;
                    }
                }
                
                i++;
            }
            
            nodes[tag].data = access[n];
            nodes[tag].flag = 1;
            
            if (rand() % 10 < 4)
            {
                nodes[tag].modify = 1;
            }
            
            else
            {
                nodes[tag].modify = 0;
            }
        }
        
        else
        {
            nodes[index].data = access[n];
            nodes[index].flag = 1;
            
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
void test_Clock()
{
    int i = 0, j = 0;
    printf ("改进型Clock置换算法\n");
    nodes = (LNode*) malloc (block * sizeof (LNode));
    
    for (i = 0; i < block; i++)
    {
        nodes[i].data = -1;
        nodes[i].flag = -1;
        nodes[i].modify = -1;
    }
    
    for (i = 0; i < 32; i++)
    {
        updated_Clock (i);
        
        for (j = 0; j < block; j++)
        {
            printf ("%d ", nodes[j].data);
        }
        
        printf ("\n");
    }
    
    printf ("改进型Clock置换算法缺页率： %2f %d \n", lost / 32.0, lost);
    lost = 0;
    index = 0;
}
