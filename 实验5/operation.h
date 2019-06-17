#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//辅?§助¨2函?￥数oy
int show_openlist();				//显?示o?打?¨°开a文?件t表à¨a,返¤|ì回?打?¨°开a文?件t个?数oy
void directory();					//显?示o?目?录?文?件t详¨o细?信?息?é
void show_help();					//该?文?件t系|ì统a3的ì?帮??助¨2
void show_ldisk();					//显?示o?磁??盘¨?内¨2容¨Y（ê?§辅?§助¨2用??）ê?
//核?心?函?￥数oy
void read_block(int,char *);		//文?件t系|ì统a3与??IO设|¨¨备à?的ì?接¨?口¨2函?￥数oy，ê?读¨￠取¨?块¨|
void write_block(int,char *);		//文?件t系|ì统a3与??IO设|¨¨备à?的ì?接¨?口¨2函?￥数oy，ê?写??入¨?块¨|
void Init();						//初?始o?化?￥文?件t系|ì统a3
int create(char *);					//创???建?§文?件t
int destroy(char *);				//删|?除y文?件t
int open(char *);					//打?¨°开a文?件t
int close(int);						//关?闭à?文?件t
int read(int,int,int);				//读¨￠文?件t
int write(int,int,int);				//写??文?件t
int write_buffer(int,int);			//把??缓o冲?区?内¨2容¨Y写??入¨?文?件t
int lseek(int,int);					//定?§位?文?件t指?针?
void Init_block(char,int);			//初?始o?化?￥字á?符¤?数oy组á¨|块¨|（ê?§辅?§助¨2）ê?

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
/**************************写??磁??盘¨?块¨|
该?函?￥数oy把??指?针?p 指?向¨°的ì?内¨2容¨Y写??入¨?逻?辑-块¨|i
拷?贝à??的ì?字á?符¤?个?数oy为a存??储??é块¨|的ì?长?è度¨¨B。?ê
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
/**************************初?始o?化?￥一°?个?字á?符¤?数oy组á¨|块¨|
处?|理¤¨a的ì?字á?符¤?数oy组á¨|块¨|长?è度¨¨为aB
内¨2容¨Y为a'\0'
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
			buffer_length = i;					//缓o冲?区?有?D效?ì长?è度¨¨
			break;
		}
	}
	
	int x = open_list[list].pointer[0];
	int y = open_list[list].pointer[1];
	int z = B-y;									//当ì?à前??块¨|空?闲D容¨Y量￠?
//	printf("W:buffer_length: %d  x:%d y:%d z:%d\n",buffer_length,x,y,z);

	if( buffer_length < z )					//块¨|容¨Y量￠?可¨|写??入¨?缓o冲?区?不?需¨¨要°a再¨′分¤?配?
	{	
		read_block(x,temp);
		strncat(temp+y,open_list[list].buffer,buffer_length);			//缓o冲?区?接¨?入¨?
		//printf("temp[0]: %c\n",temp[0]);
		write_block(x,temp);
		
		read_block(index+FILE_SIGN_AREA,temp);	//更¨1改?文?件t长?è度¨¨
		//printf("temp[1] = %d\n",temp[1]);
		temp[1] += buffer_length;	
		//printf("temp[1] = %d\n",temp[1]);
		write_block(index+FILE_SIGN_AREA,temp);
		
		open_list[list].pointer[0] = x;
		open_list[list].pointer[1] = y + buffer_length ;					//更¨1新?文?件t读¨￠写??指?针?
	}
	else									//大?¨?于?¨2，ê?需¨¨要°a分¤?配?新?块¨|
	{
		read_block(index+FILE_SIGN_AREA,temp);
		if(temp[2]+(buffer_length - z)/B+1 > FILE_BLOCK_LENGTH)
		{
			printf("文?件t分¤?配?数oy组á¨|不?够?分¤?配?\n");
			return ERROR;
		}

		//分¤?别àe三¨y部?分¤?
		//第ì¨2一°?部?分¤? 填??满¨2
		read_block(x,temp);
		strncat(temp+y,open_list[list].buffer,z);			//缓o冲?区?接¨?入¨?z的ì?长?è度¨¨，ê?填??满¨2当ì?à前??块¨|
		write_block(x,temp);

		//第ì¨2二t部?分¤? 还1需¨¨要°a分¤?配? (buffer_length - z)/B+1块¨| 

		//******************寻??找¨°文?件t区?（ê?§目?录?项?之?后¨?）ê?的ì?空?闲D块¨|，ê?分¤?配?新?块¨|
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
				printf("磁??盘¨?已°?满¨2，ê?分¤?配?失o?ì败?¨1\n");
				return ERROR;
			}
			
			Init_block(temp,B);						
			strncpy(temp,(open_list[list].buffer+z+(i*B)),B);
			write_block(freed,temp);				//写??入¨?内¨2容¨Y

			read_block((freed-K)/B,temp);			//更¨1改?位?图a?状á??态??
			temp[(freed-K)%B] = BUSY;
			write_block((freed-K)/B,temp);			//写??入¨?磁??盘¨?，ê?位?图a?(文?件t内¨2容¨Y所¨′对?应?|的ì?)

			read_block(index+FILE_SIGN_AREA,temp);
			temp[2] ++;								//增?加¨?文?件t分¤?配?数oy组á¨|长?è度¨¨
			temp[2+temp[2]] = freed;
			write_block(index+FILE_SIGN_AREA,temp);	
		}

		//(buffer_length - z)%B 不?满¨2一°?块¨|在¨2尾2部?的ì?部?分¤?
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
			printf("磁??盘¨?已°?满¨2，ê?分¤?配?失o?ì败?¨1\n");
			return ERROR;
		}
		Init_block(temp,B);	
	//	printf("(buffer_length - z)%B = %d\n",(buffer_length - z)%B);
		strncpy(temp,(open_list[list].buffer+z+(i*B)),(buffer_length - z)%B);
		write_block(freed,temp);				//写??入¨?内¨2容¨Y

		read_block((freed-K)/B,temp);			//更¨1改?位?图a?状á??态??
		temp[(freed-K)%B] = BUSY;
		write_block((freed-K)/B,temp);			//写??入¨?磁??盘¨?，ê?位?图a?(文?件t内¨2容¨Y所¨′对?应?|的ì?)

		read_block(index+FILE_SIGN_AREA,temp);
		temp[2] ++;								//增?加¨?文?件t分¤?配?数oy组á¨|长?è度¨¨
		temp[2+temp[2]] = freed;
		write_block(index+FILE_SIGN_AREA,temp);


		read_block(index+FILE_SIGN_AREA,temp);	
		temp[1] += buffer_length;				//更¨1改?文?件t长?è度¨¨
		write_block(index+FILE_SIGN_AREA,temp);

		open_list[list].pointer[0] = freed;
		open_list[list].pointer[1] = (buffer_length - z)%B ;
	}	
//	printf("X = %d, Y = %d\n",open_list[list].pointer[0],open_list[list].pointer[1]);
}
int lseek(int index,int pos)
/*
把??文?件t的ì?读¨￠写??指?针?移°?动?￥到ì?pos 指?定?§的ì?位?置?。?êpos
是o?一°?个?整?数oy，ê?表à¨a示o?从?¨?文?件t开a始o?位?置?的ì?偏?移°?量￠?。?ê文?件t打?¨°开a时o?à，ê?读¨￠写??指?针?
自á?动?￥设|¨¨置?为a0。?ê每?次??读¨￠写??操¨′作á??之?后¨?，ê?它¨1指?向¨°最á?后¨?被à?访¤?问¨o的ì?字á?节¨2的ì?下?一°?
个?位?置?。?êlseek 能¨1够?在¨2不?进?行D读¨￠写??操¨′作á??的ì?情¨|况?下?改?变à?读¨￠写??指?针?能¨1位?置?。?ê
*/
{
	int i;
	int list = -1;
	char temp[B];
	int pos_i = pos / B;				//在¨2文?件t所¨′有?D块¨|数oy中D的ì?第ì¨2X块¨|
	int pos_j = pos % B;				//在¨2第ì¨2X块¨|中D的ì?X个?位?置?
	//***************根¨′据Yindex找¨°表à¨a目?
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	
	if(list == -1)					//没?找¨°到ì?
	{
		printf("没?找¨°到ì?当ì?à前??索??引°y号?文?件t,操¨′作á??失o?ì败?¨1...\n");
		return ERROR;
	}
	if(open_list[list].flag != BUSY)		//输o?入¨?的ì?index对?应?|文?件t没?被à?打?¨°开a
	{
		printf("输o?入¨?的ì?索??引°y号?有?D误¨?，ê?操¨′作á??失o?ì败?¨1...\n");
		return ERROR;
	}
	//**************根¨′据Y表à¨a目?中D的ì?index即??文?件t描¨¨述o?符¤?序¨°找¨°到ì?具?体??内¨2容¨Y
	read_block(open_list[list].filesignnum + FILE_SIGN_AREA,temp);
	if(pos_i > temp[2]-1)											//大?¨?于?¨2文?件t实o|ì际¨o的ì?块¨|数oy时o?à
	{
		printf("异°¨?常?ê越?界?,定?§位?失o?ì败?¨1\n");
		return ERROR;
	}
											
	//****************定?§位?指?针?

	open_list[list].pointer[0] = temp[3+pos_i];				//该?文?件t的ì?第ì¨2X块¨|的ì?实o|ì际¨o磁??盘¨?地ì?址?¤
	open_list[list].pointer[1] = pos_j;

	
	return OK;
	//*****************************
	
}


