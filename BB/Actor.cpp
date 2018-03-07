#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <cstdlib>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Actor::Actor(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, Direction startDir)  //base class of all items
	:GraphObject(imageID, startX, startY, startDir), m_world(world), m_dead(false), m_hitPoints(hitPoints)			//derived from GraphObject
{
	setVisible(true);
}

Actor::~Actor()
{}

bool Actor::allowsAgentColocation() const		//check if agent (moving actor) can be on the same space as this actor
{
	return false;
}

void Actor::takeGoodie(int IID)					//take goodie (used by KleptoBot)
{}

bool Actor::allowsBoulderColocation() const		//check if boulder can be on the same space as this actor
{
	return false;
}


bool Actor::stopsBullet() const					//check if actor stops bullet
{
	return true;
}

bool Actor::isAlive() const						//check if actor is alive
{
	return !m_dead;
}

bool Actor::countsInFactoryCensus() const		//check if actor counts in factory census false except for KleptoBots
{
	return false;
}

bool Actor::isStealable() const					//check if actor is stealable false except for goodies
{
	return false;
}

void Actor::setDead()							//set actor to dead
{
	m_dead = true;
}

bool Actor::isSwallowable() const				//is swallowable by hole false except for boulder
{
	return false;
}

int Actor::getGoodie()							//access goodie stored for KleptoBot
{
	return 0;
}

void Actor::setHitPoints(int amt)				//set hit points of actor
{
	m_hitPoints = amt;
}

int Actor::getHitPoints() const					//return hit points of actor
{
	return m_hitPoints;
}

bool Actor::isDamageable() const				//check if actor is damageable 
{
	return true;
}

void Actor::damage()							//damage actor if bullet hits damageable actor
{
	m_hitPoints -= 2;
	tryToBeKilled();
}

void Actor::tryToBeKilled()						//kill actor if hit point is zero called when actor is damaged
{
	if (m_hitPoints == 0)
	{
		setDead();
	}
}

StudentWorld* Actor::getWorld() const			//return pointer to StudentWorld object exists in
{
	return m_world;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//AGENT
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Agent::Agent(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, Direction startDir) //base class moving actors
	:Actor(world, startX, startY, imageID, hitPoints, startDir)
{}

Agent::~Agent()
{}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//PLAYER
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Player::Player(StudentWorld* world, int startX, int startY)						//construct player
	:Agent(world, startX, startY, IID_PLAYER, 20, right), p_ammo(20)
{}

Player::~Player()
{}

unsigned int Player::getHealthPct() const										//return health percent
{
	return (getHitPoints() * 5);
}

unsigned int Player::getAmmo() const											//return ammo count of plsyer
{
	return p_ammo;
}

void Player::increaseAmmo()														//increase ammo called if ammo goodie picked up
{ 
	p_ammo += 20;
}

void Player::tryToBeKilled()													//kill player if hit points are zero
{																				//decrease number of lives and play player died sound effect
	if (getHitPoints() == 0)													//else play sound impact
	{
		getWorld()->playSound(SOUND_PLAYER_DIE);
		getWorld()->decLives();
		setDead();
	}
	else
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
}

void Player::doSomething()														//player doSomething
{
	if (!isAlive())																//return doing nothing if player is dead
		return;
	int ch;
	if (getWorld()->getKey(ch))
	{
	// user hit a key this tick!
		switch (ch)
		{																		//switch based on player input
		case KEY_PRESS_LEFT:													//direction pressed set direction of player and move in the direction if possible
			setDirection(left);
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), -1, 0)))
			{
				moveTo(getX() - 1, getY());
			}
			break;
		case KEY_PRESS_RIGHT:
			setDirection(right);
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 1, 0)))
			{
				moveTo(getX() + 1, getY());
			}
			break;
		case KEY_PRESS_UP:
			setDirection(up);
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 0, 1)))
			{
				moveTo(getX(), getY() + 1);
			}
			break;
		case KEY_PRESS_DOWN:
			setDirection(down);
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 0, -1)))
			{
				moveTo(getX(), getY() - 1);
			}
			break;
		case KEY_PRESS_ESCAPE:													//escape pressed reduces lives and reset level handled by StudentWorld
			getWorld()->decLives();
			setDead();
			break;
		case KEY_PRESS_SPACE:													//space pressed fire bullet in direction player facing if player has ammo
			if (p_ammo <= 0)
				break;
			getWorld()->playSound(SOUND_PLAYER_FIRE);
			p_ammo--;
			int dir = getDirection();
			switch (dir)
			{
			case up:
				getWorld()->addActor(new Bullet(getWorld(), getX(), getY() + 1, up));
				break;
			case down:
				getWorld()->addActor(new Bullet(getWorld(), getX(), getY() - 1, down));
				break;
			case right:
				getWorld()->addActor(new Bullet(getWorld(), getX() + 1, getY(), right));
				break;
			case left:
				getWorld()->addActor(new Bullet(getWorld(), getX() - 1, getY(), left));
				break;
			}
			break;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//ROBOT (ENEMIES)
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Robot::Robot(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score, Direction startDir)  
	:Agent(world, startX, startY, imageID, hitPoints, startDir), currTick(1) //current tick set to 1
{}

