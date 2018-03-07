#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <cstdlib>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//ACTOR
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Actor::Actor(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, Direction startDir)
	:GraphObject(imageID, startX, startY, startDir), m_world(world), m_dead(false), m_hitPoints(hitPoints)
{
	setVisible(true);
}

Actor::~Actor()
{}

bool Actor::allowsAgentColocation() const
{
	return false;
}

void Actor::takeGoodie(int IID)
{}

bool Actor::allowsBoulderColocation() const
{
	return false;
}

bool Actor::bePushedBy(Agent* a, int x, int y)
{
	return false;
}

bool Actor::stopsBullet() const
{
	return true;
}

bool Actor::isAlive() const
{
	return !m_dead;
}

bool Actor::countsInFactoryCensus() const
{
	return false;
}

bool Actor::isStealable() const
{
	return false;
}

void Actor::setDead()
{
	m_dead = true;
}

bool Actor::isSwallowable() const
{
	return false;
}

int Actor::getGoodie()
{
	return 0;
}

void Actor::setHitPoints(int amt)
{
	m_hitPoints = amt;
}

int Actor::getHitPoints() const
{
	return m_hitPoints;
}

bool Actor::isDamageable() const
{
	return true;
}

void Actor::damage()
{
	m_hitPoints -= 2;
	tryToBeKilled();
}

void Actor::tryToBeKilled()
{
	if (m_hitPoints == 0)
	{
		setDead();
	}
}