void Init()
/**************************初?始o?化?￥磁??盘¨?
将?磁??盘¨?全¨?部?置?空?
然¨?后¨?创???建?§0号?文?件t描¨¨述o?符¤?为a根¨′目?录?的ì?文?件t描¨¨述o?符¤?
初?始o?化?￥位?图a?区?
***************************/
{
	
	int i;
	char temp[B];
	

	for(i = 0; i < L; i++)					
	{
		Init_block(temp,B);
		write_block(i,temp);			//初?始o?化?￥磁??盘¨?
	}

	for(i = K; i < L; i ++)						//初?始o?化?￥保à?ê留￠?区?中D的ì?位?图a?
	{
		read_block((i-K)/B,temp);
		temp[(i-K)%B] = FREE;
		write_block((i-K)%B,temp);
	}

	//**************初?始o?化?￥目?录?的ì?文?件t标à¨o识o?符¤?
	filesign temp_cnt_sign;
	temp_cnt_sign.filesign_flag =1;
	temp_cnt_sign.file_length = 0;					//文?件t长?è度¨¨初?始o?为a0					
	temp_cnt_sign.file_block = FILE_BLOCK_LENGTH;	//分¤?配?FILE_BLOCK_LENGTH*B的ì?空?间?（ê?§在¨2数oy据Y区?前??段?）ê?用??于?¨2储??é存??目?录?项?
	 
	Init_block(temp,B);
	temp[0] = temp_cnt_sign.filesign_flag;
	temp[1] = temp_cnt_sign.file_length;
	temp[2] = temp_cnt_sign.file_block;

	for(i = 0; i < FILE_BLOCK_LENGTH ; i++ )
	{
		temp[i+3] = K+i;									//默?认¨?数oy据Y区?的ì?第ì¨2前??FILE_BLOCK_LENGTH被à?根¨′目?录?占?用??
	}
	write_block(FILE_SIGN_AREA,temp);

	//**************更¨1改?位?图a?区?状á??态??
	read_block(0,temp);
	for(i = 0; i < FILE_NUM ; i++ )
	{
		temp[i] = FREE;						//此??时o?à未??创???建?§文?件t
	}
	write_block(0,temp);
}


