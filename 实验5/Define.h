#define B		10			//存??储??é块¨|长?è度¨¨
#define L		500			//存??储??é块¨|总á¨1数oy
#define K		100			//保à?ê留￠?区?大?¨?小?
#define BUSY	1
#define FREE	0
#define OK		1
#define ERROR	0
#define FILE_BLOCK_LENGTH		(B-3)				//文?件t分¤?配?磁??盘¨?块¨|号?数oy组á¨|长?è度¨¨
#define FILE_NAME_LENGTH		(B-1)				//最á?长?è文?件t名?长?è度¨¨
#define FILE_SIGN_AREA			((L-1-K)/B+1)		//保à?ê留￠?区?中D文?件t标à¨o识o?符¤?起e始o?块¨|号?(位?图a?之?后¨?)
#define FILE_NUM				FILE_BLOCK_LENGTH	//目?录?内¨2最á?多¨¤文?件t数oy目?
#define BUFFER_LENGTH			25					//打?¨°开a文?件t表à¨a目?中D的ì?缓o冲?区?长?è度¨¨
#define INPUT_LENGTH			100					//写??文?件t时o?à最á?大?¨?输o?入¨?长?è度¨¨
#define OUTPUT_LENGTH			100					//读¨￠文?件t时o?à最á?大?¨?读¨￠出?长?è度¨¨



struct filesign{							//文?件t描¨¨述o?符¤?
	int file_length;						//文?件t长?è度¨¨
	int filesign_flag;						//占?用??标à¨o识o?位?
	int file_block;							//文?件t分¤?配?磁??盘¨?块¨|号?数oy组á¨|实o|ì际¨o长?è度¨¨
	int file_block_ary[FILE_BLOCK_LENGTH];	//文?件t分¤?配?磁??盘¨?块¨|号?数oy组á¨|
};

struct contents{							//目?录?项?
	char filename[FILE_NAME_LENGTH];		//文?件t名?
	int	 filesignnum;						//文?件t描¨¨述o?符¤?序¨°号?
};

struct openfilelist{						//打?¨°开a文?件t表à¨a表à¨a目?
	char buffer[BUFFER_LENGTH];				//读¨￠写??缓o冲?区?
	int pointer[2];							//读¨￠写??指?针?(文?件t内¨2容¨Y的ì?位?置?)
	int filesignnum;						//文?件t描¨¨述o?符¤?
	int flag;								//占?用??符¤?
};

char ldisk[L][B];						//用??字á?符¤?数oy组á¨|模?ê拟a磁??盘¨?

openfilelist open_list[FILE_NUM];		//打?¨°开a文?件t表à¨a
