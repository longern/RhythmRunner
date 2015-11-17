#include "audio.h"

AUDIOENGINE gAudio;

void AudioInit()
{
    gAudio.ae.dwCallback = NULL;
    gAudio.ae.lpstrAlias = NULL;
    gAudio.ae.lpstrDeviceType =_T("MPEGAudio");
    gAudio.ae.wDeviceID = 0;
	gAudio.path[0] = WCHAR(0);
}

MCIERROR AudioOpen(const WCHAR *source)
{
	if(gAudio.ae.wDeviceID)
		AudioClose();

	wcscpy(gAudio.path, source);
	gAudio.ae.lpstrElementName = gAudio.path;
    return mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT, (DWORD)&gAudio.ae);
}

MCIERROR AudioPlay(DWORD time)
{
    MCI_PLAY_PARMS pp;
    pp.dwCallback = NULL;
    pp.dwFrom = time;
	return mciSendCommand(gAudio.ae.wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_DGV_PLAY_REPEAT, (DWORD)&pp);
}

MCIERROR AudioClose()
{
	return mciSendCommand(gAudio.ae.wDeviceID, MCI_CLOSE, MCI_NOTIFY, NULL);
}