int create(char filename[])
/**************************根¨′据Y文?件t名?创???建?§文?件t。?ê
.找¨°一°?空?闲D文?件t描¨¨述o?符¤?
.在¨2文?件t目?录?里¤?为a新?创???建?§的ì?文?件t分¤?配?一°?个?目?录?项?，ê?（ê?§可¨|能¨1需¨¨要°a为a目?录?文?件t分¤?配?新?的ì?磁??盘¨?块¨|）ê?
.在¨2分¤?配?到ì?的ì?目?录?项?里¤?记?录?文?件t名?以°?及??描¨¨述o?符¤?编à¨¤号?
.返¤|ì回?状á??态??信?息?é
***************************/
{
	int i;
	int frees;							//空?闲D的ì?文?件t描¨¨述o?符¤?位?置?	
	int	freed;							//空?闲D数oy据Y区?位?置?（ê?§目?录?项?）ê?
	int freed2;								
	char temps[B];
	char tempc[B];
	char temp[B];
	//***************查¨|看??文?件t名?是o?否¤?存??在¨2
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == BUSY)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)
			{
				printf("该?目?录?已°?经-存??在¨2文?件t名?为a%s的ì?文?件t。?ê\n",filename);
				return ERROR;
			}
		}
	}	
	//***************寻??找¨°保à?ê留￠?区?中D空?闲D的ì?文?件t描¨¨述o?符¤?
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
		printf("没?有?D空?闲D的ì?文?件t描¨¨述o?符¤?\n");
		return ERROR;
	}
	//****************寻??找¨°数oy据Y区?目?录?描¨¨述o?符¤?所¨′指?定?§的ì?中D空?闲D的ì?存??储??é块¨|
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
		printf("文?件t个?数oy已°?达??上|?限T\n");
		return ERROR;
	}
	//******************寻??找¨°文?件t区?（ê?§目?录?项?之?后¨?）ê?的ì?空?闲D块¨|，ê?分¤?配?给?新?创???建?§的ì?文?件t
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
		printf("磁??盘¨?已°?满¨2，ê?分¤?配?失o?ì败?¨1\n");
		return ERROR;
	}


	//*****************无T问¨o题?a后¨?开a始o?操¨′作á??
	filesign temp_filesign;						//创???建?§临￠¨′时o?à文?件t描¨¨述o?符¤?
	contents temp_contents;						//创???建?§临￠¨′时o?à目?录?项?

	//**************构1建?§文?件t描¨¨述o?符¤?
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
	write_block(frees,temps);				//写??入¨?磁??盘¨?，ê?文?件t描¨¨述o?符¤?区?
	
	//*****************构1建?§目?录?项?插?入¨?目?录?文?件t描¨¨述o?符¤?所¨′指?定?§的ì?数oy组á¨|块¨|
	temp_contents.filesignnum = frees - FILE_SIGN_AREA;
	strncpy(temp_contents.filename,filename,FILE_NAME_LENGTH);
	
	Init_block(tempc,B);
	tempc[0] = temp_contents.filesignnum;   
	tempc[1] = '\0';
	strcat(tempc,temp_contents.filename);
	write_block(freed,tempc);				//写??入¨?磁??盘¨?，ê?数oy据Y区?

	//*****************更¨1改?位?图a?状á??态??
	read_block((freed-K)/B,temp);			//更¨1改?位?图a?状á??态??(目?录?项?所¨′对?应?|的ì?)
	temp[(freed-K)%B] = BUSY;
	write_block((freed-K)/B,temp);			//写??入¨?磁??盘¨?，ê?位?图a?

	read_block((freed2-K)/B,temp);			//更¨1改?位?图a?状á??态??
	temp[(freed2-K)%B] = BUSY;
	write_block((freed2-K)/B,temp);			//写??入¨?磁??盘¨?，ê?位?图a?(文?件t内¨2容¨Y所¨′对?应?|的ì?)

	//****************增?加¨?目?录?文?件t描¨¨述o?符¤?中D的ì?长?è度¨¨项?
	read_block(FILE_SIGN_AREA,temp);
	temp[1]++;
	write_block(FILE_SIGN_AREA,temp);	
	
	return OK;
	
	
}

