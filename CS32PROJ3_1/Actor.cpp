#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, Direction startDirection)
	:GraphObject(imageID, startX, startY, startDirection), world(sw), m_block(false), m_dead(false)
{
	setVisible(true);
}
Actor::~Actor()
{}
void Actor::doSomething()
{}
void Actor::setBlock()
{
	m_block = true;
}
bool Actor::seeBlock()
{
	return m_block;
}
void Actor::setDead()
{
	m_dead = true;
}
bool Actor::seeDead()
{
	return m_dead;
}
StudentWorld* Actor::getWorld()
{
	return world;
}


Player::Player(StudentWorld* sw, int startX, int startY)
	:Actor(sw, IID_PLAYER, startX, startY, right)
{}
Player::~Player()
{}
void Player::doSomething()
{
	int ch;
	if (getWorld()->getKey(ch))
	{
	// user hit a key this tick!
		switch (ch)
		{
		case KEY_PRESS_LEFT:
			if (!(getWorld()->ifActorBlocks(getX() - 1, getY())))
			{
				moveTo(getX() - 1, getY());
				setDirection(left);
			}
			break;
		case KEY_PRESS_RIGHT:
			if (!(getWorld()->ifActorBlocks(getX() + 1, getY())))
			{
				moveTo(getX() + 1, getY());
				setDirection(right);
			}
			break;
		case KEY_PRESS_UP:
			if (!(getWorld()->ifActorBlocks(getX(), getY() + 1)))
			{
				moveTo(getX(), getY() + 1);
				setDirection(up);
			}
			break;
		case KEY_PRESS_DOWN:
			if (!(getWorld()->ifActorBlocks(getX(), getY() - 1)))
			{
				moveTo(getX(), getY() - 1);
				setDirection(down);
			}
			break;
		case KEY_PRESS_ESCAPE:
			getWorld()->decLives();
			setDead();
			break;
		}
	}
}

	
Wall::Wall(StudentWorld* sw, int startX, int startY, Direction startDirection)
	:Actor(sw, IID_WALL, startX, startY)
{
	setBlock();
}
Wall::~Wall()
{}

