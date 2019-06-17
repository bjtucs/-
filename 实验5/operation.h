#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//��?������2��?����oy
int show_openlist();				//��?ʾo?��?���㿪a��?��t����a,����|����?��?���㿪a��?��t��?��oy
void directory();					//��?ʾo?Ŀ?¼?��?��t�ꡧoϸ?��?Ϣ?��
void show_help();					//��?��?��tϵ|��ͳa3�Ĩ�?��??����2
void show_ldisk();					//��?ʾo?��??�̡�?�ڡ�2�ݡ�Y����?�츨?������2��??����?
//��?��?��?����oy
void read_block(int,char *);		//��?��tϵ|��ͳa3��??IO��|��������?�Ĩ�?�ӡ�?�ڡ�2��?����oy����?������ȡ��?�顧|
void write_block(int,char *);		//��?��tϵ|��ͳa3��??IO��|��������?�Ĩ�?�ӡ�?�ڡ�2��?����oy����?д??�롧?�顧|
void Init();						//��?ʼo?��?����?��tϵ|��ͳa3
int create(char *);					//��???��?����?��t
int destroy(char *);				//ɾ|?��y��?��t
int open(char *);					//��?���㿪a��?��t
int close(int);						//��?�ը�?��?��t
int read(int,int,int);				//��������?��t
int write(int,int,int);				//д??��?��t
int write_buffer(int,int);			//��??��o��?��?�ڡ�2�ݡ�Yд??�롧?��?��t
int lseek(int,int);					//��?��λ?��?��tָ?��?
void Init_block(char,int);			//��?ʼo?��?���֨�?����?��oy�騢��|�顧|����?�츨?������2����?

void read_block(int i,char *p)

{
	char * temp = (char *)malloc(sizeof(char));
	temp = p;
	for(int a = 0; a < B;)
	{
		*temp = ldisk[i][a];
		a++;
		temp++;
	}
}

void write_block(int i,char *p)
/**************************д??��??�̡�?�顧|
��?��?����oy��??ָ?��?p ָ?�򡧡�Ĩ�?�ڡ�2�ݡ�Yд??�롧?��?��-�顧|i
��?����??�Ĩ�?�֨�?����?��?��oyΪa��??��??���顧|�Ĩ�?��?���ȡ���B��?��
***************************/
{
	char * temp = (char *)malloc(sizeof(char));
	temp = p;
	for(int a = 0; a < B;)
	{
		ldisk[i][a] = *temp;
		a++;
		temp++;
	}
}

void Init_block(char *temp,int length)
/**************************��?ʼo?��?��һ��?��?�֨�?����?��oy�騢��|�顧|
��?|��衧a�Ĩ�?�֨�?����?��oy�騢��|�顧|��?���ȡ���ΪaB
�ڡ�2�ݡ�YΪa'\0'
***************************/
{
	int i;
	for(i = 0; i < length; i++)
	{
		temp[i] = '\0';
	}
}

