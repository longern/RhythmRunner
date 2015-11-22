#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <Digitalv.h>

typedef struct
{
	MCI_OPEN_PARMS ae;
	WCHAR path[256];
} AUDIOENGINE;

extern AUDIOENGINE gAudio;

void AudioInit();
MCIERROR AudioOpen(const WCHAR *source);
MCIERROR AudioPlay(DWORD time = 0);
MCIERROR AudioPlayOnce(DWORD time = 0);
MCIERROR AudioClose();
LONG AudioCurrTime();
MCIERROR AudioSeekToStart();