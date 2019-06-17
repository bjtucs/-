#include <stdio.h>
#include <stdlib.h>
#include <string>
//#include <iostream.h>
using namespace std;

#include "Define.h"
#include "Operation.h"

#define DIR		0
#define	CREATE	1
#define	DELETE	2
#define	OPEN	3
#define	CLOSE	4
#define	READ	5	
#define	WRITE	6
#define HELP	7
#define LSEEK	8
#define EXIT	9
#define	OPLIST	10
#define LDISK	11
#define CH_LENGTH	20


int main()
{

	printf("欢迎使用该文件系统\n");
	show_help();
	Init();
	create("lwmorz1");
	create("lwmorz2");
	create("lwmorz3");
	open("lwmorz1");
	open("lwmorz3");

	char ch[CH_LENGTH];
	Init_block(ch,CH_LENGTH);
	while(gets(ch))
	{
	
		int cmd;
		char filename[FILE_NAME_LENGTH];
		
		cmd = -1;
		Init_block(filename,FILE_NAME_LENGTH);
		if(strncmp("dir",ch,3) == 0)		
		{
			cmd = DIR;	
		}
		if(strncmp("create",ch,6) == 0)		
		{
			cmd = CREATE;
			strcat(filename,ch+7);
		}
		if(strncmp("delete",ch,6) == 0)		
		{
			cmd = DELETE;
			strcat(filename,ch+7);
		}
		if(strncmp("open",ch,4) == 0)		
		{
			cmd = OPEN;
			strcat(filename,ch+5);
		}
		if(strncmp("close",ch,5) == 0)			
		{
			cmd = CLOSE;
		}
		if(strncmp("read",ch,4) == 0)		
		{
			cmd = READ;
		}
		if(strncmp("write",ch,5) == 0)		
		{
			cmd = WRITE;
		}
		if(strncmp("lseek",ch,5) == 0)		
		{
			cmd = LSEEK;
		}
		if(strncmp("oplist",ch,6) == 0)		
		{
			cmd = OPLIST;
		}
		if(strncmp("exit",ch,4) == 0)		
		{
			cmd = EXIT;
			break;
		}
		if(strncmp("ldisk",ch,5) == 0)		
		{
			cmd = LDISK;
		}
		if(strncmp("help",ch,4) == 0)			
		{
			cmd = HELP;
		}
		int index,count,pos;
		switch(cmd)
		{
		case DIR:
			directory();
			printf("----------------------------------------------\n");
			break;
		case CREATE:
			if(create(filename) == OK)
				printf("创建文件成功\n");
			printf("----------------------------------------------\n");
			break;
		case DELETE:
			if(destroy(filename) == OK)
				printf("删除文件成功\n");
			printf("----------------------------------------------\n");
			break;
		case OPEN:
			if(open(filename) == OK)
				printf("打开文件成功\n");
			printf("----------------------------------------------\n");
			break;
		case CLOSE:
			if(show_openlist()==0)
			{
				printf("当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("请输入要关闭的文件索引号\n");
			scanf("%d",&index);
			if(close(index) == OK)
				printf("关闭操作成功\n");
			printf("----------------------------------------------\n");
			break;
		case READ:
			if(show_openlist()==0)
			{
				printf("当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("请输入要读取的文件索引号\n");
			scanf("%d",&index);
			printf("请输入要读取的文件长度\n");
			scanf("%d",&count);
			if(read(index,0,count) == OK)
				printf("读文件操作成功\n");
			printf("----------------------------------------------\n");
			break;
		case WRITE:
			if(show_openlist()==0)
			{
				printf("当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("请输入要写入文件的索引号\n");
			scanf("%d",&index);
			printf("请输入想要写入的文件长度\n");
			scanf("%d",&count);
			if(write(index,0,count) == OK)
				printf("写入操作成功\n");
			printf("----------------------------------------------\n");
			break;
		case LSEEK:
			if(show_openlist()==0)
			{
				printf("当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("请输入要写入文件的索引号\n");
			scanf("%d",&index);
			printf("请输入想要设置的文件相对位置\n");
			scanf("%d",&pos);
			lseek(index,pos);
			printf("----------------------------------------------\n");
			break;
		case OPLIST:
			if(show_openlist()==0)
			{
				printf("当前没有文件被打开\n");
				printf("----------------------------------------------\n");
				break;
			}
			printf("----------------------------------------------\n");
			break;
		case HELP:
			show_help();
			break;
		case LDISK:
			show_ldisk();
			break;
		default:
			printf("指令错误\n");
			printf("----------------------------------------------\n");
			break;
		}
		fflush(stdin);
		Init_block(ch,CH_LENGTH);
	}

}