int write_buffer(int index,int list)
{

	int i;
	int j;
	int freed;
	char temp[B];

	int buffer_length = BUFFER_LENGTH;
	for(i = 0; i < BUFFER_LENGTH ; i ++)
	{
		if(open_list[list].buffer[i] == '\0')
		{
			buffer_length = i;					//��o��?��?��?DЧ?����?���ȡ���
			break;
		}
	}
	
	int x = open_list[list].pointer[0];
	int y = open_list[list].pointer[1];
	int z = B-y;									//����?��ǰ??�顧|��?��D�ݡ�Y����?
//	printf("W:buffer_length: %d  x:%d y:%d z:%d\n",buffer_length,x,y,z);

	if( buffer_length < z )					//�顧|�ݡ�Y����?�ɡ�|д??�롧?��o��?��?��?�衧��Ҫ��a�١���֡�?��?
	{	
		read_block(x,temp);
		strncat(temp+y,open_list[list].buffer,buffer_length);			//��o��?��?�ӡ�?�롧?
		//printf("temp[0]: %c\n",temp[0]);
		write_block(x,temp);
		
		read_block(index+FILE_SIGN_AREA,temp);	//����1��?��?��t��?���ȡ���
		//printf("temp[1] = %d\n",temp[1]);
		temp[1] += buffer_length;	
		//printf("temp[1] = %d\n",temp[1]);
		write_block(index+FILE_SIGN_AREA,temp);
		
		open_list[list].pointer[0] = x;
		open_list[list].pointer[1] = y + buffer_length ;					//����1��?��?��t������д??ָ?��?
	}
	else									//��?��?��?��2����?�衧��Ҫ��a�֡�?��?��?�顧|
	{
		read_block(index+FILE_SIGN_AREA,temp);
		if(temp[2]+(buffer_length - z)/B+1 > FILE_BLOCK_LENGTH)
		{
			printf("��?��t�֡�?��?��oy�騢��|��?��?�֡�?��?\n");
			return ERROR;
		}

		//�֡�?��e����y��?�֡�?
		//�ڨ���2һ��?��?�֡�? ��??����2
		read_block(x,temp);
		strncat(temp+y,open_list[list].buffer,z);			//��o��?��?�ӡ�?�롧?z�Ĩ�?��?���ȡ�������?��??����2����?��ǰ??�顧|
		write_block(x,temp);

		//�ڨ���2��t��?�֡�? ��1�衧��Ҫ��a�֡�?��? (buffer_length - z)/B+1�顧| 

		//******************Ѱ??�ҡ�����?��t��?����?��Ŀ?¼?��?֮?��?����?�Ĩ�?��?��D�顧|����?�֡�?��?��?�顧|
		for(i = 0; i < (buffer_length - z)/B ; i ++ )
		{
			for(j = K + FILE_NUM; j < L ; j++)
			{
				read_block((j-K)/B,temp);
				if(temp[(j-K)%B] == FREE)
				{
					freed = j;
		//			printf("freed= : %d\n",freed);
					break;
				}
			}
			if(j == L)
			{
				printf("��??�̡�?�ѡ�?����2����?�֡�?��?ʧo?����?��1\n");
				return ERROR;
			}
			
			Init_block(temp,B);						
			strncpy(temp,(open_list[list].buffer+z+(i*B)),B);
			write_block(freed,temp);				//д??�롧?�ڡ�2�ݡ�Y

			read_block((freed-K)/B,temp);			//����1��?λ?ͼa?״��??̬??
			temp[(freed-K)%B] = BUSY;
			write_block((freed-K)/B,temp);			//д??�롧?��??�̡�?����?λ?ͼa?(��?��t�ڡ�2�ݡ�Y�������?Ӧ?|�Ĩ�?)

			read_block(index+FILE_SIGN_AREA,temp);
			temp[2] ++;								//��?�ӡ�?��?��t�֡�?��?��oy�騢��|��?���ȡ���
			temp[2+temp[2]] = freed;
			write_block(index+FILE_SIGN_AREA,temp);	
		}

		//(buffer_length - z)%B ��?����2һ��?�顧|�ڡ�2β2��?�Ĩ�?��?�֡�?
		for(j = K + FILE_NUM; j < L ; j++)
		{
			read_block((j-K)/B,temp);
			if(temp[(j-K)%B] == FREE)
			{
				freed = j;
				break;
			}
		}
		if(j == L)
		{
			printf("��??�̡�?�ѡ�?����2����?�֡�?��?ʧo?����?��1\n");
			return ERROR;
		}
		Init_block(temp,B);	
	//	printf("(buffer_length - z)%B = %d\n",(buffer_length - z)%B);
		strncpy(temp,(open_list[list].buffer+z+(i*B)),(buffer_length - z)%B);
		write_block(freed,temp);				//д??�롧?�ڡ�2�ݡ�Y

		read_block((freed-K)/B,temp);			//����1��?λ?ͼa?״��??̬??
		temp[(freed-K)%B] = BUSY;
		write_block((freed-K)/B,temp);			//д??�롧?��??�̡�?����?λ?ͼa?(��?��t�ڡ�2�ݡ�Y�������?Ӧ?|�Ĩ�?)

		read_block(index+FILE_SIGN_AREA,temp);
		temp[2] ++;								//��?�ӡ�?��?��t�֡�?��?��oy�騢��|��?���ȡ���
		temp[2+temp[2]] = freed;
		write_block(index+FILE_SIGN_AREA,temp);


		read_block(index+FILE_SIGN_AREA,temp);	
		temp[1] += buffer_length;				//����1��?��?��t��?���ȡ���
		write_block(index+FILE_SIGN_AREA,temp);

		open_list[list].pointer[0] = freed;
		open_list[list].pointer[1] = (buffer_length - z)%B ;
	}	
//	printf("X = %d, Y = %d\n",open_list[list].pointer[0],open_list[list].pointer[1]);
}
int lseek(int index,int pos)
/*
��??��?��t�Ĩ�?������д??ָ?��?�ơ�?��?������?pos ָ?��?��Ĩ�?λ?��?��?��pos
��o?һ��?��?��?��oy����?����aʾo?��?��?��?��t��aʼo?λ?��?�Ĩ�?ƫ?�ơ�?����?��?����?��t��?���㿪aʱo?������?������д??ָ?��?
�Ԩ�?��?����|������?Ϊa0��?��ÿ?��??������д??�١�������??֮?��?����?����1ָ?�򡧡��?��?����?�á�?�ʡ�o�Ĩ�?�֨�?�ڡ�2�Ĩ�?��?һ��?
��?λ?��?��?��lseek �ܡ�1��?�ڡ�2��?��?��D������д??�١�������??�Ĩ�?�顧|��?��?��?�䨤?������д??ָ?��?�ܡ�1λ?��?��?��
*/
{
	int i;
	int list = -1;
	char temp[B];
	int pos_i = pos / B;				//�ڡ�2��?��t��������?D�顧|��oy��D�Ĩ�?�ڨ���2X�顧|
	int pos_j = pos % B;				//�ڡ�2�ڨ���2X�顧|��D�Ĩ�?X��?λ?��?
	//***************�������Yindex�ҡ������aĿ?
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	
	if(list == -1)					//û?�ҡ��㵽��?
	{
		printf("û?�ҡ��㵽��?����?��ǰ??��??����y��?��?��t,�١�������??ʧo?����?��1...\n");
		return ERROR;
	}
	if(open_list[list].flag != BUSY)		//��o?�롧?�Ĩ�?index��?Ӧ?|��?��tû?����?��?���㿪a
	{
		printf("��o?�롧?�Ĩ�?��??����y��?��?D��?����?�١�������??ʧo?����?��1...\n");
		return ERROR;
	}
	//**************�������Y����aĿ?��D�Ĩ�?index��??��?��t�衧����o?����?�򡧡��ҡ��㵽��?��?��??�ڡ�2�ݡ�Y
	read_block(open_list[list].filesignnum + FILE_SIGN_AREA,temp);
	if(pos_i > temp[2]-1)											//��?��?��?��2��?��tʵo|���ʡ�o�Ĩ�?�顧|��oyʱo?��
	{
		printf("��㡧?��?��Խ?��?,��?��λ?ʧo?����?��1\n");
		return ERROR;
	}
											
	//****************��?��λ?ָ?��?

	open_list[list].pointer[0] = temp[3+pos_i];				//��?��?��t�Ĩ�?�ڨ���2X�顧|�Ĩ�?ʵo|���ʡ�o��??�̡�?�ب�?ַ?��
	open_list[list].pointer[1] = pos_j;

	
	return OK;
	//*****************************
	
}


