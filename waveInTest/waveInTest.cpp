#include "rtpSend.h"
//#include "waveIn.h"
#include "wavFileRead.h"
#include "newAudioEncode.h"
//#include <signal.h>

//struct waveBuffer buffer;
//DWORD threadID_waveIn,threadID_rtpSend;
//HANDLE hThread_waveIn,hThread_rtpSend;
//void catcher(int sig)
//{	
//	writeWAVData("..\\record.wav", buffer.dataBuffer, buffer.size, SamplesPerSec, BitsPerSample/8, Channels);	
//	myWaveInDestory(&buffer);
//	CloseHandle(hThread_waveIn);
//
//	WaitForSingleObject(hThread_rtpSend, INFINITE);
//	rtpDestory();
//	CloseHandle(hThread_rtpSend);
//	free(buffer.dataBuffer);
//	exit(0);
//}
//int main(int argc, char* argv[])
//{
//rtpInitialize();
//myWaveInInitalize(&buffer);
//hThread_waveIn = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myWaveInStart, NULL, 0, &threadID_waveIn);
//hThread_rtpSend = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rtpSend, &buffer, 0, &threadID_rtpSend);
//
//while(1)
//{
//	Sleep(20);
//	signal(SIGINT, catcher);
//}
//return 0;
//}

int readFile(const char* filename, char** buffer, int &size, int &sample_rate, int &channels)
{
	wave_reader_error* error=new wave_reader_error;
	struct wave_reader* reader;
	int reader_num = 0;
	reader = wave_reader_open(filename, error);

	reader_num = wave_reader_get_num_samples(reader);
	channels = wave_reader_get_num_channels(reader);
	sample_rate = wave_reader_get_sample_rate(reader);
	//初始化输入缓冲
	size = reader_num*channels*wave_reader_get_sample_bits(reader)/8;
	*buffer = (char*)malloc( (size)*sizeof(char) );
	//文件数据读入输入缓冲
	wave_reader_get_samples(reader, reader_num, *buffer);

	//写入文件测试
	//FILE* fin = fopen("..\\es03.pcm","wb+");
	//fwrite(*buffer, size, 1, fin);
	//fclose(fin);

	wave_reader_close(reader);
	return 0;
}
int sendFile(const char* filename)
{
	int sample_rate=0,channels=0;
	int buffer_size=0,encoder_size=0;
	char* buffer=NULL;
	unsigned char* out=NULL;
	readFile(filename, &buffer,buffer_size, sample_rate, channels);

	encoderInitialize(sample_rate, channels);
	rtpInitialize("127.0.0.1", 30998);
	while( readBuffer(&buffer, buffer_size) )
	{
		encoder_size = encoder(&out);
		send_four_frame(out,encoder_size);
	}
	//发送空包结束
	rtpSend((unsigned char*)buffer, 0);
	rtpDestory();
	encoderDestory();
	return 0;
}
int main(int argc, char* argv[])
{
	const char* filename = "..\\es03.wav";
	sendFile(filename);
	return 1;
}