#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;



// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor :public GraphObject
{
public:
	Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction startDirection = none);
	virtual ~Actor();
	virtual void doSomething();
	StudentWorld* getWorld();
	void setBlock();
	bool seeBlock();
	void setDead();
	bool seeDead();
private:
	StudentWorld* world;
	bool m_block;
	bool m_dead;
};

class Player :public Actor
{
public:
	Player(StudentWorld* sw, int startX, int startY);
	~Player();
	virtual void doSomething();
};

class Wall :public Actor
{
public:
	Wall(StudentWorld* sw, int startX, int startY, Direction startDirection = none);
	~Wall();
};

#endif // ACTOR_H_

