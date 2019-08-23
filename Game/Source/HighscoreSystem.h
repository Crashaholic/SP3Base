#include "zcustompch.h"
#ifndef HIGHSCORE_SYSTEM
#define HIGHSCORE_SYSTEM

struct Score
{
	Score() {};
	~Score() {};
	Score(unsigned int a, std::string b)
	{
		score = a;
		name = b;
	}

	operator unsigned int()
	{
		return this->score;
	}
	operator std::string()
	{
		return this->name;
	}
	bool operator<(const Score& rhs)
	{
		return (this->score < rhs.score);
	}
	bool operator>(const Score& rhs)
	{
		return (this->score > rhs.score);
	}
	bool operator==(const Score& rhs)
	{
		return (this->score == rhs.score);
	}
	bool operator>=(const Score& rhs)
	{
		return (this->score >= rhs.score);
	}
	bool operator<=(const Score& rhs)
	{
		return (this->score <= rhs.score);
	}

	unsigned int score;
	std::string name;
};

class HighScoreSystem : public Singleton<HighScoreSystem>
{
public:
	HighScoreSystem();
	~HighScoreSystem();

	bool GetEligible(bool tank, unsigned int score);
	void SubmitHighscore(bool tank, unsigned int score, std::string name);

	std::string GetHighscore(bool tank);

	void ParseFile(bool tank, std::vector<Score>* s);
};

#endif