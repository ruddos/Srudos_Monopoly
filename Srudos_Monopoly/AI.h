#pragma once
#include"Tile.h"
class AI
{
public:
	AI(Player&);
	Player& get_player() const;
	virtual bool should_buy(PropertyTile&) const = 0;
	virtual bool jail_break(void) const = 0;
	virtual bool should_build(Street&) const = 0;
	virtual void pledge(PropertyTile&) const = 0;
private:
	Player& m_player;
};

class First_AI :public AI
{
public:
	First_AI(Player&);
	virtual bool should_buy(PropertyTile&) const;
	virtual bool jail_break(void) const;
	virtual bool should_build(Street&) const;
	virtual void pledge(PropertyTile&) const;
};

class Second_AI :public AI
{
public:
	Second_AI(Player&);
	virtual bool should_buy(PropertyTile&) const;
	virtual bool jail_break(void) const;
	virtual bool should_build(Street&) const;
	virtual void pledge(PropertyTile&) const;
};