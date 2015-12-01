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

std::vector<std::string> strsplit(const std::string &str, char delimiter)
{
	std::vector<std::string> res;
	size_t prevoff = 0, off;
	if (str.length() == 0)
		return res;

	while ((off = str.find(delimiter, prevoff)) != std::string::npos)
	{
		res.push_back(str.substr(prevoff, off));
		prevoff = off + 1;
	}
	res.push_back(str.substr(prevoff));
	return res;
}

VOID produceHitObject(int t, bool includeItem)
{
	BARRIERINFO barr;
	barr.msecs = t;
	
	if (includeItem)
	{
		barr.type = std::rand() % 6;
		if (barr.type >= 3)
			barr.type = 0;
	}
	else
	{
		barr.type = std::rand() % 5;
		if (barr.type >= 2)
			barr.type = 0;
	}

	//  Check which track can be appended
	int avlTrack[4] = { 1, 1, 1, 1 }, avlCount = 0;
	for (int i = 0; i < 4; i++)
	{
		if (global.barriers[i].size() >= 1)
		{
			if (global.barriers[i].back().msecs + global.currSong().msPerBeat / 4. + 1. >= barr.msecs)
				avlTrack[i] = 0;
			if (global.barriers[i].back().type == 1 && global.barriers[i].back().height == 0. &&
				global.barriers[i].back().msecs + global.currSong().msPerBeat / 2. + 1. >= barr.msecs)
				avlTrack[i] = 0;
		}
		if (global.barriers[i].size() >= 2)
		{
			BOOL tripleBarr = TRUE;
			for (int j = 0; j < 4; j++)
				if (i != j && global.barriers[j].size() &&
					global.barriers[j].back().msecs > global.barriers[i][global.barriers[i].size() - 2].msecs)
					tripleBarr = FALSE;
			if (tripleBarr)
				avlTrack[i] = 0;
		}
	}
	for (int i = 0; i < 4; i++)
		if (avlTrack[i])
			avlCount++;
	int track = std::rand() % avlCount;
	for (int i = 0; i <= track; i++)
		if (!avlTrack[i])
			track++;

	//  Change the height
	int lastHeight = 0;
	if (global.barriers[track].size())
		lastHeight = global.barriers[track].back().height;
	if (barr.type == 1)
		barr.height = 1 - lastHeight;
	else
		barr.height = lastHeight;

	global.barriers[track].push_back(barr);
}

VOID readHitObjects(std::string str)
{
	UINT comaCount = 0;
	UINT i;

	//  Find the second coma
	for (i = 0; i < 2; i++)
		str = str.substr(str.find_first_of(',') + 1);
	int barrierTime = std::atol(str.data());
	produceHitObject(barrierTime, false);

	//  Transform sliders into barriers
	if (str.find_first_of('|') != std::string::npos)
	{
		UINT repeatTimes;  //  Calc the how many barriers will a slider produce
		double pixelLength;
		str = str.substr(str.find_first_of('|') + 1);
		str = str.substr(str.find_first_of(',') + 1);
		repeatTimes = std::atoi(str.data());
		str = str.substr(str.find_first_of(',') + 1);
		pixelLength = std::atof(str.data());
		double sliderDuration = pixelLength / global.currSong().sliderMultiplier / 100. * global.currSong().msPerBeat;
		//  Calc How many milliseconds is between two barriers

		//  Repeatly produce barriers
		for (i = 1; i <= repeatTimes; i++)
			produceHitObject((LONG)(barrierTime + i * sliderDuration), true);
	}
}

VOID readBasicInfo(const WCHAR *filePathName, SONGINFO *info)
{
	std::ifstream osuFileStream;
	osuFileStream.open(filePathName);
	std::string line, state;

	info->beatmapSetId = _wtoi(filePathName);

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
		else if (state == "Events")
		{
			if (line[0] == '0')
			{
				std::vector<std::string> res = strsplit(line, ',');
				if (res.size() >= 3)
				{
					std::string fileWithQuote = res[2];
					info->bgImgFile = strtowstr(fileWithQuote.substr(1, fileWithQuote.length() - 2));
				}
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
	for (int i = 0; i < 4;i++)
		global.barriers[i].clear();
	std::ifstream osuFileStream;
	osuFileStream.open(filePathName);
	std::string line, state;

	srand(global.currSong().beatmapSetId);

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