Robot::~Robot()
{}


int Robot::getCurrTick()											//return current tick used to check if enemy should do something
{
	return currTick;
}

void Robot::incrTick()												//icrease current tick
{
	currTick++;
}

void Robot::resetTick()												//reset tick to 1
{
	currTick = 1;
}


bool Robot::ClearShot()												//check if robot has clear shot of player
{
	bool clear = false;
	if (getY() == getWorld()->getPlayer()->getY() && getDirection() == right)		//if robot is in the same row/column and facing the player
	{																				//check every square inbetween robot and player		
		for (int i = 1; getX() + i <= getWorld()->getPlayer()->getX(); i++)			//check if bullet is blocked handled by StudentWorld
		{																			//return true if bullet will reach player else return false
			if (!(getWorld()->BulletIsBlocked(this, getX() + i, getY())))			
				clear = true;
			else
				return false;
		}
		return clear;
	}
	if (getY() == getWorld()->getPlayer()->getY() && getDirection() == left)
	{
		for (int i = -1; getX() + i >= getWorld()->getPlayer()->getX(); i--)
		{
			if (!(getWorld()->BulletIsBlocked(this, getX() + i, getY())))
				clear = true;
			else
				return false;
		}
		return clear;
	}
	if (getX() == getWorld()->getPlayer()->getX() && getDirection() == up)
	{
		for (int i = 1; getY() + i <= getWorld()->getPlayer()->getY(); i++)
		{
			if (!(getWorld()->BulletIsBlocked(this, getX(), getY() + i)))
				clear = true;
			else
				return false;
		}
		return clear;
	}
	if (getX() == getWorld()->getPlayer()->getX() && getDirection() == down)
	{
		for (int i = -1; getY() + i >= getWorld()->getPlayer()->getY(); i--)
		{
			if (!(getWorld()->BulletIsBlocked(this, getX(), getY() + i)))
				clear = true;
			else
				return false;
		}
		return clear;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//SNARLBOT
/////////////////////////////////////////////////////////////////////////////////////////////////////////

SnarlBot::SnarlBot(StudentWorld* world, int startX, int startY, Direction startDir)		//create SnarlBot
	:Robot(world, startX, startY, IID_SNARLBOT, 10, 100, startDir)
{}

SnarlBot::~SnarlBot()
{}

void SnarlBot::tryToBeKilled()										//kill SnarlBot if hit points is zero play robot death sound and increase score by 100
{																	//else play robot impact
	if (getHitPoints() == 0)
	{
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(100);
		setDead();
	}
	else
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
}

void SnarlBot::doSomething()											//SnarlBot doSomething
{
	if (!isAlive())														//return without doing anything if SnarlBot is dead
		return;
	int ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current           //calculate how often SnarlBot should do something
	// level number (0, 1, 2, etc.)
	if (ticks < 3)
		ticks = 3; // no SnarlBot moves more frequently than this
	if (getCurrTick() != ticks)											//if SnarlBot object tick member not equal to ticks do nothing and increase tick
	{
		incrTick();
		return;
	}
	if (getCurrTick() == ticks)										//if tick data member equal to ticks check SnarlBot direction
	{

		int dir = getDirection();
		switch (dir)
		{
		case left:																		//if robot has a clear shot at the player in the direction it is facing
			if (ClearShot())															//fire a bullet and reset tick data member
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX() - 1, getY(), left));
				resetTick();
				return;
			}
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), -1, 0)))				//otherwise move one step in direction SnarlBot is facing if possible
			{
				moveTo(getX() - 1, getY());
			}																			//otherwise switch direction of bot to opposite direction
			else
				setDirection(right);
			break;
		case right:
			if (ClearShot())
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX() + 1, getY(), right));
				resetTick();
				return;
			}
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 1, 0)))
			{
				moveTo(getX() + 1, getY());
			}
			else
				setDirection(left);
			break;
		case up:
			if (ClearShot())
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX(), getY() + 1, up));
				resetTick();
				return;
			}
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 0, 1)))
			{
				moveTo(getX(), getY() + 1);
			}
			else
				setDirection(down);
			break;
		case down:
			if (ClearShot())
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX(), getY() - 1, down));
				resetTick();
				return;
			}
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 0, -1)))
			{
				moveTo(getX(), getY() - 1);
			}
			else
				setDirection(up);
			break;
		}
		
		resetTick();
	}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////


