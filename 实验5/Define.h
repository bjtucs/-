#define B		10			//��??��??���顧|��?���ȡ���
#define L		500			//��??��??���顧|�ܨ���1��oy
#define K		100			//����?������?��?��?��?С?
#define BUSY	1
#define FREE	0
#define OK		1
#define ERROR	0
#define FILE_BLOCK_LENGTH		(B-3)				//��?��t�֡�?��?��??�̡�?�顧|��?��oy�騢��|��?���ȡ���
#define FILE_NAME_LENGTH		(B-1)				//�?��?����?��t��?��?���ȡ���
#define FILE_SIGN_AREA			((L-1-K)/B+1)		//����?������?��?��D��?��t�ꨤ��oʶo?����?��eʼo?�顧|��?(λ?ͼa?֮?��?)
#define FILE_NUM				FILE_BLOCK_LENGTH	//Ŀ?¼?�ڡ�2�?�ࡧ����?��t��oyĿ?
#define BUFFER_LENGTH			25					//��?���㿪a��?��t����aĿ?��D�Ĩ�?��o��?��?��?���ȡ���
#define INPUT_LENGTH			100					//д??��?��tʱo?���?��?��?��o?�롧?��?���ȡ���
#define OUTPUT_LENGTH			100					//��������?��tʱo?���?��?��?�������?��?���ȡ���



struct filesign{							//��?��t�衧����o?����?
	int file_length;						//��?��t��?���ȡ���
	int filesign_flag;						//ռ?��??�ꨤ��oʶo?λ?
	int file_block;							//��?��t�֡�?��?��??�̡�?�顧|��?��oy�騢��|ʵo|���ʡ�o��?���ȡ���
	int file_block_ary[FILE_BLOCK_LENGTH];	//��?��t�֡�?��?��??�̡�?�顧|��?��oy�騢��|
};

struct contents{							//Ŀ?¼?��?
	char filename[FILE_NAME_LENGTH];		//��?��t��?
	int	 filesignnum;						//��?��t�衧����o?����?�򡧡��?
};

struct openfilelist{						//��?���㿪a��?��t����a����aĿ?
	char buffer[BUFFER_LENGTH];				//������д??��o��?��?
	int pointer[2];							//������д??ָ?��?(��?��t�ڡ�2�ݡ�Y�Ĩ�?λ?��?)
	int filesignnum;						//��?��t�衧����o?����?
	int flag;								//ռ?��??����?
};

char ldisk[L][B];						//��??�֨�?����?��oy�騢��|ģ?����a��??�̡�?

openfilelist open_list[FILE_NUM];		//��?���㿪a��?��t����a
