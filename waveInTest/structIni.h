#pragma once
struct waveBuffer
{
	char* dataBuffer;
	int size;
	bool recordFlag;  //1 indicate recording ,0 indicate record terminated
};
#define SamplesPerSec 48000
#define BitsPerSample 16
#define Channels 1
#define MI  17