//KLEPTOBOT
/////////////////////////////////////////////////////////////////////////////////////////////////////////

KleptoBot::KleptoBot(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score) //construct kleptobot
	:Robot(world, startX, startY, imageID, hitPoints, 10, right), stepsInDirection(0), d(right), m_goodie(0) 
{																//steps in direction set to zero set direction to right set goodie held by KleptoBot to emtpy
	distanceBeforeTurning = (rand() % 6 + 1); //random value for number of steps in directon between 1 to 6
}

KleptoBot::~KleptoBot()
{}

int KleptoBot::getGoodie()		//return goodie KleptoBot is holding
{
	return m_goodie;
}

void KleptoBot::takeGoodie(const int IID)		//take goodie if applicable which is handeled by StudentWorld
{
	m_goodie = IID;
}

void KleptoBot::doSomething()					//KleptoBot doSomething
{
	if (!isAlive())		
		return;
	int ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current     
	// level number (0, 1, 2, etc.)
	if (ticks < 3)
		ticks = 3; // no SnarlBot moves more frequently than this
	if (getCurrTick() != ticks)
	{
		incrTick();
		return;
	}
	if (getCurrTick() == ticks)								//if KleptoBot is taking an action this tick
	{
		if (getWorld()->colocatedGoodie(this))				//if KleptoBot steals goodie play munch sound and reset data member tick and do nothing else
		{
			std::cerr << m_goodie;
			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			resetTick();
			std::cerr << m_goodie;
			return;
		}

		else if (stepsInDirection != distanceBeforeTurning)  // if the number of steps the KleptoBot has taken in the direction it is facing is less than the random distance before turning data member
		{
			std::cerr << std::endl << stepsInDirection << " " << distanceBeforeTurning << std::endl;
			switch (d)
			{																	//get direction of the bot and if the bot can move in the direction move one step and increment steps in direction reset data member tick and return
			case left:															//otherwise call function ifBlockedOrDistance
				setDirection(left);
				if (!(getWorld()->ifActorBlocks(this, getX(), getY(), -1, 0)))
				{
					
					moveTo(getX() - 1, getY());
					stepsInDirection++;
					resetTick();
					return;
				}
				else
				{
					ifBlockedOrDistance();
					return;
				}
				break;
			case right:
				setDirection(right);
				if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 1, 0)))
				{
					
					moveTo(getX() + 1, getY());
					stepsInDirection++;
					resetTick();
					return;
				}
				else
				{
					ifBlockedOrDistance();
					return;
				}
				break;
			case up:
				setDirection(up);
				if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 0, 1)))
				{
					
					moveTo(getX(), getY() + 1);
					stepsInDirection++;
					resetTick();
					return;
				}
				else
				{
					ifBlockedOrDistance();
					return;
				}
				break;
			case down:
				setDirection(down);
				if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 0, -1)))
				{
					
					moveTo(getX(), getY() - 1);
					stepsInDirection++;
					resetTick();
					return;
				}
				else
				{
					ifBlockedOrDistance();
					return;
				}
				break;
			}

		}
		
		else if (stepsInDirection == distanceBeforeTurning)		//if steps in direction equal to distance before turning call ifBlockedOrDistance
		{
			ifBlockedOrDistance();
		}
	}
}

