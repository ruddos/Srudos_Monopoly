#pragma once
#include  "Tile.h"

class AI {
public:
	virtual void shouldbuy(PropertyTile& tile) = 0;
	virtual bool jail_break(void) = 0;
	virtual bool shouldbuild(Street& street) = 0;
	virtual void pledge(void) = 0;

private:
};

class PlayerAI {
public:
	PlayerAI(Player& player);
	virtual bool shouldbuy(PropertyTile& tile);
	virtual bool jail_break(void) = 0;
	virtual bool shouldbuild(Street& street) = 0;
	virtual void pledge(void) = 0;

private:
	Player& m_Player;
};