int destroy(char * filename)
/**************************删|?除y指?定?§文?件t。?ê
.在¨2目?录?里¤?搜?索??该?文?件t的ì?描¨¨述o?符¤?编à¨¤号?
.删|?除y该?文?件t对?应?|的ì?目?录?项?，ê?并?é更¨1新?位?图a?
.释o¨a放¤?文?件t描¨¨述o?符¤?
.返¤|ì回?状á??态??信?息?é
***************************/
{
	int i;
	int dtys;										//将?要°a删|?除y的ì?文?件t的ì?目?录?项?的ì?文?件t描¨¨述o?符¤?位?置?
	int dtyd;										//将?要°a删|?除y的ì?文?件t的ì?目?录?项?位?置?
	int use_block;									//该?文?件t实o|ì际¨o使o1用??的ì?块¨|数oy
	int index;
	char temp[B];
	char tempd[B];


	//***************依°¨¤据Y文?件t名?寻??找¨°文?件t的ì?目?录?项?和¨a文?件t描¨¨述o?符¤?
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == BUSY)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)
			{
				dtyd = i;								//找¨°到ì?文?件t目?录?项?位?置?
				dtys = temp[0] + FILE_SIGN_AREA;		//找¨°到ì?文?件t目?录?项?对?应?|的ì?文?件t描¨¨述o?符¤?位?置?(序¨°号?为atemp[0])
				index = temp[0];
				break;
			}
		}
	}
	if(i == K+FILE_NUM)
	{
		printf("没?有?D找¨°到ì?该?文?件t\n");
		return ERROR;
	}
	
	//************根¨′据Y文?件t描¨¨述o?符¤?即??（ê?§index）ê?查¨|看??该?文?件t是o?否¤?打?¨°开a
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
		printf("该?文?件t已°?经-被à?打?¨°开a,需¨¨要°a关?闭à?才?能¨1删|?除y\n");
		return ERROR;
	}


	//****************找¨°到ì?位?置?后¨?开a始o?操¨′作á??
	
	
	//****************依°¨¤次??更¨1新?文?件t块¨|数oy组á¨|中D指?定?§的ì?块¨|的ì?位?图a?区?
	read_block(dtys,temp);
	use_block = temp[2];
	for(i = 0 ; i < use_block ; i++)
	{
		read_block((temp[i+3]-K)/B,tempd);				
		tempd[(temp[i+3]-K)%B] = FREE;
		write_block((temp[i+3]-K)/B,tempd);
	}
	//***************删|?除y该?目?录?项?
	Init_block(temp,B);
	write_block(dtys,temp);
	
	


	//***************删|?除y文?件t描¨¨述o?符¤?
	Init_block(temp,B);
	write_block(dtyd,temp);

	//****************更¨1改?位?图a?区?
	read_block((dtyd-K)/B,temp);
	temp[(dtyd-K)%B] = FREE;
	write_block((dtyd-K)/B,temp);
	//**************目?录?文?件t描¨¨述o?符¤?中D的ì?长?è度¨¨减?1
	read_block(FILE_SIGN_AREA,temp);
	temp[1]--;
	write_block(FILE_SIGN_AREA,temp);

	
	return OK;

}
int open(char * filename)
/***************************打?¨°开a文?件t。?ê
该?函?￥数oy返¤|ì回?的ì?索??引°y号?可¨|用??于?¨2后¨?续?的ì?read, write, lseek, 或¨°close 操¨′作á??。?ê
.搜?索??目?录?找¨°到ì?文?件t对?应?|的ì?描¨¨述o?符¤?序¨°号?
.在¨2打?¨°开a文?件t表à¨a中D分¤?配?一°?个?表à¨a目?
.在¨2分¤?配?到ì?的ì?表à¨a目?中D把??读¨￠写??指?针?置?为a0，ê?并?é记?录?描¨¨述o?符¤?编à¨¤号?
.读¨￠入¨?文?件t的ì?第ì¨2一°?块¨|到ì?读¨￠写??缓o冲?区?中D
.返¤|ì回?分¤?配?到ì?的ì?表à¨a目?在¨2打?¨°开a文?件t表à¨a中D的ì?索??引°y号?
***************************/
{
	int i;
	int opd;
	int ops;
	int list;
	char temp[B];
	int index;
	//***************依°¨¤据Y文?件t名?寻??找¨°文?件t的ì?目?录?项?和¨a文?件t描¨¨述o?符¤?
	for(i = K ; i < K+FILE_NUM ; i++)
	{
		read_block((i-K)/B,temp);
		if(temp[(i-K)%B] == BUSY)
		{
			read_block(i,temp);
			if(strncmp(temp+1,filename,FILE_NAME_LENGTH) == 0)
			{
				opd = i;								//找¨°到ì?文?件t目?录?项?位?置?
				ops = temp[0] ;							//找¨°到ì?文?件t目?录?项?对?应?|的ì?文?件t描¨¨述o?符¤?序¨°号?
			//	printf("opd: %d,ops: %d\n",opd,ops);
				break;
			}
		}
	}
	if(i == K+FILE_NUM)
	{
		printf("没?有?D找¨°到ì?该?文?件t\n");
		return ERROR;
	}

	//*************查¨|看??该?文?件t是o?否¤?被à?打?¨°开a
	for(i = 0 ; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == ops && open_list[i].flag == BUSY)
		{
			printf("该?文?件t已°?经-被à?打?¨°开a\n");
			return ERROR;
		}
	}

	//**************找¨°一°?块¨|没?使o1用??的ì?表à¨a目?以°?便à?分¤?配?
	for(i = 0 ; i < FILE_NUM ; i++)
	{
		if(open_list[i].flag != BUSY)
		{	
			list = i;
			break;
		}
	}

	//******************对?表à¨a目?进?行D操¨′作á??
	
	open_list[list].filesignnum = ops;								//写??入¨?文?件t描¨¨述o?符¤?序¨°号?

	open_list[list].flag = BUSY;									//置?标à¨o志?位?为a占?用??
	
	index = open_list[list].filesignnum;							//生|¨2成¨|索??引°y号?(相¨¤当ì?à于?¨2该?文?件t目?录?项?的ì?文?件t描¨¨述o?符¤?序¨°号?)		
	lseek(index,0);													//指?针?指?向¨°文?件t首o?á部?(即??指?针?指?为a0)
	
	Init_block(open_list[list].buffer,BUFFER_LENGTH);				//初?始o?化?￥缓o冲?区?
	read_block(open_list[list].pointer[0],temp);					//读¨￠文?件t的ì?首o?á块¨|	
	strncpy(open_list[list].buffer,temp,BUFFER_LENGTH);				//把??文?件t的ì?第ì¨2一°?块¨|写??入¨?缓o冲?区?
	
	return OK;
	
}

