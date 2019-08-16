#ifndef HIGHSCORE_SYSTEM
#define HIGHSCORE_SYSTEM

class HighScoreSystem
{
private:
	HighScoreSystem();
	static HighScoreSystem* Instance;
public:
	static HighScoreSystem* GetInstance()
	{
		if (!Instance)
			Instance = new HighScoreSystem;
		return Instance;
	}



};

#endif