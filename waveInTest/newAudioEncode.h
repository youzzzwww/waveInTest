#pragma once
#include "enc_interface.h"
#include "stdio.h"
#include "string.h"

#pragma comment(lib,"encoder.lib")

int encoderInitialize(int sampling_rate, int channels);
int readBuffer(char** buffer, int buffer_size);
int encoder(unsigned char** out);
int encoderDestory();