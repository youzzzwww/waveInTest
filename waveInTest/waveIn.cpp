#include "waveIn.h"

WAVEHDR* waveBlocks;
HWAVEIN hWaveIn;
CRITICAL_SECTION waveCriticalSection;

void CALLBACK waveInProc(
HWAVEIN hWaveIn,
UINT uMsg,
DWORD dwInstance,
DWORD dwParam1,
DWORD dwParam2
)
{
	if(uMsg != WIM_DATA)
		return;
	PWAVEHDR pWaveHdr = (PWAVEHDR)dwParam1;
	waveBuffer* buffer = (waveBuffer*)dwInstance;

	waveInUnprepareHeader(hWaveIn, pWaveHdr, sizeof(WAVEHDR));
	EnterCriticalSection(&waveCriticalSection);
	if(pWaveHdr->dwBytesRecorded > 0)
	{
		buffer->dataBuffer = (char*)realloc(buffer->dataBuffer, buffer->size + pWaveHdr->dwBytesRecorded);
		memcpy(buffer->dataBuffer+buffer->size, pWaveHdr->lpData, pWaveHdr->dwBytesRecorded);
		buffer->size += pWaveHdr->dwBytesRecorded;
	}
	LeaveCriticalSection(&waveCriticalSection);

	pWaveHdr->dwBytesRecorded = 0;
	pWaveHdr->dwFlags = 0;
	waveInPrepareHeader(hWaveIn, pWaveHdr, sizeof(WAVEHDR));
	waveInAddBuffer(hWaveIn, pWaveHdr, sizeof(WAVEHDR));
}
WAVEHDR* allocateBlocks(int size, int count)
{

char* buffer;
int i;
WAVEHDR* blocks;
DWORD totalBufferSize = (size + sizeof(WAVEHDR)) * count;
/*
* allocate memory for the entire set in one go
*/
if((buffer = (char*)HeapAlloc(
GetProcessHeap(),
HEAP_ZERO_MEMORY,
totalBufferSize
)) == NULL)

{

fprintf(stderr, "Memory allocationerror\n");
ExitProcess(1);

}
/*
* and set up the pointers to each bit
*/
blocks = (WAVEHDR*)buffer;
buffer += sizeof(WAVEHDR) * count;
for(i = 0; i < count; i++) {

blocks[i].dwBufferLength = size;
blocks[i].lpData = (LPSTR)buffer;
blocks[i].dwLoops = 1;
buffer += size;

}
return blocks;

}
void freeBlocks(WAVEHDR* blockArray)
{

/*
* and this is why allocateBlocks works the way it does
*/
HeapFree(GetProcessHeap(), 0, blockArray);

}

int myWaveInInitalize(waveBuffer* buffer)
{
	InitializeCriticalSection(&waveCriticalSection);
	WAVEFORMATEX wfx; /* look this up in your documentation */

	buffer->dataBuffer = (char*)malloc(1*sizeof(char));
	buffer->size = 0;
	buffer->recordFlag = 1;
	//*
	//* initialise the module variables
	//*/
	waveBlocks = allocateBlocks(BLOCK_SIZE, BLOCK_COUNT);

	//*
	//* set up the WAVEFORMATEX structure.
	//*/
	wfx.nSamplesPerSec = SamplesPerSec; /* sample rate */
	wfx.wBitsPerSample = BitsPerSample; /* sample size */
	wfx.nChannels= Channels; /* channels*/
	wfx.cbSize = 0; /* size of _extra_ info */
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
	//*
	//* try to open the default wave device. WAVE_MAPPER is
	//* a constant defined in mmsystem.h, it always points to the
	//* default wave device on the system (some people have 2 or
	//* more sound cards).
	//*
	int recordnum = waveInGetNumDevs(); //check the record device nums if exist
	int mmr = waveInOpen(&hWaveIn,WAVE_MAPPER,&wfx,(DWORD_PTR)waveInProc,(DWORD_PTR)buffer,CALLBACK_FUNCTION);
	if(mmr != MMSYSERR_NOERROR)
	{
	fprintf(stderr, "%s: unable to open wave mapper device\n");
	ExitProcess(1);
	}
	return 1;
}

DWORD WINAPI myWaveInStart()
{
	//HWAVEIN hWaveIn; /* device handle */
	int i;
	//…Ë÷√À´ª∫≥Â
	for(i=0;i<BLOCK_COUNT;i++)
	{
		waveInPrepareHeader(hWaveIn,&waveBlocks[i],sizeof(WAVEHDR));
		waveInAddBuffer(hWaveIn,&waveBlocks[i],sizeof(WAVEHDR));
	}
	// Begin sampling    
	waveInStart(hWaveIn);
	return 1;
}
int myWaveInDestory(waveBuffer* buffer)
{	
	int i;
	waveInStop(hWaveIn);
	//waveInReset(hWaveIn);
	for(i=0;i<BLOCK_COUNT;i++)
		waveInUnprepareHeader(hWaveIn,&waveBlocks[i],sizeof(WAVEHDR));
	waveInClose(hWaveIn);
	freeBlocks(waveBlocks);
	DeleteCriticalSection(&waveCriticalSection);
	//change the record flag to continue send rtp package
	buffer->recordFlag = 0;
	return 1;
}