void Init()
/**************************��?ʼo?��?����??�̡�?
��?��??�̡�?ȫ��?��?��?��?
Ȼ��?��?��???��?��0��?��?��t�衧����o?����?Ϊa������Ŀ?¼?�Ĩ�?��?��t�衧����o?����?
��?ʼo?��?��λ?ͼa?��?
***************************/
{
	
	int i;
	char temp[B];
	

	for(i = 0; i < L; i++)					
	{
		Init_block(temp,B);
		write_block(i,temp);			//��?ʼo?��?����??�̡�?
	}

	for(i = K; i < L; i ++)						//��?ʼo?��?������?������?��?��D�Ĩ�?λ?ͼa?
	{
		read_block((i-K)/B,temp);
		temp[(i-K)%B] = FREE;
		write_block((i-K)%B,temp);
	}

	//**************��?ʼo?��?��Ŀ?¼?�Ĩ�?��?��t�ꨤ��oʶo?����?
	filesign temp_cnt_sign;
	temp_cnt_sign.filesign_flag =1;
	temp_cnt_sign.file_length = 0;					//��?��t��?���ȡ�����?ʼo?Ϊa0					
	temp_cnt_sign.file_block = FILE_BLOCK_LENGTH;	//�֡�?��?FILE_BLOCK_LENGTH*B�Ĩ�?��?��?����?���ڡ�2��oy��Y��?ǰ??��?����?��??��?��2��??����??Ŀ?¼?��?
	 
	Init_block(temp,B);
	temp[0] = temp_cnt_sign.filesign_flag;
	temp[1] = temp_cnt_sign.file_length;
	temp[2] = temp_cnt_sign.file_block;

	for(i = 0; i < FILE_BLOCK_LENGTH ; i++ )
	{
		temp[i+3] = K+i;									//Ĭ?�ϡ�?��oy��Y��?�Ĩ�?�ڨ���2ǰ??FILE_BLOCK_LENGTH����?������Ŀ?¼?ռ?��??
	}
	write_block(FILE_SIGN_AREA,temp);

	//**************����1��?λ?ͼa?��?״��??̬??
	read_block(0,temp);
	for(i = 0; i < FILE_NUM ; i++ )
	{
		temp[i] = FREE;						//��??ʱo?��δ??��???��?����?��t
	}
	write_block(0,temp);
}


