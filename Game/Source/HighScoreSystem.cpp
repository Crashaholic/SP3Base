#include "zcustompch.h"
#include "HighscoreSystem.h"

HighScoreSystem::HighScoreSystem()
{
}

HighScoreSystem::~HighScoreSystem()
{
}

void HighScoreSystem::ParseFile(bool tank, std::vector<Score>* s)
{
	std::string results = GetHighscore(tank);
	size_t lastpos = 0;
	for (size_t i = 0; i < results.size(); ++i)
	{
		if (results[i] == '\n')
		{
			std::string temp;
			temp = results.substr(lastpos, i - lastpos);
			{
				size_t seperator = temp.find(",");
				Score currentLine;
				currentLine.score = std::stoul(temp.substr(0, seperator));
				currentLine.name = temp.substr(seperator + 1);
				s->push_back(currentLine);
			}
			lastpos = i + 1;
		}
	}
}

bool HighScoreSystem::GetEligible(bool tank, unsigned int score)
{
	std::fstream fileobj;
	Score lastScore;

	if (!tank)
		fileobj.open("SaveData/highscoresP.txt");
	else
		fileobj.open("SaveData/highscoresT.txt");
	std::string temp;
	unsigned short counter = 0;
	while (std::getline(fileobj, temp))
	{
		if (counter == 4)
		{
			uint32_t seperator = temp.find(",");
			Score currentLine;
			currentLine.score = std::stoul(temp.substr(0, seperator));
			currentLine.name = temp.substr(seperator + 1);
			lastScore = currentLine;
		}
		++counter;
		if (counter > 4) break;
	}
	fileobj.close();

	return score >= lastScore.score;
}

void HighScoreSystem::SubmitHighscore(bool tank, unsigned int score, std::string name)
{
	std::fstream fileobj;
	std::vector<Score> currentHighscores;

	Score current { score, name };
	if (!GetEligible(tank, score))
	{
		return;
	}

	ParseFile(tank, &currentHighscores);

	fileobj.close();
	currentHighscores.push_back(current);
	std::sort(currentHighscores.rbegin(), currentHighscores.rend());
	currentHighscores.pop_back();
	if (!tank)
		fileobj.open("SaveData/highscoresP.txt", std::ios::out);
	else
		fileobj.open("SaveData/highscoresT.txt", std::ios::out);
	for (size_t i = 0; i < currentHighscores.size(); i++)
	{
		fileobj << currentHighscores[i].score << ',' << currentHighscores[i].name << '\n';
	}
	fileobj.close();
}

std::string HighScoreSystem::GetHighscore(bool tank)
{
	std::fstream fileobj;
	std::vector<Score> currentHighscores;

	if (!tank)
		fileobj.open("SaveData/highscoresP.txt");
	else
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
