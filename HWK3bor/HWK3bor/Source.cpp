#include <iostream>
#include <string>
using namespace std;

class Character
{
public:
	Character(string name);
	virtual void printWeapon() const = 0;
	virtual string attackAction() const
	{
		return "rushes toward the enemy";
	}
	virtual ~Character()
	{

	}
	string name() const
	{
		return m_name;
	}
private:
	string m_name;
};

class Dwarf:public Character
{
public:
	Dwarf(string name)
		:Character(name)
	{

	}
	virtual ~Dwarf()
	{

	}
	virtual void printWeapon() const
	{
		cout << "an axe";
	}
};

class Elf:public Character
{
public:
	Elf(string name, int numofarrows)
		:Character(name), m_arrows(numofarrows)
	{

	}
	virtual ~Elf()
	{

	}
	virtual void printWeapon() const
	{
		cout << "a bow and quiver of " << m_arrows << "arrows";
	}
private:
	int m_arrows;
};

class Boggie:public Character
{
public:
	Boggie(string name)
		:Character(name)
	{

	}
	virtual ~Boggie()
	{

	}
	virtual void printWeapon() const
	{
		cout << "a short sword";
	}
	virtual string attackAction() const
	{
		return "whimpers";
	}
};

void strike(const Character* cp)
{
	cout << cp->name() << ", wielding ";
	cp->printWeapon();
	cout << ", " << cp->attackAction() << "." << endl;
}

int main()
{
	Character* characters[4];
	characters[0] = new Dwarf("Gimlet");
	// Elves have a name and initial number of arrows in their quiver
	characters[1] = new Elf("Legolam", 10);
	characters[2] = new Boggie("Frito");
	characters[3] = new Boggie("Spam");

	cout << "The characters strike!" << endl;
	for (int k = 0; k < 4; k++)
		strike(characters[k]);

	// Clean up the characters before exiting
	cout << "Cleaning up" << endl;
	for (int k = 0; k < 4; k++)
		delete characters[k];
}