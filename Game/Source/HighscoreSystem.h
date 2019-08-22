#ifndef HIGHSCORE_SYSTEM
#define HIGHSCORE_SYSTEM
#include <SingletonTemplate.h>
#include <string>

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

	void SubmitHighscoreP(unsigned int score, std::string name);
	void SubmitHighscoreT(unsigned int score, std::string name);

	std::string GetHighscoreP();
	std::string GetHighscoreT();

};

#endif