int create(char filename[])
/**************************�������Y��?��t��?��???��?����?��t��?��
.�ҡ���һ��?��?��D��?��t�衧����o?����?
.�ڡ�2��?��tĿ?¼?���?Ϊa��?��???��?��Ĩ�?��?��t�֡�?��?һ��?��?Ŀ?¼?��?����?����?��ɡ�|�ܡ�1�衧��Ҫ��aΪaĿ?¼?��?��t�֡�?��?��?�Ĩ�?��??�̡�?�顧|����?
.�ڡ�2�֡�?��?����?�Ĩ�?Ŀ?¼?��?���?��?¼?��?��t��?�ԡ�?��??�衧����o?����?�ਤ�����?
.����|����?״��??̬??��?Ϣ?��
***************************/
{
	int i;
	int frees;							//��?��D�Ĩ�?��?��t�衧����o?����?λ?��?	
	int	freed;							//��?��D��oy��Y��?λ?��?����?��Ŀ?¼?��?����?
	int freed2;								
	char temps[B];
	char tempc[B];
	char temp[B];
	//***************�顧|��??��?��t��?��o?���?��??�ڡ�2
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == BUSY)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)
			{
				printf("��?Ŀ?¼?�ѡ�?��-��??�ڡ�2��?��t��?Ϊa%s�Ĩ�?��?��t��?��\n",filename);
				return ERROR;
			}
		}
	}	
	//***************Ѱ??�ҡ��㱣��?������?��?��D��?��D�Ĩ�?��?��t�衧����o?����?
	for(i = FILE_SIGN_AREA ; i < K; i ++)
	{	
		read_block(i,temp);
		if(temp[0] == FREE)
		{
			frees = i;							
			break;
		}
	}
	if(i == K)
	{
		printf("û?��?D��?��D�Ĩ�?��?��t�衧����o?����?\n");
		return ERROR;
	}
	//****************Ѱ??�ҡ�����oy��Y��?Ŀ?¼?�衧����o?����?������ָ?��?��Ĩ�?��D��?��D�Ĩ�?��??��??���顧|
	for(i = K ; i < K+FILE_NUM ; i++)
	{	
		read_block((i-K)/B,temp);						
		if(temp[(i-K)%B] == FREE)
		{
			freed = i;
			break;
		}
	}
	if(i == K+FILE_NUM)
	{
		printf("��?��t��?��oy�ѡ�?��??��|?��T\n");
		return ERROR;
	}
	//******************Ѱ??�ҡ�����?��t��?����?��Ŀ?¼?��?֮?��?����?�Ĩ�?��?��D�顧|����?�֡�?��?��?��?��???��?��Ĩ�?��?��t
	for(i = K + FILE_NUM; i < L ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == FREE)
		{
			freed2 = i;
			break;
		}
	}
	if(i == L)
	{
		printf("��??�̡�?�ѡ�?����2����?�֡�?��?ʧo?����?��1\n");
		return ERROR;
	}


	//*****************��T�ʡ�o��?a��?��aʼo?�١�������??
	filesign temp_filesign;						//��???��?���١顧��ʱo?����?��t�衧����o?����?
	contents temp_contents;						//��???��?���١顧��ʱo?��Ŀ?¼?��?

	//**************��1��?����?��t�衧����o?����?
	temp_filesign.filesign_flag = 1;
	temp_filesign.file_length = 0;
	temp_filesign.file_block = 1;
	
	
	Init_block(temps,B);
	temps[0] = temp_filesign.filesign_flag;
	temps[1] = temp_filesign.file_length;
	temps[2] = temp_filesign.file_block;
	temps[3] = freed2;
	for(i = 4 ; i < FILE_BLOCK_LENGTH ; i++)
	{
		temps[i] = '\0';
	}
	write_block(frees,temps);				//д??�롧?��??�̡�?����?��?��t�衧����o?����?��?
	
	//*****************��1��?��Ŀ?¼?��?��?�롧?Ŀ?¼?��?��t�衧����o?����?������ָ?��?��Ĩ�?��oy�騢��|�顧|
	temp_contents.filesignnum = frees - FILE_SIGN_AREA;
	strncpy(temp_contents.filename,filename,FILE_NAME_LENGTH);
	
	Init_block(tempc,B);
	tempc[0] = temp_contents.filesignnum;   
	tempc[1] = '\0';
	strcat(tempc,temp_contents.filename);
	write_block(freed,tempc);				//д??�롧?��??�̡�?����?��oy��Y��?

	//*****************����1��?λ?ͼa?״��??̬??
	read_block((freed-K)/B,temp);			//����1��?λ?ͼa?״��??̬??(Ŀ?¼?��?�������?Ӧ?|�Ĩ�?)
	temp[(freed-K)%B] = BUSY;
	write_block((freed-K)/B,temp);			//д??�롧?��??�̡�?����?λ?ͼa?

	read_block((freed2-K)/B,temp);			//����1��?λ?ͼa?״��??̬??
	temp[(freed2-K)%B] = BUSY;
	write_block((freed2-K)/B,temp);			//д??�롧?��??�̡�?����?λ?ͼa?(��?��t�ڡ�2�ݡ�Y�������?Ӧ?|�Ĩ�?)

	//****************��?�ӡ�?Ŀ?¼?��?��t�衧����o?����?��D�Ĩ�?��?���ȡ�����?
	read_block(FILE_SIGN_AREA,temp);
	temp[1]++;
	write_block(FILE_SIGN_AREA,temp);	
	
	return OK;
	
	
}

