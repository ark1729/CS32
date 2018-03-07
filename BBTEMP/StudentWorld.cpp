#include "StudentWorld.h"
#include <string>
#include <cstdlib>

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
	delete m_player;
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
		delete (*itr);
	m_actors.clear();
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
				m_player = (new Player(this, i, j));
			}
			if (item == Level::wall)
			{
				m_actors.push_back(new Wall(this, i, j));
			}
			if (item == Level::boulder)
			{
				m_actors.push_back(new Boulder(this, i, j));
			}
			if (item == Level::hole)
			{
				m_actors.push_back(new Hole(this, i, j));
			}
			if (item == Level::jewel)
			{
				m_actors.push_back(new Jewel(this, i, j));
				jewel++;
			}
			if (item == Level::exit)
			{
				m_actors.push_back(new Exit(this, i, j));
			}
			if (item == Level::ammo)
			{
				m_actors.push_back(new AmmoGoodie(this, i, j));
			}
			if (item == Level::restore_health)
			{
				m_actors.push_back(new RestoreHealthGoodie(this, i, j));
			}
			if (item == Level::extra_life)
			{
				m_actors.push_back(new ExtraLifeGoodie(this, i, j));
			}
			if (item == Level::horiz_snarlbot)
			{
				m_actors.push_back(new SnarlBot(this, i, j, Actor::right));
			}
			if (item == Level::vert_snarlbot)
			{
				m_actors.push_back(new SnarlBot(this, i, j, Actor::down));
			}
			if (item == Level::kleptobot_factory)
			{
				m_actors.push_back(new KleptoBotFactory(this, i, j, KleptoBotFactory::REGULAR));
			}
			if (item == Level::angry_kleptobot_factory)
			{
				m_actors.push_back(new KleptoBotFactory(this, i, j, KleptoBotFactory::ANGRY));
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

void StudentWorld::addActor(Actor* a)
{
	m_actors.push_front(a);
}

int StudentWorld::getJewelCount()
{
	return jewel;
}
void StudentWorld::decrJewel()
{
	jewel--;
}

Player* StudentWorld::getPlayer()
{
	return m_player;
}

bool StudentWorld::census(Actor* a)
{
	int count = 0;
	int i = a->getX() - 3;
	int j = a->getX() + 3;
	int k = a->getY() - 3;
	int l = a->getY() + 3;
	if (i < 0)
		i = 0;
	if (k < 0)
		k = 0;
	if (j > 14)
		j = 14;
	if (l > 14)
		l = 14;
	for (int m = 0; i + m <= j; m++)
	{
		for (int n = 0; k + n <= l; n++)
		{
			for (std::list< Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
			{
				if ((*itr)->countsInFactoryCensus())
				{
					if (i + m == (*itr)->getX() && k + n == (*itr)->getY())
					{
						count++;
						
					}
				}
			}
		}
	}
	std::cerr << std::endl << count;
	if (count < 3)
	{
		return true;
	}
	return false;
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
	m_player->doSomething();
	for (std::list< Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); ++i)
	{
		(*i)->doSomething();
		if (!(m_player->isAlive()))
		{
			return GWSTATUS_PLAYER_DIED;
		}
		if ((*i)->getID() == IID_EXIT && isPlayerColocatedWith((*i)->getX(), (*i)->getY()) && (*i)->isVisible())
		{
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	for (std::list< Actor*>::iterator i = m_actors.begin(); i != m_actors.end();)
	{
		if ((*i)->isAlive() == false)
		{
			delete(*i);
			i = m_actors.erase(i);
		}
		else
			++i;
	}

	if (!(m_bonus == 0))
		m_bonus--;
	setDisplayText();

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	delete m_player;
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
		delete (*itr);
	m_actors.clear();
	jewel = 0;
}

bool StudentWorld::colocatedFactory(Actor *a)
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if ((*itr)->getID() == IID_ANGRY_KLEPTOBOT || (*itr)->getID() == IID_KLEPTOBOT)
		{
			if (a->getX() == (*itr)->getX() && a->getY() == (*itr)->getY())
			{
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::colocatedGoodie(Actor *a)
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if ((*itr)->isStealable())
		{
			if (a->getX() == (*itr)->getX() && a->getY() == (*itr)->getY())
			{
				if (rand() % 10 == 9)
				{
					if (a->getGoodie() == 0)
					{
						a->takeGoodie((*itr)->getID());
						cerr << (*itr)->getID();
						delete(*itr);
						itr = m_actors.erase(itr);
						return true;
					}
				}
				else
					return false;
			}
		}
	}
	return false;
}

bool StudentWorld::swallowSwallowable(Actor* a)
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if ((*itr)->isSwallowable())
		{
			if (a->getX() == (*itr)->getX() && a->getY() == (*itr)->getY())
			{
				(*itr)->setDead();
				return true;
			}
		}
	}
	return false;
}

bool StudentWorld::isPlayerColocatedWith(int x, int y) const
{
	if (m_player->getX() == x && m_player->getY() == y)
		return true;
	return false;
}

bool StudentWorld::ifActorBlocks(Actor* actor, int x, int y, int dx,int dy)
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if (actor->getID() == IID_PLAYER)
		{
			if (!((*itr)->allowsAgentColocation()))
			{
				if ((*itr)->getX() == x + dx && (*itr)->getY() == y + dy && (*itr)->getID() != IID_BOULDER)
				{
					return true;
				}
				if ((*itr)->getX() == x + dx && (*itr)->getY() == y + dy && (*itr)->getID() == IID_BOULDER)
				{
					if (canBoulderMoveTo((x + 2 * dx), (y + 2 * dy)))
					{
						(*itr)->moveTo((x + 2 * dx), (y + 2 * dy));
						return false;
					}
					else
						return true;
				}
			}
		}
		else
		{
			if (!((*itr)->allowsAgentColocation()))
			{
				if ((*itr)->getX() == x + dx && (*itr)->getY() == y + dy)
				{
					return true;
				}

			}
			if (isPlayerColocatedWith(actor->getX() + dx, actor->getY() + dy))
				return true;
		}

	}
	return false;
}

bool StudentWorld::BulletIsBlocked(Actor* a, int x, int y)
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if (a->getID() == IID_BULLET)
		{

			if ((*itr)->stopsBullet())
			{
				if ((*itr)->getX() == x && (*itr)->getY() == y)
				{
					if ((*itr)->isDamageable())
						(*itr)->damage();
					return true;
				}
			}
		}
		else
		{
			if ((*itr)->stopsBullet())
			{
				if ((*itr)->getX() == x && (*itr)->getY() == y)
				{
					return true;
				}
			}
			if (isPlayerColocatedWith(x, y))
				return false;
		}
	}
	return false;
}

bool StudentWorld::canBoulderMoveTo(int x, int y) 
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if (!((*itr)->allowsBoulderColocation()))
		{
			if ((*itr)->getX() == x && (*itr)->getY() == y)
			{
				return false;
			}
		}
	}
	return true;
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
	int health = m_player->getHealthPct();
	int ammo = m_player->getAmmo();
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
