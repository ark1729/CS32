#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(std::string assetDir)
	: GameWorld(assetDir)
{}

StudentWorld::~StudentWorld()
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		delete m_actors[i];
		m_actors.clear();
	}
}

int StudentWorld::loadLevelAndCreate()
{
	std::ostringstream oss;
	int k = getLevel();
	oss.fill('0');
	oss << std::setw(2) << k;
	std::string lvl = oss.str();
	std::string curLevel = "level" + lvl + ".dat";

	Level lev(assetDirectory());
	Level::LoadResult result = lev.loadLevel(curLevel);
	if (result == Level::load_fail_file_not_found)
	{
		return GWSTATUS_PLAYER_WON;
	}
	if (result == Level::load_fail_bad_format)
	{
		return GWSTATUS_LEVEL_ERROR;
	}

	for (int i = 0; i < VIEW_WIDTH; i++)
	{
		for (int j = 0; j < VIEW_HEIGHT; j++)
		{
			Level::MazeEntry item = lev.getContentsOf(i, j);
			if (item == Level::player)
			{
				m_actors.push_back(new Player(this, i, j));
			}
			if (item == Level::wall)
			{
				m_actors.push_back(new Wall(this, i, j));
			}
		}
	}
	m_bonus = 1000;
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::init()
{
	int result = loadLevelAndCreate();
	return result;
}

int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after hitting enter a few times 
	//decLives();
	//return GWSTATUS_CONTINUE_GAME;

	if (getLives() == 0)
	{
		return GWSTATUS_PLAYER_DIED;
	}
	for (std::vector< Actor* >::iterator i = m_actors.begin(); i != m_actors.end(); i++)
	{
		(*i)->doSomething();
	}
	for (std::vector< Actor* >::iterator i = m_actors.begin(); i != m_actors.end(); i++)
	{
		if ((*i)->seeDead() == true)
		{
			if ((*i)->getID() == IID_PLAYER)
			{
				delete (*i);
				return GWSTATUS_PLAYER_DIED;
			}
			delete(*i);
		}
	}

	if (!(m_bonus == 0))
		m_bonus--;
	setDisplayText();

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		delete m_actors[i];
		m_actors.clear();
	}
}

bool StudentWorld::ifActorBlocks(int x, int y)
{
	for (int i = 0; i < m_actors.size(); i++)
	{
		if (m_actors[i]->seeBlock())
		{
			if (m_actors[i]->getX() == x && m_actors[i]->getY() == y)
			{
				return true;
			}
		}
	}
	return false;
}

int StudentWorld::getCurrentLevelBonus()
{
	return m_bonus;
}

void StudentWorld::setDisplayText()
{
	int score = getScore();
	int level = getLevel();
	unsigned int bonus = getCurrentLevelBonus();
	int livesLeft = getLives();
	int health = 100;
	int ammo = 50;
	string s = Format(score, level, livesLeft, health, ammo, bonus);
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

string Format(int score, int level, int livesLeft, int health, int ammo, int bonus)
{
	ostringstream ss;
	ss.fill('0');
	ss << std::setw(7) << score;
	std::string s_score = ss.str();
	ostringstream sl;
	sl.fill('0');
	sl << std::setw(2) << level;
	std::string s_level = sl.str();
	ostringstream sll;
	sll.fill(' ');
	sll << std::setw(2) << livesLeft;
	std::string s_lives = sll.str();
	ostringstream sh;
	sh.fill(' ');
	sh << std::setw(3) << health << "%";
	std::string s_health = sh.str();
	ostringstream sa;
	sa.fill(' ');
	sa << std::setw(3) << ammo;
	std::string s_ammo = sa.str();
	ostringstream sb;
	sb.fill(' ');
	sb << std::setw(4) << bonus;
	std::string s_bonus = sb.str();
	string n = "Score: " + s_score + "  " + "Level: " + s_level + "  " + "Lives: " + s_lives + "  " + "Health: " + s_health + "  " + "Ammo: " + s_ammo + "  " + "Bonus: " + s_bonus;
	return n;
}
