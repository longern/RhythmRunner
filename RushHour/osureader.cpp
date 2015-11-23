#include <fstream>
#include "osureader.h"

std::wstring strtowstr(const std::string &str)
{
	std::wstring wstr;
	wstr.resize(str.length() + 1);
	for (unsigned int i = 0; i < str.length(); i++)
		wstr[i] = wchar_t(str[i]);
	wstr[str.length()] = '\0';
	return wstr;
}

VOID readHitObjects(std::string str)
{
	global.barriers.push_back(BARRIERINFO());
	UINT barrierLast = global.barriers.size() - 1;
	UINT comaCount = 0;
	UINT i;
	//  Find the second coma
	for (i = 0; i < 2; i++)
		str = str.substr(str.find_first_of(',') + 1);
	int barrierTime = std::atol(str.data());
	global.barriers.back().msecs = barrierTime;
	global.barriers.back().type = std::rand() % 2;
	global.barriers.back().track = std::rand() % 4;

	//  Reproduce when too dense
	if (barrierLast >= 2)
		while (global.barriers[barrierLast - 1].track == global.barriers[barrierLast].track
			&& global.barriers[barrierLast - 2].track == global.barriers[barrierLast].track)
			global.barriers.back().track = std::rand() % 4;

	//  Transform sliders into barriers
	if (str.find_first_of('|') != std::string::npos)
	{
		UINT repeatTimes;
		double pixelLength;
		str = str.substr(str.find_first_of('|') + 1);
		str = str.substr(str.find_first_of(',') + 1);
		repeatTimes = std::atoi(str.data());
		str = str.substr(str.find_first_of(',') + 1);
		pixelLength = std::atof(str.data());
		double sliderDuration = pixelLength / global.currSong().sliderMultiplier / 100. * global.currSong().msPerBeat;
		for (i = 1; i <= repeatTimes; i++)
		{
			global.barriers.push_back(BARRIERINFO());
			barrierLast = global.barriers.size() - 1;
			global.barriers.back().msecs = (LONG)(barrierTime + i * sliderDuration);
			global.barriers.back().type = std::rand() % 3;
			global.barriers.back().track = std::rand() % 4;
			if (barrierLast >= 2)
				while (global.barriers[barrierLast - 1].track == global.barriers[barrierLast].track
					&& global.barriers[barrierLast - 2].track == global.barriers[barrierLast].track)
					global.barriers.back().track = std::rand() % 4;
		}
	}
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
				info->audioFilename = strtowstr(line.substr(line.find_first_not_of(' ')));
			}
			else if (line.find("AudioLeadIn:") != std::string::npos)
			{
				line = line.substr(12);
				info->audioLeadIn = std::atoi(line.substr(line.find_first_not_of(' ')).data());
			}
			else if (line.find("PreviewTime:") != std::string::npos)
			{
				line = line.substr(12);
				info->previewTime = std::atoi(line.substr(line.find_first_not_of(' ')).data());
			}
		}
		else if (state == "Metadata")
		{
			if (line.find("Title:") != std::string::npos)
			{
				line = line.substr(6);
				info->title = strtowstr(line.substr(line.find_first_not_of(' ')));
			}
		}
		else if (state == "TimingPoints")
		{
			info->msPerBeat = std::atof(line.substr(line.find(",") + 1).data());
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
		if (state == "Difficulty")
		{
			if (line.find("SliderMultiplier:") != std::string::npos)
			{
				line = line.substr(17);
				global.currSong().sliderMultiplier = std::atof(line.substr(line.find_first_not_of(' ')).data());
			}
		}
		if (state == "HitObjects")
		{
			readHitObjects(line);
		}
		else
			continue;
	}
	osuFileStream.close();
}