int close(int index) 
/***************************关?闭à?文?件t。?ê
.把??缓o冲?区?的ì?内¨2容¨Y写??入¨?磁??盘¨?
.释o¨a放¤?该?文?件t再¨′打?¨°开a文?件t表à¨a中D对?应?|的ì?表à¨a目?
.返¤|ì回?状á??态??信?息?é
***************************/
{
	int i;
	int list = -1;
	char temp[B];
	//***************根¨′据Yindex找¨°表à¨a目?
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	if(list == -1)					//没?找¨°到ì?
	{
		printf("没?找¨°到ì?当ì?à前??索??引°y号?文?件t,操¨′作á??失o?ì败?¨1...\n");
		return ERROR;
	}
	if(open_list[list].flag != BUSY)		//输o?入¨?的ì?index对?应?|文?件t没?被à?打?¨°开a
	{
		printf("输o?入¨?的ì?索??引°y号?有?D误¨?，ê?操¨′作á??失o?ì败?¨1...\n");
		return ERROR;
	}
	
	//****************将?缓o冲?区?的ì?内¨2容¨Y写??入¨?磁??盘¨?
	/* //测a试o?用??
	for(i = 0 ; i < BUFFER_LENGTH-1; i++ )
	{
		open_list[list].buffer[i] = 5;
	}
	*/
	
	write_buffer(index,list);			//将?当ì?à前??list打?¨°开a文?件t表à¨a对?应?|的ì?缓o冲?区?入¨?index索??引°y号?的ì?文?件t
	
	//****************清?楚t操¨′作á?? 释o¨a放¤?表à¨a目?
	Init_block(open_list[list].buffer,BUFFER_LENGTH);				//清?除y缓o冲?区?
	open_list[list].filesignnum = FREE;								//清?除y文?件t描¨¨述o?符¤?
	open_list[list].flag = FREE;									//清?除y占?用??标à¨o志?位?
	open_list[list].pointer[0] = NULL;								//清?楚t指?针?
	open_list[list].pointer[1] = NULL;
	return OK;
}
int read(int index, int mem_area, int count) 
/*
从?¨?指?定?§文?件t顺3序¨°读¨￠入¨?count 个?字á?
节¨2mem_area 指?定?§的ì?内¨2存??位?置?。?ê读¨￠操¨′作á??从?¨?文?件t的ì?读¨￠写??指?针?指?示o?的ì?位?置?
开a始o?。?ê
*/
{
	int i;
	int list = -1;
	char temp[B];
	//***************根¨′据Yindex找¨°打?¨°开a文?件t表à¨a表à¨a目?
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	if(list == -1)					//没?找¨°到ì?
	{
		printf("没?找¨°到ì?当ì?à前??索??引°y号?文?件t,操¨′作á??失o?ì败?¨1...\n");
		return ERROR;
	}
	if(open_list[list].flag != BUSY)		//输o?入¨?的ì?index对?应?|文?件t没?被à?打?¨°开a
	{
		printf("输o?入¨?的ì?索??引°y号?有?D误¨?，ê?操¨′作á??失o?ì败?¨1...\n");
		return ERROR;
	}

	//***************根¨′据Yindex找¨°文?件t描¨¨述o?符¤?
	char temp_output[OUTPUT_LENGTH];
	Init_block(temp_output,OUTPUT_LENGTH);
	char output[OUTPUT_LENGTH];
	Init_block(output,OUTPUT_LENGTH);

	read_block(FILE_SIGN_AREA+index,temp);
	int file_length = temp[1];					//文?件t长?è度¨¨
	int file_block = temp[2];					//文?件t实o|ì际¨o使o1用??块¨|
	int file_area;

	//**********************拷?贝à??文?件t内¨2容¨Y至¨￠temp_output
	for(i = 0; i < file_block - 1 ; i++)
	{
		read_block(FILE_SIGN_AREA+index,temp);
		read_block(temp[3+i],temp);
		strncpy(temp_output+i*B,temp,B);			
	}
	read_block(FILE_SIGN_AREA+index,temp);
	read_block(temp[3+i],temp);
	strncpy(temp_output+i*B,temp,B);
	
	//******************当ì?à前??文?件t读¨￠写??坐á?标à¨o
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
	file_area = i * B + y;							//转áa换?为a文?件t内¨2相¨¤对?位?置?									
	
	for(i = 0 ;  i < count ; i++)
	{
		output[i+mem_area] = temp_output[i+file_area]; 
	}

	printf("%s\n",output+mem_area);
	return OK;
}
int write(int index,int mem_area,int count)
/*
把??mem_area 指?定?§的ì?内¨2存??位?置?开a
始o?的ì?count 个?字á?节¨2顺3序¨°写??入¨?指?定?§文?件t。?ê写??操¨′作á??从?¨?文?件t的ì?读¨￠写??指?针?指?示o?
的ì?位?置?开a始o?。?ê
*/
{
	int i;
	int list = -1;
	int input_length;
	char temp[B];

	//*************根¨′据Yindex找¨°到ì?文?件t打?¨°开a表à¨a
	for(i = 0; i < FILE_NUM ; i++)
	{
		if(open_list[i].filesignnum == index)
		{
			list = i;
			break;
		}
	}
	if(list == -1)					//没?找¨°到ì?
	{
		printf("没?找¨°到ì?当ì?à前??索??引°y号?文?件t,操¨′作á??失o?ì败?¨1...\n");
		return ERROR;
	}
	if(open_list[list].flag != BUSY)		//输o?入¨?的ì?index对?应?|文?件t没?被à?打?¨°开a
	{
		printf("输o?入¨?的ì?索??引°y号?有?D误¨?，ê?操¨′作á??失o?ì败?¨1...\n");
		return ERROR;
	}

	char input[INPUT_LENGTH];
	Init_block(input,INPUT_LENGTH);
	i = 0;
	fflush(stdin);
	while(scanf("%c",&input[i]))
	{
		if(input[i] == '\n')											//回?车|ì时o?à终?止1读¨￠入¨?
		{
			input[i] == '\0';			
			break;
		}
		i++;
	}
	input_length = i;		
	//*******************考?虑?输o?入¨?串??的ì?长?è度¨¨ 需¨¨要°a写??入¨?的ì?长?è度¨¨为a input_length - mem_area
	//缓o冲?区?容¨Y量￠?可¨|写??入¨?输o?入¨?内¨2容¨Y不?需¨¨要°a再¨′分¤?配?
	if(count <= BUFFER_LENGTH )					
	{
		strncat(open_list[list].buffer,input+mem_area,count);			//存??入¨?缓o冲?区?
	}
	//大?¨?于?¨2缓o冲?区?长?è度¨¨分¤?次??写??入¨?，ê?需¨¨要°a分¤?块¨|写??入¨?
	else													
	{
		int rest;						//当ì?à前??缓o冲?区?空?闲D容¨Y量￠?
		for(i = 0; i < BUFFER_LENGTH ; i++)
		{
			if(open_list[list].buffer[i] == FREE)
			{
				rest = BUFFER_LENGTH - i;
				break;
			}
		}		
		//第ì¨2一°?部?分¤?，ê?缓o冲?区?有?D一°?定?§容¨Y量￠?rest 将?缓o冲?区?写??入¨?文?件t，ê?清?空?缓o冲?区?		
		strncat(open_list[list].buffer+BUFFER_LENGTH-rest,input + mem_area,rest);
		write_buffer(index,list);
		Init_block(open_list[list].buffer,BUFFER_LENGTH);
		//第ì¨2二t部?分¤?，ê?循-环?¤ (input_length - mem_area)/BUFFER_LENGTH 块¨|缓o冲?区?写??入¨?文?件t
		for(i = 0; i < (count/BUFFER_LENGTH)-1 ; i++)
		{
			strncpy(open_list[list].buffer,(input+mem_area)+rest+i*BUFFER_LENGTH,BUFFER_LENGTH);
	//		printf("循-环?¤时o?à：êo第ì¨2%d块¨|缓o冲?区?：êobuffer: %s\n",i,open_list[list].buffer);
			write_buffer(index,list);
			Init_block(open_list[list].buffer,BUFFER_LENGTH);
		}
		//第ì¨2三¨y部?分¤?，ê?(count%BUFFER_LENGTH)剩o?ê余?¨¤未??满¨2一°?块¨|的ì?写??入¨?缓o冲?区?
		Init_block(open_list[list].buffer,BUFFER_LENGTH);
		strncpy(open_list[list].buffer,(input+mem_area)+rest+i*BUFFER_LENGTH,count%BUFFER_LENGTH);
		int buffer_start;
	//	printf("第ì¨2%d块¨|缓o冲?区?：êobuffer: %s\n",i,open_list[list].buffer);
	}
	return OK;
}

