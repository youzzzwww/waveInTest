#include <stdlib.h>  //û�����У���extern "C"{   �� �����
#ifndef _ENC_INTERFACE_H
#define _ENC_INTERFACE_H


/**����������
  *
  *numOfChannelsΪ������
  *samplingRateΪ����Ƶ�ʣ���λHz
  *����ֵΪ���������
  *
**/
extern "C"{
_declspec(dllexport) long __stdcall createEncoder(int numOfChannels, long samplingRate,short mi);


/**����һ֡
  *
  *handleΪ���������
  *inΪ���뻺��(�������һ֡ԭʼ����)
  *outΪ��������(��ű���������)
  *����ֵΪ�����ĳߴ�
  *
**/
_declspec(dllexport) int __stdcall doEncoder(long handle, unsigned char* in, unsigned char *out);

/************************************************************************/
/* ����ֵ����ȡ�ļ��ĳ���                                               */
/************************************************************************/
_declspec(dllexport) int __stdcall get_read_length(long handle);




/**���ٱ�����
  *
  *handleΪ���������
  *
**/
_declspec(dllexport) void __stdcall exitEncoder(long handle);


//�����
void copy_data(float data[],int size, short data16[]);  

};
#endif


