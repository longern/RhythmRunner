#include "main.h"
#include "audio.h"

int ToWindowX(double rx)
{
	return (int)(WNDWIDTH * rx);
}

int ToWindowY(double ry)
{
	return (int)(WNDHEIGHT * ry);
}

BOOL Circle(HDC hdc, int x, int y, int r)
{
	return Ellipse(hdc, x - r, y - r, x + r, y + r);
}

VOID SwitchSong()
{
	SONGINFO &currSong = global.songs[global.currentSong - 1];
	HANDLE findFile;
	WIN32_FIND_DATA fNextInfo;
	WCHAR firstMp3File[200];
	findFile = FindFirstFile((currSong.name + _T("/") + currSong.audioFilename).data(), &fNextInfo);
	if(findFile != INVALID_HANDLE_VALUE)
	{
		wsprintf(firstMp3File, TEXT("%s/%s"), global.songs[global.currentSong - 1].name.data(), currSong.audioFilename.data());
		UINT rs;
		rs = AudioOpen(firstMp3File);
		AudioPlay();
	}
}

LONG GLOBAL::timePass()
{
	LARGE_INTEGER ct;
	QueryPerformanceCounter(&ct);
	return (long)((ct.QuadPart - beginTime.QuadPart) / (double)clockFrequency.QuadPart * 1000);
}