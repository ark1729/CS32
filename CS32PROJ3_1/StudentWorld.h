#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	~StudentWorld();
	int loadLevelAndCreate();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool ifActorBlocks(int x, int y);
	void setDisplayText();
	int getCurrentLevelBonus();
private:
	std::vector<Actor*> m_actors;
	int m_bonus;
};

std::string Format(int score, int level, int livesLeft, int health, int ammo, int bonus);

#endif // STUDENTWORLD_H_