int destroy(char * filename)
/**************************ɾ|?��yָ?��?����?��t��?��
.�ڡ�2Ŀ?¼?���?��?��??��?��?��t�Ĩ�?�衧����o?����?�ਤ�����?
.ɾ|?��y��?��?��t��?Ӧ?|�Ĩ�?Ŀ?¼?��?����?��?������1��?λ?ͼa?
.��o��a�š�?��?��t�衧����o?����?
.����|����?״��??̬??��?Ϣ?��
***************************/
{
	int i;
	int dtys;										//��?Ҫ��aɾ|?��y�Ĩ�?��?��t�Ĩ�?Ŀ?¼?��?�Ĩ�?��?��t�衧����o?����?λ?��?
	int dtyd;										//��?Ҫ��aɾ|?��y�Ĩ�?��?��t�Ĩ�?Ŀ?¼?��?λ?��?
	int use_block;									//��?��?��tʵo|���ʡ�oʹo1��??�Ĩ�?�顧|��oy
	int index;
	char temp[B];
	char tempd[B];


	//***************���㡧���Y��?��t��?Ѱ??�ҡ�����?��t�Ĩ�?Ŀ?¼?��?�͡�a��?��t�衧����o?����?
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == BUSY)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)
			{
				dtyd = i;								//�ҡ��㵽��?��?��tĿ?¼?��?λ?��?
				dtys = temp[0] + FILE_SIGN_AREA;		//�ҡ��㵽��?��?��tĿ?¼?��?��?Ӧ?|�Ĩ�?��?��t�衧����o?����?λ?��?(�򡧡��?Ϊatemp[0])
				index = temp[0];
				break;
			}
		}
	}
	if(i == K+FILE_NUM)
	{
		printf("û?��?D�ҡ��㵽��?��?��?��t\n");
		return ERROR;
	}
	
	//************�������Y��?��t�衧����o?����?��??����?��index����?�顧|��??��?��?��t��o?���?��?���㿪a
	int list = -1;
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	if(open_list[list].flag == BUSY && list != -1)
	{
		printf("��?��?��t�ѡ�?��-����?��?���㿪a,�衧��Ҫ��a��?�ը�?��?�ܡ�1ɾ|?��y\n");
		return ERROR;
	}


	//****************�ҡ��㵽��?λ?��?��?��aʼo?�١�������??
	
	
	//****************���㡧���??����1��?��?��t�顧|��oy�騢��|��Dָ?��?��Ĩ�?�顧|�Ĩ�?λ?ͼa?��?
	read_block(dtys,temp);
	use_block = temp[2];
	for(i = 0 ; i < use_block ; i++)
	{
		read_block((temp[i+3]-K)/B,tempd);				
		tempd[(temp[i+3]-K)%B] = FREE;
		write_block((temp[i+3]-K)/B,tempd);
	}
	//***************ɾ|?��y��?Ŀ?¼?��?
	Init_block(temp,B);
	write_block(dtys,temp);
	
	


	//***************ɾ|?��y��?��t�衧����o?����?
	Init_block(temp,B);
	write_block(dtyd,temp);

	//****************����1��?λ?ͼa?��?
	read_block((dtyd-K)/B,temp);
	temp[(dtyd-K)%B] = FREE;
	write_block((dtyd-K)/B,temp);
	//**************Ŀ?¼?��?��t�衧����o?����?��D�Ĩ�?��?���ȡ�����?1
	read_block(FILE_SIGN_AREA,temp);
	temp[1]--;
	write_block(FILE_SIGN_AREA,temp);

	
	return OK;

}
int open(char * filename)
/***************************��?���㿪a��?��t��?��
��?��?����oy����|����?�Ĩ�?��??����y��?�ɡ�|��??��?��2��?��?�Ĩ�?read, write, lseek, �򡧡�close �١�������??��?��
.��?��??Ŀ?¼?�ҡ��㵽��?��?��t��?Ӧ?|�Ĩ�?�衧����o?����?�򡧡��?
.�ڡ�2��?���㿪a��?��t����a��D�֡�?��?һ��?��?����aĿ?
.�ڡ�2�֡�?��?����?�Ĩ�?����aĿ?��D��??������д??ָ?��?��?Ϊa0����?��?����?¼?�衧����o?����?�ਤ�����?
.�������롧?��?��t�Ĩ�?�ڨ���2һ��?�顧|����?������д??��o��?��?��D
.����|����?�֡�?��?����?�Ĩ�?����aĿ?�ڡ�2��?���㿪a��?��t����a��D�Ĩ�?��??����y��?
***************************/
{
	int i;
	int opd;
	int ops;
	int list;
	char temp[B];
	int index;
	//***************���㡧���Y��?��t��?Ѱ??�ҡ�����?��t�Ĩ�?Ŀ?¼?��?�͡�a��?��t�衧����o?����?
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == BUSY)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)
			{
				opd = i;								//�ҡ��㵽��?��?��tĿ?¼?��?λ?��?
				ops = temp[0] ;							//�ҡ��㵽��?��?��tĿ?¼?��?��?Ӧ?|�Ĩ�?��?��t�衧����o?����?�򡧡��?
			//	printf("opd: %d,ops: %d\n",opd,ops);
				break;
			}
		}
	}
	if(i == K+FILE_NUM)
	{
		printf("û?��?D�ҡ��㵽��?��?��?��t\n");
		return ERROR;
	}

	//*************�顧|��??��?��?��t��o?���?����?��?���㿪a
	for(i = 0 ; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == ops && open_list[i].flag == BUSY)
		{
			printf("��?��?��t�ѡ�?��-����?��?���㿪a\n");
			return ERROR;
		}
	}

	//**************�ҡ���һ��?�顧|û?ʹo1��??�Ĩ�?����aĿ?�ԡ�?�㨤?�֡�?��?
	for(i = 0 ; i < FILE_NUM ; i++)
	{
		if(open_list[i].flag != BUSY)
		{	
			list = i;
			break;
		}
	}

	//******************��?����aĿ?��?��D�١�������??
	
	open_list[list].filesignnum = ops;								//д??�롧?��?��t�衧����o?����?�򡧡��?

	open_list[list].flag = BUSY;									//��?�ꨤ��o־?λ?Ϊaռ?��??
	
	index = open_list[list].filesignnum;							//��|��2�ɡ�|��??����y��?(�ࡧ�赱��?����?��2��?��?��tĿ?¼?��?�Ĩ�?��?��t�衧����o?����?�򡧡��?)		
	lseek(index,0);													//ָ?��?ָ?�򡧡���?��t��o?����?(��??ָ?��?ָ?Ϊa0)
	
	Init_block(open_list[list].buffer,BUFFER_LENGTH);				//��?ʼo?��?����o��?��?
	read_block(open_list[list].pointer[0],temp);					//��������?��t�Ĩ�?��o?���顧|	
	strncpy(open_list[list].buffer,temp,BUFFER_LENGTH);				//��??��?��t�Ĩ�?�ڨ���2һ��?�顧|д??�롧?��o��?��?
	
	return OK;
	
}

