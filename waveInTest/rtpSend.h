#pragma once
#include "ortp/ortp.h"
#include <time.h>
#pragma comment(lib,"oRTP.lib")
//#include "structIni.h"

int getCurrentMilliseconds();
//extern RtpSession *session;

int rtpInitialize(const char* ipAddr, int port);
int rtpSend(unsigned char* buffer, int size);
int send_packet_split_frames(unsigned char* out,int encoder_size, int frame_num);
//DWORD WINAPI rtpSend(LPVOID pParam);
int rtpDestory();