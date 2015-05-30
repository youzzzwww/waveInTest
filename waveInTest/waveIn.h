#pragma once
#include <Windows.h>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#include "structIni.h"

#define BLOCK_SIZE 2048
#define BLOCK_COUNT 4

extern CRITICAL_SECTION waveCriticalSection;

void CALLBACK waveInProc(HWAVEIN, UINT, DWORD, DWORD, DWORD);
WAVEHDR* allocateBlocks(int size, int count);
void freeBlocks(WAVEHDR* blockArray);

int myWaveInInitalize(waveBuffer* buffer);
DWORD WINAPI myWaveInStart();
int myWaveInDestory(waveBuffer* buffer);