void KleptoBot::ifBlockedOrDistance()
{
	stepsInDirection = 0;							//reset steps in direction pick a random direction and random distance before turning
	std::cerr << "called";
	distanceBeforeTurning = (rand() % 6 + 1);
	int k = (rand() % 4);						
	switch (k)
	{
	case 0:
		d = up;
		break;
	case 1:
		d = down;
		break;
	case 2:
		d = left;
		break;
	case 3:
		d = right;
		break;
	}

	std::list<Direction> direction = { up, down, left, right }; //list of possible directions

	while (!direction.empty())									//while list is not empty
	{
		if (d == up)
		{
			setDirection(up);
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 0, 1))) //check if bot can move in direction
			{
				moveTo(getX(), getY() + 1);
				stepsInDirection++;
				resetTick();
				std::cerr << "finished";
				return;
			}															//otherwise remove direction from list of possibilities and check the remaining directions
			else
			{
				direction.remove(up);
				if (!direction.empty())
					d = direction.front();
				else
				{
					std::cerr << "finished";
					return;
				}
			}
		}
		if (d == down)
		{
			setDirection(down);
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 0, -1)))
			{
				moveTo(getX(), getY() - 1);
				stepsInDirection++;
				resetTick();
				std::cerr << "finished";
				return;
			}
			else
			{
				direction.remove(down);
				if (!direction.empty())
					d = direction.front();
				else
				{
					std::cerr << "finished";
					return;
				}
			}
		}
		if (d == left)
		{
			setDirection(left);
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), -1, 0)))
			{
				moveTo(getX() - 1, getY());
				stepsInDirection++;
				resetTick();
				std::cerr << "finished";
				return;
			}
			else
			{
				direction.remove(left);
				if (!direction.empty())
					d = direction.front();
				else
				{
					std::cerr << "finished";
					return;
				}
			}
		}
		if (d == right)
		{
			setDirection(right);
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 1, 0)))
			{
				moveTo(getX() + 1, getY());
				stepsInDirection++;
				resetTick();
				std::cerr << "finished";
				return;
			}
			else
			{
				direction.remove(right);
				if (!direction.empty())
					d = direction.front();
				else
				{
					std::cerr << "finished";
					return;
				}
			}
		}
	}
	std::cerr << "finished";
	return;												//if bot blocked in all directions return with what direction is set to
}

void KleptoBot::tryToBeKilled()
{
	std::cerr << getHitPoints() << std::endl;
	if (getHitPoints() <= 0)
	{																//set bot to dead if hit points less than zero
		getWorld()->playSound(SOUND_ROBOT_DIE);						//play death sound ansd increase score by
		if(getID()== IID_KLEPTOBOT)
			getWorld()->increaseScore(10);
		if (getID() == IID_ANGRY_KLEPTOBOT)
			getWorld()->increaseScore(20);
		setDead();
		if (m_goodie == IID_AMMO)										//if bot had a goodie create new goodie of that type at the position bot died
		{
			getWorld()->addActor(new AmmoGoodie(getWorld(), getX(), getY()));
		}
		if (m_goodie == IID_EXTRA_LIFE)
		{
			getWorld()->addActor(new ExtraLifeGoodie(getWorld(), getX(), getY()));
		}
		if (m_goodie == IID_RESTORE_HEALTH)
		{
			getWorld()->addActor(new RestoreHealthGoodie(getWorld(), getX(), getY()));
		}
	}
	else
		getWorld()->playSound(SOUND_ROBOT_IMPACT); //otherise play impact sound
}

bool KleptoBot::countsInFactoryCensus() const  //counts in factory census
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//ANGRY
/////////////////////////////////////////////////////////////////////////////////////////////////////////

AngryKleptoBot::AngryKleptoBot(StudentWorld* world, int startX, int startY)
	:KleptoBot(world, startX, startY, IID_ANGRY_KLEPTOBOT, 8, 20)
{}

AngryKleptoBot::~AngryKleptoBot()
{}

