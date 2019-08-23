#include "zcustompch.h"
#include "SceneManager.h"
#include <fstream>

int			SceneManager::planeChoice;
std::string	SceneManager::planeDecalChoice;
int			SceneManager::decalChoiceP;
Color		SceneManager::planeColor[2];
int			SceneManager::tankChoice;
std::string	SceneManager::tankDecalChoice;
int			SceneManager::decalChoiceT;
Color		SceneManager::tankColor[2];

int			SceneManager::money;
bool		SceneManager::planeUnlock[3];
int			SceneManager::planeCost[3];

SceneManager::SceneManager()
{
	planeChoice = 0;
	planeColor[0].Set(1, 1, 1);
	planeColor[1].Set(1, 1, 1);
	planeDecalChoice = "";

	tankChoice = 0;
	tankColor[0].Set(1, 1, 1);
	tankColor[1].Set(1, 1, 1);
	tankDecalChoice = "";

	planeUnlock[0] = true;

	for (int i = 0; i < 3; ++i)
	{
		planeCost[i] = 150 * i;
	}
	std::ifstream Colors("ColorSaves.txt");
	std::string temp;
	int i = 0;
	while (std::getline(Colors, temp))
	{
		++i;
		switch (i)
		{
		case 1:
			decalChoiceP = stoi(temp);
			break;
		case 2:
			planeColor[0].Set(temp);
			break;
		case 3:
			planeColor[1].Set(temp);
			break;
		case 4:
			decalChoiceT = stoi(temp);
			break;
		case 5:
			tankColor[0].Set(temp);
			break;
		case 6:
			tankColor[1].Set(temp);
			break;
		}
	}
}


SceneManager::~SceneManager()
{
	std::vector<std::string> lines;
	std::ofstream output;
	output.open("ColorSaves.txt");
	output << std::to_string(decalChoiceP) + "\n";
	output << std::to_string(planeColor[0].r) + "," + std::to_string(planeColor[0].g) + "," + std::to_string(planeColor[0].b) + "\n";
	output << std::to_string(planeColor[1].r) + "," + std::to_string(planeColor[1].g) + "," + std::to_string(planeColor[1].b) + "\n";
	output << std::to_string(decalChoiceT) + "\n";
	output << std::to_string(tankColor[0].r) + "," + std::to_string(tankColor[0].g) + "," + std::to_string(tankColor[0].b) + "\n";
	output << std::to_string(tankColor[1].r) + "," + std::to_string(tankColor[1].g) + "," + std::to_string(tankColor[1].b) + "\n";
}

void SceneManager::firstScene(string name)
{
	activeScene = sceneList[name];
}

void SceneManager::switchToScene(string name, Scene* curr)
{
	curr->readyExit = true;
	activeScene = sceneList[name];
}

void SceneManager::addScene(string name, Scene * scene)
{
	if ((scene == NULL) || (scene == nullptr))
	{
		return;
	}
	this->sceneList[name] = scene;
}

Scene * SceneManager::getActiveScene()
{
	return activeScene;
}

unordered_map<string, Scene*>& SceneManager::getList()
{
	return sceneList;
}

void SceneManager::readMonies()
{
	string line;
	ifstream stream;
	stream.open("SaveData/monies.txt");
	if (!stream)
	{
		cout << "File doesn't exist." << endl;
		exit(1);
	}
	while (getline(stream, line))
	{
		int comma = 0;
		int markr = 0; // after first comma

		for (unsigned int i = 0; i < line.length(); ++i)
		{
			// detect comma in line
			// note: second param in substr() indicates length
			if (line[i] == ',')
			{
				if (comma == 0)
				{
					money = stoi(line.substr(0, i));
				}
				if (comma == 1)
				{
					planeUnlock[1] = stoi(line.substr(i - 1, 1));
					planeUnlock[2] = stoi(line.substr(i + 1, 1));
				}
				++comma;
			}
		}
	}
	stream.close();
}

void SceneManager::writeMonies(int money, bool warthog, bool harrier)
{
	fstream stream;
	stream.open("SaveData/monies.txt", ofstream::out | ofstream::trunc);
	stream << money << "," << warthog << "," << harrier;
}

SceneManager * SceneManager::getSceneManager()
{
	return GetInstance();
}
