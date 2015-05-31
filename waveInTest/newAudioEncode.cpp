#include "newAudioEncode.h"

long encoder_handle; //���������
unsigned char *in,*out;
FILE* wbFile;

int encoderInitialize(int sampling_rate, int channels)
{
	int BUF_MAX=65536,OUT_BUF_SIZE=1000;
	//��������ʼ��
	encoder_handle = createEncoder(channels, sampling_rate, 17);
	if(encoder_handle<0)
	{
		printf("create encoder fail\n");
		return 0;
	}
	in = (unsigned char *)malloc(BUF_MAX * sizeof(char));
	out = (unsigned char *)malloc(OUT_BUF_SIZE * sizeof(char));
	//��¼�ļ�
	wbFile = fopen("..\\encoder_record.wbp","wb+");

	return 0;
}
int readBuffer(char** Buffer, int buffer_size)
{
	int read_len;
	static int read_size=0;

	read_len = get_read_length(encoder_handle);
	if(read_size + read_len <= buffer_size)
	{
		memcpy(in, (*Buffer)+read_size, read_len);
		read_size += read_len;
	}
	else
		return 0;

	return 1;
}
int encoder(unsigned char** out_buffer)
{
	int ret=0;
	ret = doEncoder(encoder_handle, in, out);
	*out_buffer = out;
	//д��wb���ݵ���¼�ļ�
	//fwrite(out, ret, 1, wbFile);

	return ret;
}

int encoderDestory()
{
		//�ͷű������
	if(-1!=encoder_handle) exitEncoder(encoder_handle);
	if(NULL!=in) free(in);
	if(NULL!=out) free(out);
	//�رռ�¼�ļ�
	fclose(wbFile);
	return 0;
}