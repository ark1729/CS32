
class Character
{
public:
	Character(string name)
		:m_name(name)
	{}
	virtual void printWeapon() const = 0;
	virtual string attackAction() const
	{
		return "rushes toward the enemy";
	}
	virtual ~Character()
	{}
	string name() const
	{
		return m_name;
	}
private:
	string m_name;
};

class Dwarf : public Character
{
public:
	Dwarf(string name)
		:Character(name)
	{}
	virtual ~Dwarf()
	{
		cout << "Destroying " << name() << " the dwarf" << endl;
	}
	virtual void printWeapon() const
	{
		cout << "an axe";
	}
};

class Elf : public Character
{
public:
	Elf(string name, int numofarrows)
		:Character(name), m_arrows(numofarrows)
	{}
	virtual ~Elf()
	{
		cout << "Destroying " << name() << " the elf" << endl;
	}
	virtual void printWeapon() const
	{
		cout << "a bow and quiver of " << m_arrows << " arrows";
	}
private:
	int m_arrows;
};

class Boggie : public Character
{
public:
	Boggie(string name)
		:Character(name)
	{}
	virtual ~Boggie()
	{
		cout << "Destroying " << name() << " the boggie" << endl;
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

