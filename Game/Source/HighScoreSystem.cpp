#include "HighscoreSystem.h"
#include <fstream>
#include <vector>
#include <algorithm>

HighScoreSystem::HighScoreSystem()
{

}

HighScoreSystem::~HighScoreSystem()
{

}

void HighScoreSystem::SubmitHighscoreP(unsigned int score, std::string name)
{
	std::fstream fileobj;
	std::vector<Score> currentHighscores;

	fileobj.open("SaveData/highscoresP.txt");
	std::string temp;
	unsigned short counter = 0;
	while (std::getline(fileobj, temp))
	{
		uint32_t seperator = temp.find(",");
		Score currentLine;
		currentLine.score = std::stoul(temp.substr(0, seperator));
		currentLine.name = temp.substr(seperator + 1);
		currentHighscores.push_back(currentLine);
		++counter;
		if (counter > 4) break;
	}
	fileobj.close();

	Score current { score, name };

	if (current < currentHighscores.back())
	{
		throw "Git gud";
		return;
	}

	currentHighscores.push_back(current);
	std::sort(currentHighscores.rbegin(), currentHighscores.rend());
	currentHighscores.pop_back();
	fileobj.open("SaveData/highscoresP.txt");
	for (size_t i = 0; i < currentHighscores.size(); i++)
	{
		fileobj << currentHighscores[i].score << ',' << currentHighscores[i].name << '\n';
	}
	fileobj.close();
}

void HighScoreSystem::SubmitHighscoreT(unsigned int score, std::string name)
{
	std::fstream fileobj;
	std::vector<Score> currentHighscores;

	fileobj.open("SaveData/highscoresT.txt");
	std::string temp;
	unsigned short counter = 0;
	while (std::getline(fileobj, temp))
	{
		uint32_t seperator = temp.find(",");
		Score currentLine;
		currentLine.score = std::stoul(temp.substr(0, seperator));
		currentLine.name = temp.substr(seperator + 1);
		currentHighscores.push_back(currentLine);
		++counter;
		if (counter > 4) break;
	}
	fileobj.close();

	Score current { score, name };

	if (current < currentHighscores.back())
	{
		throw "Git gud";
		return;
	}

	currentHighscores.push_back(current);
	std::sort(currentHighscores.rbegin(), currentHighscores.rend());
	currentHighscores.pop_back();
	fileobj.open("SaveData/highscoresT.txt");
	for (size_t i = 0; i < currentHighscores.size(); i++)
	{
		fileobj << currentHighscores[i].score << ',' << currentHighscores[i].name << '\n';
	}
	fileobj.close();
}

std::string HighScoreSystem::GetHighscoreP()
{
	std::fstream fileobj;
	std::vector<Score> currentHighscores;

	fileobj.open("SaveData/highscoresP.txt");
	std::string temp;
	unsigned short counter = 0;
	std::string total;
	while (std::getline(fileobj, temp))
	{
		total += temp + '\n';
	}
	fileobj.close();
	return total;
}

std::string HighScoreSystem::GetHighscoreT()
{
	std::fstream fileobj;
	std::vector<Score> currentHighscores;

	fileobj.open("SaveData/highscoresT.txt");
	std::string temp;
	unsigned short counter = 0;
	std::string total;
	while (std::getline(fileobj, temp))
	{
		total += temp + '\n';
	}
	fileobj.close();
	return total;
}

