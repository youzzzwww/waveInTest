#pragma once
struct waveBuffer
{
	char* dataBuffer;
	int size;
	int frames_per_packet;  
};
#define SamplesPerSec 48000
#define BitsPerSample 16
#define Channels 1
#define MI  17