StudentWorld* Actor::getWorld() const
{
	return m_world;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//AGENT
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Agent::Agent(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, Direction startDir)
	:Actor(world, startX, startY, imageID, hitPoints, startDir)
{}

Agent::~Agent()
{}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//PLAYER
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Player::Player(StudentWorld* world, int startX, int startY)
	:Agent(world, startX, startY, IID_PLAYER, 20, right), p_ammo(20)
{}

Player::~Player()
{}

unsigned int Player::getHealthPct() const
{
	return (getHitPoints() * 5);
}

unsigned int Player::getAmmo() const
{
	return p_ammo;
}

void Player::increaseAmmo()
{
	p_ammo += 20;
}

void Player::tryToBeKilled()
{
	if (getHitPoints() == 0)
	{
		getWorld()->playSound(SOUND_PLAYER_DIE);
		getWorld()->decLives();
		setDead();
	}
	else
		getWorld()->playSound(SOUND_PLAYER_IMPACT);
}

void Player::doSomething()
{
	if (!isAlive())
		return;
	int ch;
	if (getWorld()->getKey(ch))
	{
	// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
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
		case KEY_PRESS_ESCAPE:
			getWorld()->decLives();
			setDead();
			break;
		case KEY_PRESS_SPACE:
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
	:Agent(world, startX, startY, imageID, hitPoints, startDir), currTick(1)
{}

Robot::~Robot()
{}

void Robot::doSomething() const
{}

bool Robot::isShootingRobot() const
{
	return true;
}

int Robot::getCurrTick()
{
	return currTick;
}

void Robot::incrTick()
{
	currTick++;
}

void Robot::resetTick()
{
	currTick = 1;
}


bool Robot::ClearShot()
{
	bool clear = false;
	if (getY() == getWorld()->getPlayer()->getY() && getDirection() == right)
	{
		for (int i = 1; getX() + i <= getWorld()->getPlayer()->getX(); i++)
		{
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

SnarlBot::SnarlBot(StudentWorld* world, int startX, int startY, Direction startDir)
	:Robot(world, startX, startY, IID_SNARLBOT, 10, 100, startDir)
{}

SnarlBot::~SnarlBot()
{}

void SnarlBot::tryToBeKilled()
{
	if (getHitPoints() == 0)
	{
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(100);
		setDead();
	}
	else
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
}

void SnarlBot::doSomething()
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
	if (getCurrTick() == ticks)
	{

		int dir = getDirection();
		switch (dir)
		{
		case left:
			if (ClearShot())
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX() - 1, getY(), left));
				resetTick();
				return;
			}
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), -1, 0)))
			{
				moveTo(getX() - 1, getY());
			}
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

KleptoBot::KleptoBot(StudentWorld* world, int startX, int startY, int imageID, unsigned int hitPoints, unsigned int score)
	:Robot(world, startX, startY, imageID, hitPoints, 10, right), stepsInDirection(0), d(right), m_goodie(0)
{
	distanceBeforeTurning = (rand() % 5 + 1);
}

KleptoBot::~KleptoBot()
{}

int KleptoBot::getGoodie()
{
	return m_goodie;
}

void KleptoBot::takeGoodie(const int IID)
{
	m_goodie = IID;
}

void KleptoBot::doSomething()
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
	if (getCurrTick() == ticks)
	{
		if (getWorld()->colocatedGoodie(this))
		{
			std::cerr << m_goodie;
			getWorld()->playSound(SOUND_ROBOT_MUNCH);
			resetTick();
			std::cerr << m_goodie;
			return;
		}

		else if (stepsInDirection != distanceBeforeTurning)
		{
			std::cerr << std::endl << stepsInDirection << " " << distanceBeforeTurning << std::endl;
			switch (d)
			{
			case left:
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
		
		else if (stepsInDirection == distanceBeforeTurning)
		{
			ifBlockedOrDistance();
		}
	}
}

void KleptoBot::ifBlockedOrDistance()
{
	stepsInDirection = 0;
	std::cerr << "called";
	distanceBeforeTurning = (rand() % 5 + 1);
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

	std::list<Direction> direction = { up, down, left, right };

	while (!direction.empty())
	{
		if (d == up)
		{
			setDirection(up);
			if (!(getWorld()->ifActorBlocks(this, getX(), getY(), 0, 1)))
			{
				moveTo(getX(), getY() + 1);
				stepsInDirection++;
				resetTick();
				std::cerr << "finished";
				return;
			}
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
	return;
}

void KleptoBot::tryToBeKilled()
{
	std::cerr << getHitPoints() << std::endl;
	if (getHitPoints() <= 0)
	{
		getWorld()->playSound(SOUND_ROBOT_DIE);
		getWorld()->increaseScore(10);
		setDead();
		if (m_goodie == IID_AMMO)
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
		getWorld()->playSound(SOUND_ROBOT_IMPACT);
}

bool KleptoBot::countsInFactoryCensus() const
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

void AngryKleptoBot::doSomething()
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
	if (getCurrTick() == ticks)
	{

		int dir = getDirection();
		switch (dir)
		{
		case left:
			if (ClearShot())
			{
				getWorld()->playSound(SOUND_ENEMY_FIRE);
				getWorld()->addActor(new Bullet(getWorld(), getX() - 1, getY(), left));
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
		KleptoBot::doSomething();
		resetTick();
	}
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//FACTORY
/////////////////////////////////////////////////////////////////////////////////////////////////////////

KleptoBotFactory::KleptoBotFactory(StudentWorld* world, int startX, int startY, ProductType type)
	:Actor(world, startX, startY, IID_ROBOT_FACTORY, 100, none), pType(type)
{}

KleptoBotFactory::~KleptoBotFactory()
{}

void KleptoBotFactory::doSomething()
{
	if (getWorld()->census(this) && !(getWorld()->colocatedFactory(this)))
	{
		if (rand() % 50 == 9)
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

Bullet::Bullet(StudentWorld* world, int startX, int startY, Direction startDir)
	:Actor(world, startX, startY, IID_BULLET, 100, startDir)
{}

void Bullet::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->BulletIsBlocked(this, getX(), getY()))
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
	int dir = getDirection();
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
	if (getWorld()->BulletIsBlocked(this, getX(), getY()))
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

bool Bullet::allowsAgentColocation() const
{
	return true;
}

bool Bullet::stopsBullet() const
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//WALL
/////////////////////////////////////////////////////////////////////////////////////////////////////////	

Wall::Wall(StudentWorld* world, int startX, int startY)
	:Actor(world, startX, startY, IID_WALL, 100, none)
{}

Wall::~Wall()
{}

void Wall::doSomething()
{}

bool Wall::isDamageable() const
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

//BOULDER
/////////////////////////////////////////////////////////////////////////////////////////////////////////

Boulder::Boulder(StudentWorld* world, int startX, int startY)
	:Actor(world, startX, startY, IID_BOULDER, 10, none)
{}

void Boulder::doSomething()
{}

bool Boulder::isSwallowable() const
{
	return true;
}

bool Boulder::bePushedBy(Agent* a, int x, int y)
{
	if (a->getID() == IID_PLAYER)
	{
		if (getWorld()->canBoulderMoveTo(x, y))
		{
			return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Hole
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Hole::Hole(StudentWorld* world, int startX, int startY)
	:Actor(world, startX, startY, IID_HOLE, 10, none)
{}

void Hole::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->swallowSwallowable(this))
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

Exit::Exit(StudentWorld* world, int startX, int startY)
	:Actor(world, startX, startY, IID_EXIT, 100, none)
{
	setVisible(false);
}

void Exit::doSomething()
{
	if (getWorld()->getJewelCount() == 0)
		setVisible(true);
	if (getWorld()->isPlayerColocatedWith(getX(), getY()) && isVisible())
	{
		getWorld()->playSound(SOUND_FINISHED_LEVEL);
		getWorld()->increaseScore(2000);
		getWorld()->increaseScore(getWorld()->getCurrentLevelBonus());
	}

}
bool Exit::allowsAgentColocation() const
{
	return true;
}

bool Exit::stopsBullet() const
{
	return false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ITEM
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PickupableItem::PickupableItem(StudentWorld* world, int startX, int startY, int imageID, unsigned int score)
	:Actor(world, startX, startY, imageID, 100, none), m_pscore(score)
{}

void PickupableItem::doSomething()
{}

bool PickupableItem::allowsAgentColocation() const
{
	return true;
}

unsigned int PickupableItem::pScore()
{
	return m_pscore;
}

bool PickupableItem::stopsBullet() const
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//JEWEL
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Jewel::Jewel(StudentWorld* world, int startX, int startY)
	:PickupableItem(world, startX, startY, IID_JEWEL, 50)
{}

void Jewel::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->isPlayerColocatedWith(getX(), getY()))
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

Goodie::Goodie(StudentWorld* world, int startX, int startY, int imageID, unsigned int score)
	:PickupableItem(world, startX, startY, imageID, score)
{}
void Goodie::doSomething()
{}

bool Goodie::isStealable() const
{
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//EXTRA LIFE
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ExtraLifeGoodie::ExtraLifeGoodie(StudentWorld* world, int startX, int startY)
	:Goodie(world, startX, startY, IID_EXTRA_LIFE, 1000)
{}

void ExtraLifeGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->isPlayerColocatedWith(getX(), getY()) && isVisible())
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

void RestoreHealthGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->isPlayerColocatedWith(getX(), getY()) && isVisible())
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

void AmmoGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (getWorld()->isPlayerColocatedWith(getX(), getY()) && isVisible())
	{
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(pScore());
		getWorld()->getPlayer()->increaseAmmo();
		setDead();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
