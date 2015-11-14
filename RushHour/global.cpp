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
	HANDLE findFile;
	WIN32_FIND_DATA fNextInfo;
	WCHAR firstMp3File[200];
	wsprintf(firstMp3File, TEXT("%s/*.mp3"), global.songs[global.currentSong - 1].name);
	findFile = FindFirstFile(firstMp3File, &fNextInfo);
	if(findFile != INVALID_HANDLE_VALUE)
	{
		wsprintf(firstMp3File, TEXT("%s/%s"), global.songs[global.currentSong - 1].name, fNextInfo.cFileName);
		UINT rs;
		rs = AudioOpen(firstMp3File);
		AudioPlay();
	}
}