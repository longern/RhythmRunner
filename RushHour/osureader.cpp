#include <fstream>
#include "osureader.h"

std::wstring strtowstr(const std::string &str)
{
	std::wstring wstr;
	wstr.resize(str.length() + 1);
	for(unsigned int i = 0; i < str.length(); i++)
		wstr[i] = wchar_t(str[i]);
	wstr[str.length()] = '\0';
	return wstr;
}

VOID readTimingPoints(const std::string &str)
{
	global.barriers.push_back(BARRIERINFO());
	UINT barrierLast = global.barriers.size() - 1;
	UINT comaCount = 0;
	unsigned int i;
	for(i = 0; i < str.length(); i++)
	{
		if(str[i] == ',')
			comaCount++;
		if(comaCount >= 2)
		{
			i++;
			break;
		}
	}
	global.barriers.back().msecs = std::atol(str.data() + i);
	global.barriers.back().type = std::rand() % 4;
	global.barriers.back().track = std::rand() % 4;
	if(barrierLast >= 2)
		while(global.barriers[barrierLast - 1].track == global.barriers[barrierLast].track
		   && global.barriers[barrierLast - 2].track == global.barriers[barrierLast].track)
			global.barriers.back().track = std::rand() % 4;
}

VOID readBasicInfo(const WCHAR *filePathName, SONGINFO *info)
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
			state = line.substr(1, line.length() - 2);
			continue;
		}

		if (state == "General")
		{
			if (line.find("AudioFilename:") != std::string::npos)
			{
				line = line.substr(14);
				global.songs[global.totalSongCount].audioFilename = strtowstr(line.substr(line.find_first_not_of(' ')));
			}
			else if (line.find("AudioLeadIn:") != std::string::npos)
			{
				line = line.substr(12);
				global.songs[global.totalSongCount].audioLeadIn = std::atoi(line.substr(line.find_first_not_of(' ')).data());
			}
		}
		else if (state == "Metadata")
		{
			if (line.find("Title:") != std::string::npos)
			{
				line = line.substr(6);
				global.songs[global.totalSongCount].title = strtowstr(line.substr(line.find_first_not_of(' ')));
			}
		}
		else if (state == "TimingPoints")
		{
			global.songs[global.totalSongCount].msPerBeat = std::atof(line.substr(line.find(",") + 1).data());
			state = "";
		}
		else
			continue;
	}

	osuFileStream.close();
}

VOID readBeats(const WCHAR *filePathName)
{
	global.barriers.clear();
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
			state = line.substr(1, line.length() - 2);
			continue;
		}

		if (state == "HitObjects")
		{
			readTimingPoints(line);
		}
		else
			continue;
	}
	osuFileStream.close();
}