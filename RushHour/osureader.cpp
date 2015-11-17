#include <fstream>
#include <windows.h>
#include "main.h"

VOID readTimeLines(const std::string &line)
{
	;
}

VOID readBasicInfo(WCHAR *filePathName, SONGINFO *info)
{
	std::ifstream osuFileStream;
	osuFileStream.open(filePathName);
	std::string line, state;

	while (osuFileStream)
	{
		std::getline(osuFileStream, line);
		if (!line.length())
			continue;
		if (line.front() == '[' && line.back() == ']')
		{
			state = line.substr(1, -1);
			continue;
		}

		if (state == "General")
		{
			if (line.find_first_of("AudioFilename:") == 0)
			{
				line = line.substr(14);
				line = line.substr(line.find_first_not_of(" "));
			}
		}
		else
			continue;
	}

	osuFileStream.close();
}

VOID readBeats(WCHAR *filePathName)
{
	std::ifstream osuFileStream;
	osuFileStream.open(filePathName);
	std::string line, state;

	while (osuFileStream)
	{
		std::getline(osuFileStream, line);
		if (!line.length())
			continue;
		if (line.front() == '[' && line.back() == ']')
		{
			state = line.substr(1, -1);
			continue;
		}

		if (state == "TimeLines")
		{
			readTimeLines(line);
		}
		else
			continue;
	}
	osuFileStream.close();
}