void AngryKleptoBot::doSomething()  //AngryKleptoBot doSomething
{
	if (!isAlive())
		return;
	int ticks = (28 - getWorld()->getLevel()) / 4; // levelNumber is the current
	// level number (0, 1, 2, etc.)
	if (ticks < 3)
		ticks = 3; // no SnarlBot moves more frequently than this
	if (getCurrTick() != ticks)
	{
		incrTick();
		return;
	}
	if (getCurrTick() == ticks)					//if taking an action
	{

		int dir = getDirection();
		switch (dir)
		{
		case left:							//check direction and check if bot can shoot player
			if (ClearShot())
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX() - 1, getY(), left)); //fire bullet at player and reset tick data memeber and do nothing else
				resetTick();
				return;
			}
			break;
		case right:
			if (ClearShot())
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX() + 1, getY(), right));
				resetTick();
				return;
			}
			break;
		case up:
			if (ClearShot())
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX(), getY() + 1, up));
				resetTick();
				return;
			}
			break;
		case down:
			if (ClearShot())
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX(), getY() - 1, down));
				resetTick();
				return;
			}
			break;
		}
		KleptoBot::doSomething();		//if not firing do KleptoBot doSomething
		resetTick();
	}
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//FACTORY
/////////////////////////////////////////////////////////////////////////////////////////////////////////

KleptoBotFactory::KleptoBotFactory(StudentWorld* world, int startX, int startY, ProductType type) //create factory
	:Actor(world, startX, startY, IID_ROBOT_FACTORY, 100, none), pType(type) //store factory type
{}

KleptoBotFactory::~KleptoBotFactory()
{}

void KleptoBotFactory::doSomething()
{
	if (getWorld()->census(this) && !(getWorld()->colocatedFactory(this))) //check if there are not three kleptobots in the area handled by StudentWorld and there is not a kleptobot on the factory
	{
		if (rand() % 50 == 9)  //1/50 chance to create new KleptoBot
		{
			if (pType == ANGRY)
			{
				getWorld()->addActor(new AngryKleptoBot(getWorld(), getX(), getY()));
				getWorld()->playSound(SOUND_ROBOT_BORN);
				return;
			}
			if (pType == REGULAR)
			{
				getWorld()->addActor(new KleptoBot(getWorld(), getX(), getY(), IID_KLEPTOBOT, 5, 10));
				getWorld()->playSound(SOUND_ROBOT_BORN);
				return;
			}
		}
		else
			return;
	}
	else 
		return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////

//BULLET
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Bullet::Bullet(StudentWorld* world, int startX, int startY, Direction startDir) //create bullet
	:Actor(world, startX, startY, IID_BULLET, 100, startDir)
{}

Bullet::~Bullet()
{}

void Bullet::doSomething() //bullet doSomething
{
	if (!isAlive())
		return;
	if (getWorld()->BulletIsBlocked(this, getX(), getY()))  //If bullet is blocked handled by StudentWorld set bullet to dead
	{
		setDead();
		return;
	}
	if (getWorld()->isPlayerColocatedWith(getX(), getY())) //If player and bullet in same spot player takes damage and bullet dies
	{
		getWorld()->getPlayer()->damage();
		setDead();
		return;
	}
	int dir = getDirection();  //otherwise get direction of bullet and move it one space in that direction
	switch (dir)
	{
	case up:
		moveTo(getX(), getY() + 1);
		break;
	case down:
		moveTo(getX(), getY() - 1);
		break;
	case right:
		moveTo(getX() + 1, getY());
		break;
	case left:
		moveTo(getX() - 1, getY());
		break;
	}
	if (getWorld()->BulletIsBlocked(this, getX(), getY()))  //check if bullet is blocked or hit the player at the new location
	{
		setDead();
		return;
	}
	if (getWorld()->isPlayerColocatedWith(getX(), getY()))
	{
		getWorld()->getPlayer()->damage();
		setDead();
		return;
	}
}

bool Bullet::allowsAgentColocation() const //bullet can be in the same space as other actor 
{
	return true;
}

bool Bullet::stopsBullet() const //bullet does not stop another bullet
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//WALL
/////////////////////////////////////////////////////////////////////////////////////////////////////////	

Wall::Wall(StudentWorld* world, int startX, int startY) //create wall
	:Actor(world, startX, startY, IID_WALL, 100, none)
{}

Wall::~Wall()
{}

void Wall::doSomething() //walls do nothing
{}

bool Wall::isDamageable() const //walls cannot be damaged by bullets
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//BOULDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Boulder::Boulder(StudentWorld* world, int startX, int startY) //create boulder
	:Actor(world, startX, startY, IID_BOULDER, 10, none)
{}

Boulder::~Boulder()
{}

void Boulder::doSomething() //boulder does nothing
{}

bool Boulder::isSwallowable() const //will be swallowed by hole
{
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Hole
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Hole::Hole(StudentWorld* world, int startX, int startY) //create hole
	:Actor(world, startX, startY, IID_HOLE, 10, none)
{}

Hole::~Hole()
{}

void Hole::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->swallowSwallowable(this)) //if hole swallows boulder handled by StudentWorld then set dead
	{
		setDead();
	}
}