int close(int index) 
/***************************��?�ը�?��?��t��?��
.��??��o��?��?�Ĩ�?�ڡ�2�ݡ�Yд??�롧?��??�̡�?
.��o��a�š�?��?��?��t�١����?���㿪a��?��t����a��D��?Ӧ?|�Ĩ�?����aĿ?
.����|����?״��??̬??��?Ϣ?��
***************************/
{
	int i;
	int list = -1;
	char temp[B];
	//***************�������Yindex�ҡ������aĿ?
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	if(list == -1)					//û?�ҡ��㵽��?
	{
		printf("û?�ҡ��㵽��?����?��ǰ??��??����y��?��?��t,�١�������??ʧo?����?��1...\n");
		return ERROR;
	}
	if(open_list[list].flag != BUSY)		//��o?�롧?�Ĩ�?index��?Ӧ?|��?��tû?����?��?���㿪a
	{
		printf("��o?�롧?�Ĩ�?��??����y��?��?D��?����?�١�������??ʧo?����?��1...\n");
		return ERROR;
	}
	
	//****************��?��o��?��?�Ĩ�?�ڡ�2�ݡ�Yд??�롧?��??�̡�?
	/* //��a��o?��??
	for(i = 0 ; i < BUFFER_LENGTH-1; i++ )
	{
		open_list[list].buffer[i] = 5;
	}
	*/
	
	write_buffer(index,list);			//��?����?��ǰ??list��?���㿪a��?��t����a��?Ӧ?|�Ĩ�?��o��?��?�롧?index��??����y��?�Ĩ�?��?��t
	
	//****************��?��t�١�������?? ��o��a�š�?����aĿ?
	Init_block(open_list[list].buffer,BUFFER_LENGTH);				//��?��y��o��?��?
	open_list[list].filesignnum = FREE;								//��?��y��?��t�衧����o?����?
	open_list[list].flag = FREE;									//��?��yռ?��??�ꨤ��o־?λ?
	open_list[list].pointer[0] = NULL;								//��?��tָ?��?
	open_list[list].pointer[1] = NULL;
	return OK;
}
int read(int index, int mem_area, int count) 
/*
��?��?ָ?��?����?��t˳3�򡧡�������롧?count ��?�֨�?
�ڡ�2mem_area ָ?��?��Ĩ�?�ڡ�2��??λ?��?��?��������١�������??��?��?��?��t�Ĩ�?������д??ָ?��?ָ?ʾo?�Ĩ�?λ?��?
��aʼo?��?��
*/
{
	int i;
	int list = -1;
	char temp[B];
	//***************�������Yindex�ҡ����?���㿪a��?��t����a����aĿ?
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	if(list == -1)					//û?�ҡ��㵽��?
	{
		printf("û?�ҡ��㵽��?����?��ǰ??��??����y��?��?��t,�١�������??ʧo?����?��1...\n");
		return ERROR;
	}
	if(open_list[list].flag != BUSY)		//��o?�롧?�Ĩ�?index��?Ӧ?|��?��tû?����?��?���㿪a
	{
		printf("��o?�롧?�Ĩ�?��??����y��?��?D��?����?�١�������??ʧo?����?��1...\n");
		return ERROR;
	}

	//***************�������Yindex�ҡ�����?��t�衧����o?����?
	char temp_output[OUTPUT_LENGTH];
	Init_block(temp_output,OUTPUT_LENGTH);
	char output[OUTPUT_LENGTH];
	Init_block(output,OUTPUT_LENGTH);

	read_block(FILE_SIGN_AREA+index,temp);
	int file_length = temp[1];					//��?��t��?���ȡ���
	int file_block = temp[2];					//��?��tʵo|���ʡ�oʹo1��??�顧|
	int file_area;

	//**********************��?����??��?��t�ڡ�2�ݡ�Y������temp_output
	for(i = 0; i < file_block - 1 ; i++)
	{
		read_block(FILE_SIGN_AREA+index,temp);
		read_block(temp[3+i],temp);
		strncpy(temp_output+i*B,temp,B);			
	}
	read_block(FILE_SIGN_AREA+index,temp);
	read_block(temp[3+i],temp);
	strncpy(temp_output+i*B,temp,B);
	
	//******************����?��ǰ??��?��t������д??����?�ꨤ��o
	int x = open_list[list].pointer[0];
	int y = open_list[list].pointer[1];

	for(i = 0 ; i < file_block ; i++)
	{
		read_block(FILE_SIGN_AREA+index,temp);
		if(temp[3+i] == x)
		{
			break;
		}
	}
	file_area = i * B + y;							//ת��a��?Ϊa��?��t�ڡ�2�ࡧ���?λ?��?									
	
	for(i = 0 ;  i < count ; i++)
	{
		output[i+mem_area] = temp_output[i+file_area]; 
	}

	printf("%s\n",output+mem_area);
	return OK;
}
int write(int index,int mem_area,int count)
/*
��??mem_area ָ?��?��Ĩ�?�ڡ�2��??λ?��?��a
ʼo?�Ĩ�?count ��?�֨�?�ڡ�2˳3�򡧡�д??�롧?ָ?��?����?��t��?��д??�١�������??��?��?��?��t�Ĩ�?������д??ָ?��?ָ?ʾo?
�Ĩ�?λ?��?��aʼo?��?��
*/
{
	int i;
	int list = -1;
	int input_length;
	char temp[B];

	//*************�������Yindex�ҡ��㵽��?��?��t��?���㿪a����a
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	if(list == -1)					//û?�ҡ��㵽��?
	{
		printf("û?�ҡ��㵽��?����?��ǰ??��??����y��?��?��t,�١�������??ʧo?����?��1...\n");
		return ERROR;
	}
	if(open_list[list].flag != BUSY)		//��o?�롧?�Ĩ�?index��?Ӧ?|��?��tû?����?��?���㿪a
	{
		printf("��o?�롧?�Ĩ�?��??����y��?��?D��?����?�١�������??ʧo?����?��1...\n");
		return ERROR;
	}

	char input[INPUT_LENGTH];
	Init_block(input,INPUT_LENGTH);
	i = 0;
	fflush(stdin);
	while(scanf("%c",&input[i]))
	{
		if(input[i] == '\n')											//��?��|��ʱo?����?ֹ1�������롧?
		{
			input[i] == '\0';			
			break;
		}
		i++;
	}
	input_length = i;		
	//*******************��?��?��o?�롧?��??�Ĩ�?��?���ȡ��� �衧��Ҫ��aд??�롧?�Ĩ�?��?���ȡ���Ϊa input_length - mem_area
	//��o��?��?�ݡ�Y����?�ɡ�|д??�롧?��o?�롧?�ڡ�2�ݡ�Y��?�衧��Ҫ��a�١���֡�?��?
	if(count <= BUFFER_LENGTH )					
	{
		strncat(open_list[list].buffer,input+mem_area,count);			//��??�롧?��o��?��?
	}
	//��?��?��?��2��o��?��?��?���ȡ����֡�?��??д??�롧?����?�衧��Ҫ��a�֡�?�顧|д??�롧?
	else													
	{
		int rest;						//����?��ǰ??��o��?��?��?��D�ݡ�Y����?
		for(i = 0; i < BUFFER_LENGTH ; i++)
		{
			if(open_list[list].buffer[i] == FREE)
			{
				rest = BUFFER_LENGTH - i;
				break;
			}
		}		
		//�ڨ���2һ��?��?�֡�?����?��o��?��?��?Dһ��?��?���ݡ�Y����?rest ��?��o��?��?д??�롧?��?��t����?��?��?��o��?��?		
		strncat(open_list[list].buffer+BUFFER_LENGTH-rest,input + mem_area,rest);
		write_buffer(index,list);
		Init_block(open_list[list].buffer,BUFFER_LENGTH);
		//�ڨ���2��t��?�֡�?����?ѭ-��?�� (input_length - mem_area)/BUFFER_LENGTH �顧|��o��?��?д??�롧?��?��t
		for(i = 0; i < (count/BUFFER_LENGTH)-1 ; i++)
		{
			strncpy(open_list[list].buffer,(input+mem_area)+rest+i*BUFFER_LENGTH,BUFFER_LENGTH);
	//		printf("ѭ-��?��ʱo?������o�ڨ���2%d�顧|��o��?��?����obuffer: %s\n",i,open_list[list].buffer);
			write_buffer(index,list);
			Init_block(open_list[list].buffer,BUFFER_LENGTH);
		}
		//�ڨ���2����y��?�֡�?����?(count%BUFFER_LENGTH)ʣo?����?����δ??����2һ��?�顧|�Ĩ�?д??�롧?��o��?��?
		Init_block(open_list[list].buffer,BUFFER_LENGTH);
		strncpy(open_list[list].buffer,(input+mem_area)+rest+i*BUFFER_LENGTH,count%BUFFER_LENGTH);
		int buffer_start;
	//	printf("�ڨ���2%d�顧|��o��?��?����obuffer: %s\n",i,open_list[list].buffer);
	}
	return OK;
}

