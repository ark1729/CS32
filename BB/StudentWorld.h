#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <list>

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
	bool ifActorBlocks(Actor* actor, int x, int y, int dx, int dy);
	// Can a boulder move to x,y?
	bool canBoulderMoveTo(int x, int y);
	bool BulletIsBlocked(Actor* a, int x, int y);
	bool isPlayerColocatedWith(int x, int y) const;
	void setDisplayText();
	int getCurrentLevelBonus();
	bool swallowSwallowable(Actor* a);
	int getJewelCount();
	void decrJewel();
	Player* getPlayer();
	void addActor(Actor* a);
	bool colocatedGoodie(Actor *a);
	bool colocatedFactory(Actor *a);
	bool census(Actor* a);
private:
	std::list<Actor*> m_actors;
	Player* m_player;
	int m_bonus;
	int jewel;
};

std::string Format(int score, int level, int livesLeft, int health, int ammo, int bonus);

#endif // STUDENTWORLD_H_