bool Hole::allowsBoulderColocation() const
{
	return true;
}

bool Hole::stopsBullet() const
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//EXIT
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Exit::Exit(StudentWorld* world, int startX, int startY) //create exit
	:Actor(world, startX, startY, IID_EXIT, 100, none)
{
	setVisible(false); //exit not visible at beginning
}

Exit::~Exit()
{}

void Exit::doSomething() //exit doSomething
{
	if (getWorld()->getJewelCount() == 0) //if no jewels left make exit visible
		setVisible(true);				  
	if (getWorld()->isPlayerColocatedWith(getX(), getY()) && isVisible()) //ifplayer reached visibe exit end level and award remaining bonus and 2000
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(2000);
		getWorld()->increaseScore(getWorld()->getCurrentLevelBonus());
	}

}
bool Exit::allowsAgentColocation() const //agents can walk over exit
{
	return true;
}

bool Exit::stopsBullet() const			//does not stop bullets
{
	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ITEM
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID, unsigned int score) //base class for pickupable items
	:Actor(world, startX, startY, imageID, 100, none), m_pscore(score)
{}

PickupableItem::~PickupableItem()
{}


bool PickupableItem::allowsAgentColocation() const		//agents can step on pickupable items
{
	return true;
}

unsigned int PickupableItem::pScore()					//return score of pickupable item
{
	return m_pscore;
}

bool PickupableItem::stopsBullet() const				//items do not stop bullets
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//JEWEL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Jewel::Jewel(StudentWorld* world, int startX, int startY)
	:PickupableItem(world, startX, startY, IID_JEWEL, 50) //jewel worth 50 points
{}

Jewel::~Jewel()
{}

void Jewel::doSomething()			//jewel do something
{
	if (!isAlive())
		return;
	if (getWorld()->isPlayerColocatedWith(getX(), getY())) //if player on jewel pick it up increase score play sound  got goodie decrase number of jewels on map
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->decrJewel();
		getWorld()->increaseScore(pScore());
		setDead();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//GOODIE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID, unsigned int score) //base class for stealable goodies
	:PickupableItem(world, startX, startY, imageID, score)
{}

Goodie::~Goodie()
{}


bool Goodie::isStealable() const		//can be stolen by KleptoBots
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//EXTRA LIFE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY)
	:Goodie(world, startX, startY, IID_EXTRA_LIFE, 1000)
{}

ExtraLifeGoodie::~ExtraLifeGoodie()
{}

void ExtraLifeGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->isPlayerColocatedWith(getX(), getY()) && isVisible()) //if player on goodie pick it up increase score play sound got goodie increase lives
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(pScore());
		getWorld()->incLives();
		setDead();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//RESTORE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RestoreHealthGoodie::RestoreHealthGoodie(StudentWorld* world, int startX, int startY)
	:Goodie(world, startX, startY, IID_RESTORE_HEALTH, 500)
{}

RestoreHealthGoodie::~RestoreHealthGoodie()
{}

void RestoreHealthGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->isPlayerColocatedWith(getX(), getY()) && isVisible())  //if player on goodie pick it up increase score play sound got goodie restore life of player to full
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(pScore());
		getWorld()->getPlayer()->setHitPoints(20);
		setDead();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Ammo
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AmmoGoodie::AmmoGoodie(StudentWorld* world, int startX, int startY)
	:Goodie(world, startX, startY, IID_AMMO, 100)
{}

AmmoGoodie::~AmmoGoodie()
{}

void AmmoGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->isPlayerColocatedWith(getX(), getY()) && isVisible()) //if player on goodie pick it up increase score play sound got goodie increase ammo
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(pScore());
		getWorld()->getPlayer()->increaseAmmo();
		setDead();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