void directory()
//�С�D����a��?ʾo?��������?D��?��t��??��?��?���ȡ�����?��
{
	int i;
	int filenum;
	int filelength;
	char filename[FILE_NAME_LENGTH];
	char temp[B];
	char tempd[B];
	char temps[B];
	read_block(FILE_SIGN_AREA,temp);
	filenum = temp[1];						//ʵo|���ʡ�o��??�ڡ�2�Ĩ�?��?��t��?��oy
	printf("\n");
	if(filenum == 0 )
	{
		printf("\t\t\t\t��?Ŀ?¼?��?û?��?D��?��t\n");
	}
	
	for(i = 0; i < FILE_NUM; i++)
	{
		read_block(temp[3+i],tempd);					//������ȡ��?Ŀ?¼?��?
		if(tempd[0] != 0)
		{
			read_block(tempd[0]+FILE_SIGN_AREA,temps);		//������ȡ��?��?��t�衧����o?����?
			if(temps[0] == BUSY && tempd[0] != 0)
			{	
				filelength = temps[1];
				strcpy(filename,tempd+1);
				printf("%-10s\t\t%-2d�֨�?�ڡ�2\n",filename,filelength);
			}
		}
	}

	if(filenum != 0 )
	{
		printf("\t\t\t\t��2%d��?��?��t\n",filenum);
	}
}

