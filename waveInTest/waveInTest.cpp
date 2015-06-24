#include "rtpSend.h"
//#include "waveIn.h"
#include "wavFileRead.h"
#include "newAudioEncode.h"
#include "structIni.h"
#include "tcpCommunication.h"

CRITICAL_SECTION csPacketSize;
int readFile(const char* filename, char** buffer, int &size, int &sample_rate, int &channels)
{
	wave_reader_error* error=new wave_reader_error;
	struct wave_reader* reader;
	int reader_num = 0;
	reader = wave_reader_open(filename, error);

	reader_num = wave_reader_get_num_samples(reader);
	channels = wave_reader_get_num_channels(reader);
	sample_rate = wave_reader_get_sample_rate(reader);
	//��ʼ�����뻺��
	size = reader_num*channels*wave_reader_get_sample_bits(reader)/8;
	*buffer = (char*)malloc( (size)*sizeof(char) );
	//�ļ����ݶ������뻺��
	wave_reader_get_samples(reader, reader_num, *buffer);

	//д���ļ�����
	//FILE* fin = fopen("..\\es03.pcm","wb+");
	//fwrite(*buffer, size, 1, fin);
	//fclose(fin);

	wave_reader_close(reader);
	return 0;
}
DWORD WINAPI encoderStart(LPVOID pParam)
{
	waveBuffer* wave_buffer = (waveBuffer*)pParam;
	char* buffer = wave_buffer->dataBuffer;
	int buffer_size = wave_buffer->size;
	int encoder_size=0;
	unsigned char* out=NULL;

	while( readBuffer(&buffer, buffer_size) )
	{
		encoder_size = encoder(&out);

		EnterCriticalSection(&csPacketSize);
		send_packet_split_frames(out,encoder_size,wave_buffer->frames_per_packet);
		LeaveCriticalSection(&csPacketSize);
	}
	//���Ϳհ�����
	rtpSend((unsigned char*)buffer, 0);
	
	return 0;
}
int sendFile(const char* filename)
{
	DWORD threadID_encoder,threadID_tcp;
	HANDLE hThread_encoder,hThread_tcp;
	int sample_rate=0,channels=0;
	waveBuffer buffer;
	buffer.frames_per_packet = 1;
	readFile(filename, &buffer.dataBuffer,buffer.size, sample_rate, channels);

	InitializeCriticalSection(&csPacketSize);
	encoderInitialize(sample_rate, channels);
	rtpInitialize("127.0.0.1", 30998);
	tcpIni("127.0.0.1", 30996);

	hThread_encoder = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)encoderStart, &buffer, 0, &threadID_encoder);
	hThread_tcp = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)tcpCommunicationStart, &buffer.frames_per_packet, 0, &threadID_tcp);

	while( WaitForSingleObject(hThread_encoder, INFINITE)!=WAIT_OBJECT_0 || 
		WaitForSingleObject(hThread_tcp, INFINITE)!=WAIT_OBJECT_0)
	{
		Sleep(80);
	}
	tcpDestroy();
	rtpDestory();
	encoderDestory();
	DeleteCriticalSection(&csPacketSize);
	return 0;
}
int main(int argc, char* argv[])
{
	const char* filename = "..\\es03.wav";
	sendFile(filename);
	return 1;
}