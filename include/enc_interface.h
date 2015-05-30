#include <stdlib.h>  //没有这行，“extern "C"{   ” 会出错
#ifndef _ENC_INTERFACE_H
#define _ENC_INTERFACE_H


/**创建编码器
  *
  *numOfChannels为声道数
  *samplingRate为采样频率，单位Hz
  *返回值为编码器句柄
  *
**/
extern "C"{
_declspec(dllexport) long __stdcall createEncoder(int numOfChannels, long samplingRate,short mi);


/**编码一帧
  *
  *handle为编码器句柄
  *in为输入缓存(待编码的一帧原始语音)
  *out为码流缓存(存放编码后的码流)
  *返回值为码流的尺寸
  *
**/
_declspec(dllexport) int __stdcall doEncoder(long handle, unsigned char* in, unsigned char *out);

/************************************************************************/
/* 返回值：读取文件的长度                                               */
/************************************************************************/
_declspec(dllexport) int __stdcall get_read_length(long handle);




/**销毁编码器
  *
  *handle为编码器句柄
  *
**/
_declspec(dllexport) void __stdcall exitEncoder(long handle);


//新添加
void copy_data(float data[],int size, short data16[]);  

};
#endif


