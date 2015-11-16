#include <fstream>
#include <windows.h>
#include "main.h"

VOID readBasicInfo(WCHAR *filePathName, SONGINFO *info)
{
	std::ifstream osuFileStream;
	osuFileStream.open(filePathName);

	osuFileStream.close();
}

VOID readBeats(WCHAR *filePathName)
{
	std::ifstream osuFileStream;
	osuFileStream.open(filePathName);

	osuFileStream.close();
}