int show_openlist()
{

	int i,j;
	int openfile = 0;
	char temp[B];
	int index;
	printf("\n��??����y��?\t\t��?��?С?\t\t��?��t��?\n");
	for(i = 0 ; i < FILE_NUM ; i ++)
	{
		if(open_list[i].flag == BUSY)
		{
			index = open_list[i].filesignnum;
			printf("  %-2d",index);
			openfile++;
			read_block(FILE_SIGN_AREA+index,temp);
			printf("\t\t %-2d",temp[1]);
			//�������Yindex(��?��t�衧����o?����?�򡧡��?)�ҡ��㵽��?��?Ŀ?¼?��?
			for(j = K; j < K+FILE_NUM ; j++)
			{
				read_block(j,temp);
				if(temp[0] == index)
				{
					printf("\t\t%-10s\n",temp+1);
				}
			}		
		}
	}
	return openfile;

}
void show_help()
{
	printf("**----------------------------------------------------------------------**\n");
	printf("**    ����1���?                   ˵|����??                                       **\n");
	printf("** dir                  ��?ʾo?Ŀ?¼?�ڡ�2�ݡ�Y                                    **\n");
	printf("**                                                                      **\n");
	printf("** create+filename      ��?��?���ԡ�?filenameΪa��?��t��?�Ĩ�?��?��t                    **\n");
	printf("**                                                                      **\n");
	printf("** delete+filename      ɾ|?��y�ԡ�?filenameΪa��?��t��?�Ĩ�?��?��t                    **\n");
	printf("**                                                                      **\n");
	printf("** open+filename        ��?���㿪a�ԡ�?filenameΪa��?��t��?�Ĩ�?��?��t                    **\n");
	printf("**                                                                      **\n");
	printf("** close                ��?�ը�?indexΪa��??����y��?�Ĩ�?��?��t                         **\n");
	printf("**                                                                      **\n");
	printf("** read                 ��?�������Y��?����ʾo?����?�ӡ�?�Ũ�?Ҫ��a��?��o?�롧?��??����y��?�ԡ�?��??������ȡ��?��?���ȡ�����?��D������**\n");                 
	printf("**                      ȡ��?��?��t�١�������??                                      **\n");
	printf("**                                                                      **\n");
	printf("** write                ��?�������Y��?����ʾo?����?�ӡ�?�Ũ�?Ҫ��a��?��o?�롧?��??����y��?�ԡ�?��??д??�롧?��?���ȡ�����?��Dд??**\n");  
	printf("**                      �롧?��?��t�١�������??                                      **\n");
	printf("**                                                                      **\n");
	printf("** lseek                ��?�������Y��?����ʾo?����?�ӡ�?�Ũ�?Ҫ��a��?��o?�롧?һ��?��?��?��?��?��?��2��?��t��?���ȡ����Ĩ�?��oy�֨�?**\n");
	printf("**                      ��??��?��2��?��λ?������д??ָ?��?                                **\n");
	printf("**                                                                      **\n");
	printf("** help                 ��??����2                                            **\n");
	printf("**                                                                      **\n");
	printf("** exit                 ��a?��?��?��tϵ|��ͳa3                                    **\n");
	printf("**----------------------------------------------------------------------**\n");
}



void show_ldisk()
{
	int a,b;
	for(a = 0; a < K+30 ;a++)
	{
		printf("%-3d :",a);
		for(b = 0; b< B ;b++)
		{
			printf("%-3d ",ldisk[a][b]);
		}
		printf("\n");
	}
}

