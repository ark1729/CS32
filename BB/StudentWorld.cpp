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

StudentWorld::~StudentWorld() //destroy student world
{
	delete m_player;  //delete player object
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr) //delete all actor objects in the list
		delete (*itr);
	m_actors.clear();			//clear the list
}

int StudentWorld::loadLevelAndCreate()  //use level class to create level
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
		return GWSTATUS_PLAYER_WON;					//no more files victory
	}
	if (result == Level::load_fail_bad_format)
	{
		return GWSTATUS_LEVEL_ERROR;				//return level error load fail bad format
	}

	for (int i = 0; i < VIEW_WIDTH; i++)					//check every grid square and create actor of type at location
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
				jewel++;	//increase jewel count for each jewel created
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
	m_bonus = 1000;																 //set bonus to 1000
	return GWSTATUS_CONTINUE_GAME;				//return continue game
}

int StudentWorld::init()
{
	int result = loadLevelAndCreate();	//set result to return of level loading
	return result;						//return result 
}

void StudentWorld::addActor(Actor* a) //add new actor to actor list
{
	m_actors.push_front(a);
}

int StudentWorld::getJewelCount()		//return jewel count
{
	return jewel;
}
void StudentWorld::decrJewel()			//decrease jewel count 
{
	jewel--;
}

Player* StudentWorld::getPlayer()		//return pointer to player object in this StudentWorld
{
	return m_player;
}

bool StudentWorld::census(Actor* a)			//census around factory 
{
	int count = 0;
	int i = a->getX() - 3;
	int j = a->getX() + 3;
	int k = a->getY() - 3;
	int l = a->getY() + 3;
	if (i < 0) //keep in bounds of map
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
			for (std::list< Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)	//check actors in squares around factory which are within the square containing 3 grid points around the factory
			{
				if ((*itr)->countsInFactoryCensus())
				{
					if (i + m == (*itr)->getX() && k + n == (*itr)->getY())	//if kleptobot in the squares increase count
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
		return true; //if count is less than three factory can produce a new robot
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
		return GWSTATUS_PLAYER_DIED;		//check if player still has lives
	}
	m_player->doSomething();		//player does something
	for (std::list< Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); ++i)		//all actors do something
	{
		(*i)->doSomething();
		if (!(m_player->isAlive()))
		{
			return GWSTATUS_PLAYER_DIED;	//if the player died due to anction of the return player died
		}
		if ((*i)->getID() == IID_EXIT && isPlayerColocatedWith((*i)->getX(), (*i)->getY()) && (*i)->isVisible())	//if playeron a visible exit return level finished
		{
			return GWSTATUS_FINISHED_LEVEL;
		}
	}

	for (std::list< Actor*>::iterator i = m_actors.begin(); i != m_actors.end();) //delete dead actors
	{
		if ((*i)->isAlive() == false)
		{
			delete(*i);
			i = m_actors.erase(i);
		}
		else
			++i;
	}

	if (!(m_bonus == 0))	//stop bonus value at zero
		m_bonus--;
	setDisplayText();

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()		//if level is reset clears actors and player and resets jewel count
{
	delete m_player;
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
		delete (*itr);
	m_actors.clear();
	jewel = 0;
}
bool StudentWorld::colocatedFactory(Actor *a)  // if factory has a bot on it return true 
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

bool StudentWorld::colocatedGoodie(Actor *a)  //if a kleptobot is on a goodie
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if ((*itr)->isStealable())  //check if goodie is stealable
		{
			if (a->getX() == (*itr)->getX() && a->getY() == (*itr)->getY())
			{
				if (rand() % 10 == 9) //1/10 chance to stealit
				{
					if (a->getGoodie() == 0)
					{
						a->takeGoodie((*itr)->getID());		//sets kleptobots goodie to full with stolen goodie using function defined in kleptobot class
						cerr << (*itr)->getID();
						delete(*itr);					//deletes the goodie will be reformed by kleptobot function
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

bool StudentWorld::swallowSwallowable(Actor* a)  //swallows boulders on hole that called function as boulder is only swallowable object
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

bool StudentWorld::isPlayerColocatedWith(int x, int y) const //check if the player is colocated with position x,y
{
	if (m_player->getX() == x && m_player->getY() == y)
		return true;
	return false;
}

bool StudentWorld::ifActorBlocks(Actor* actor, int x, int y, int dx,int dy)			//checks if agent can move to square
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if (actor->getID() == IID_PLAYER)			//if the agent is the player
		{
			if (!((*itr)->allowsAgentColocation()))
			{
				if ((*itr)->getX() == x + dx && (*itr)->getY() == y + dy && (*itr)->getID() != IID_BOULDER) 
				{
					return true;
				}
				if ((*itr)->getX() == x + dx && (*itr)->getY() == y + dy && (*itr)->getID() == IID_BOULDER) //check if the boulder can move
				{
					if (canBoulderMoveTo((x + 2 * dx), (y + 2 * dy))) 
					{
						(*itr)->moveTo((x + 2 * dx), (y + 2 * dy));
						return false;									//then player can move to that spot pushing the boulder as well
					}
					else
						return true;
				}
			}
		}
		else                                         //for all other agents just check if the objects allows agent colocation return true if it does
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

bool StudentWorld::BulletIsBlocked(Actor* a, int x, int y)		//check if bullet is blocked at position x,y
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if (a->getID() == IID_BULLET)								//if bullet calls the function then damage is dealt if true
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
		else                                            //robot calls the function for clear path then no damge is dealt if true
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

bool StudentWorld::canBoulderMoveTo(int x, int y) //can boulder move to new position x,y
{
	for (list<Actor*>::iterator itr = m_actors.begin(); itr != m_actors.end(); ++itr)
	{
		if (!((*itr)->allowsBoulderColocation()))			//check if boulder can colocate position
		{
			if ((*itr)->getX() == x && (*itr)->getY() == y)
			{
				return false;
			}
		}
	}
	return true;
}


int StudentWorld::getCurrentLevelBonus()			//return the current level bonus
{
	return m_bonus;
}

void StudentWorld::setDisplayText()		//reset display text every tick
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

string Format(int score, int level, int livesLeft, int health, int ammo, int bonus)     //format display text 
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
