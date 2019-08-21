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

void HighScoreSystem::SubmitHighscore(unsigned int score, std::string name)
{
	std::fstream fileobj;
	std::vector<Score> currentHighscores;

	fileobj.open("SaveData/highscores.txt");
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

	Score current { score, name };

	if (current < currentHighscores.back())
	{
		throw "Git gud";
		return;
	}

	currentHighscores.push_back(current);
	std::sort(currentHighscores.rbegin(), currentHighscores.rend());
	currentHighscores.pop_back();
	for (size_t i = 0; i < currentHighscores.size(); i++)
	{
		fileobj << currentHighscores[i].score << ',' << currentHighscores[i].name << '\n';
	}
	fileobj.close();
}