void directory()
//列￠D表à¨a显?示o?所¨′有?D文?件t及??其?长?è度¨¨。?ê
{
	int i;
	int filenum;
	int filelength;
	char filename[FILE_NAME_LENGTH];
	char temp[B];
	char tempd[B];
	char temps[B];
	read_block(FILE_SIGN_AREA,temp);
	filenum = temp[1];						//实o|ì际¨o存??在¨2的ì?文?件t个?数oy
	printf("\n");
	if(filenum == 0 )
	{
		printf("\t\t\t\t该?目?录?下?没?有?D文?件t\n");
	}
	
	for(i = 0; i < FILE_NUM; i++)
	{
		read_block(temp[3+i],tempd);					//读¨￠取¨?目?录?项?
		if(tempd[0] != 0)
		{
			read_block(tempd[0]+FILE_SIGN_AREA,temps);		//读¨￠取¨?文?件t描¨¨述o?符¤?
			if(temps[0] == BUSY && tempd[0] != 0)
			{	
				filelength = temps[1];
				strcpy(filename,tempd+1);
				printf("%-10s\t\t%-2d字á?节¨2\n",filename,filelength);
			}
		}
	}

	if(filenum != 0 )
	{
		printf("\t\t\t\t共2%d个?文?件t\n",filenum);
	}
}

