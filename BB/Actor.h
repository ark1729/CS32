#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
class Agent;

//ACTOR
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, Direction startDir);

	virtual ~Actor();

	// Action to perform each tick
	virtual void doSomething() = 0;

	// Is this actor alive?
	bool isAlive() const;

	// Mark this actor as dead
	void setDead();

	virtual void takeGoodie(int IID);

	// Get this actor's world
	StudentWorld* getWorld() const;

	// Can an agent occupy the same square as this actor?
	virtual bool allowsAgentColocation() const;

	// Can a boulder occupy the same square as this actor?
	virtual bool allowsBoulderColocation() const;

	// Does this actor count when a factory counts items near it?
	virtual bool countsInFactoryCensus() const;

	// Does this actor stop bullets from continuing?
	virtual bool stopsBullet() const;

	// Can this actor be damaged by bullets?
	virtual bool isDamageable() const;

	// Cause this Actor to sustain damageAmt hit points of damage.
	virtual void damage();

	virtual int getGoodie();
	
	// Can this actor be swallowed by a hole?
	virtual bool isSwallowable() const;

	// Can this actor be picked up by a KleptoBot?
	virtual bool isStealable() const;

	// How many hit points does this actor have left?
	virtual int getHitPoints() const;

	// Set this actor's hit points to amt.
	virtual void setHitPoints(int amt);

	// Make the actor sustain damage.  Return true if this kills the
	// actor, and false otherwise.
	virtual void tryToBeKilled();

private:
	unsigned int m_hitPoints;
	bool m_dead;
	StudentWorld* m_world;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//AGENT (PLAYERS, ENEMIES)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Agent : public Actor
{
public:
	Agent(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, Direction startDir);

	virtual ~Agent();

	
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//PLAYER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Player : public Agent
{
public:
	Player(StudentWorld* world, int startX, int startY);
	virtual ~Player();
	virtual void doSomething();
	

	virtual void tryToBeKilled();
	// Get player's health percentage
	unsigned int getHealthPct() const;

	// Get player's amount of ammunition
	unsigned int getAmmo() const;

	// Restore player's health to the full amount.
	//void restoreHealth();

	// Increase player's amount of ammunition.
	void increaseAmmo();
private:
	unsigned int p_ammo;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ROBOT (ENEMIES)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Robot : public Agent
{
public:
	Robot(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score, Direction startDir);
	virtual ~Robot();
	virtual bool ClearShot();
	int getCurrTick();
	void incrTick();
	void resetTick();
private:
	int currTick;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//SNARLBOT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SnarlBot : public Robot
{
public:
	SnarlBot(StudentWorld* world, int startX, int startY, Direction startDir);
	virtual ~SnarlBot();
	virtual void doSomething();
	virtual void tryToBeKilled();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//KLEPTOBOT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class KleptoBot : public Robot
{
public:
	KleptoBot(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score);
	virtual ~KleptoBot();
	virtual void doSomething();
	virtual bool countsInFactoryCensus() const;
	virtual void takeGoodie(int IID);
	void ifBlockedOrDistance();
	virtual void tryToBeKilled();
	virtual int getGoodie();
private:
	int distanceBeforeTurning;
	int stepsInDirection;
	Direction d;
	int m_goodie;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//ANGRY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AngryKleptoBot : public KleptoBot
{
public:
	AngryKleptoBot(StudentWorld* world, int startX, int startY);
	virtual ~AngryKleptoBot();
	virtual void doSomething();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//EXIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Exit : public Actor
{
public:
	Exit(StudentWorld* world, int startX, int startY);
	virtual ~Exit();
	virtual void doSomething();
	virtual bool allowsAgentColocation() const;
	virtual bool stopsBullet() const;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//WALL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Wall : public Actor
{
public:
	Wall(StudentWorld* world, int startX, int startY);
	virtual ~Wall();
	virtual void doSomething();
	virtual bool isDamageable() const;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//BOULDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Boulder : public Actor
{
public:
	Boulder(StudentWorld* world, int startX, int startY);
	virtual ~Boulder();
	virtual void doSomething();
	virtual bool isSwallowable() const;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//HOLE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Hole : public Actor
{
public:
	Hole(StudentWorld* world, int startX, int startY);
	virtual ~Hole();
	virtual void doSomething();
	virtual bool allowsBoulderColocation() const;
	virtual bool stopsBullet() const;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Bullet : public Actor
{
public:
	Bullet(StudentWorld* world, int startX, int startY, Direction startDir);
	virtual ~Bullet();
	virtual void doSomething();
	virtual bool allowsAgentColocation() const;
	virtual bool stopsBullet() const;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//FACTORY
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class KleptoBotFactory : public Actor
{
public:
	enum ProductType { REGULAR, ANGRY };

	KleptoBotFactory(StudentWorld* world, int startX, int startY, ProductType type);
	virtual ~KleptoBotFactory();
	virtual void doSomething();
private:
	ProductType pType;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ITEM
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PickupableItem : public Actor
{
public:
	PickupableItem(StudentWorld* world, int startX, int startY, int imageID, unsigned int score);
	virtual ~PickupableItem();
	virtual bool allowsAgentColocation() const;
	unsigned int pScore();
	virtual bool stopsBullet() const;
private:
	unsigned int m_pscore;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//JEWEL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Jewel : public PickupableItem
{
public:
	Jewel(StudentWorld* world, int startX, int startY);
	virtual ~Jewel();
	virtual void doSomething();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//GOODIE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Goodie : public PickupableItem
{
public:
	Goodie(StudentWorld* world, int startX, int startY, int imageID, unsigned int score);
	virtual ~Goodie();
	virtual bool isStealable() const;


};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//EXTRA LIFE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ExtraLifeGoodie : public Goodie
{
public:
	
	ExtraLifeGoodie(StudentWorld* world, int startX, int startY);
	virtual ~ExtraLifeGoodie();
	virtual void doSomething();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//RESTORE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RestoreHealthGoodie : public Goodie
{
public:
	RestoreHealthGoodie(StudentWorld* world, int startX, int startY);
	virtual ~RestoreHealthGoodie();
	virtual void doSomething();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//AMMO
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class AmmoGoodie : public Goodie
{
public:
	AmmoGoodie(StudentWorld* world, int startX, int startY);
	virtual ~AmmoGoodie();
	virtual void doSomething();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // ACTOR_H_