int show_openlist()
{

	int i,j;
	int openfile = 0;
	char temp[B];
	int index;
	printf("\n索??引°y号?\t\t大?¨?小?\t\t文?件t名?\n");
	for(i = 0 ; i < FILE_NUM ; i ++)
	{
		if(open_list[i].flag == BUSY)
		{
			index = open_list[i].filesignnum;
			printf("  %-2d",index);
			openfile++;
			read_block(FILE_SIGN_AREA+index,temp);
			printf("\t\t %-2d",temp[1]);
			//根¨′据Yindex(文?件t描¨¨述o?符¤?序¨°号?)找¨°到ì?其?目?录?项?
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
	printf("**    命¨1令￠?                   说|ì明??                                       **\n");
	printf("** dir                  显?示o?目?录?内¨2容¨Y                                    **\n");
	printf("**                                                                      **\n");
	printf("** create+filename      新?建?§以°?filename为a文?件t名?的ì?文?件t                    **\n");
	printf("**                                                                      **\n");
	printf("** delete+filename      删|?除y以°?filename为a文?件t名?的ì?文?件t                    **\n");
	printf("**                                                                      **\n");
	printf("** open+filename        打?¨°开a以°?filename为a文?件t名?的ì?文?件t                    **\n");
	printf("**                                                                      **\n");
	printf("** close                关?闭à?index为a索??引°y号?的ì?文?件t                         **\n");
	printf("**                                                                      **\n");
	printf("** read                 请?根¨′据Y提?¨￠示o?，ê?接¨?着á?要°a求¨?输o?入¨?索??引°y号?以°?及??读¨￠取¨?长?è度¨¨进?行D读¨￠**\n");                 
	printf("**                      取¨?文?件t操¨′作á??                                      **\n");
	printf("**                                                                      **\n");
	printf("** write                请?根¨′据Y提?¨￠示o?，ê?接¨?着á?要°a求¨?输o?入¨?索??引°y号?以°?及??写??入¨?长?è度¨¨进?行D写??**\n");  
	printf("**                      入¨?文?件t操¨′作á??                                      **\n");
	printf("**                                                                      **\n");
	printf("** lseek                请?根¨′据Y提?¨￠示o?，ê?接¨?着á?要°a求¨?输o?入¨?一°?个?不?大?¨?于?¨2文?件t长?è度¨¨的ì?数oy字á?**\n");
	printf("**                      用??于?¨2定?§位?读¨￠写??指?针?                                **\n");
	printf("**                                                                      **\n");
	printf("** help                 帮??助¨2                                            **\n");
	printf("**                                                                      **\n");
	printf("** exit                 退a?出?文?件t系|ì统a3                                    **\n");
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

