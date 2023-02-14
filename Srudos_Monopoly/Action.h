#pragma once
#include <iostream>
#include <vector>
#include "Tile.h"

using namespace std;

class Game;

class Action {
public:
	virtual void invoke(Game& game, unsigned char curr_player) = 0;
};

class StreetAction : public Action {
public:
	StreetAction(Street& street_tile);
	virtual void invoke(Game& game, unsigned char curr_player);

private:
	Street& m_street_Tile;
};

class RailRoadAction : public Action {
public:
	RailRoadAction(Railroad& railroad_tile);
	virtual void invoke(Game& game, unsigned char curr_player);

private:
	Railroad& m_railroad_tile;
};

class ServiceAction : public Action {
public:
	ServiceAction(Service& service_tile);
	virtual void invoke(Game& game, unsigned char curr_player);

private:
	Service& m_service_tile;
};

class EachHousesAction : public Action {
public:
	EachHousesAction(unsigned char house, unsigned char hotel);
	virtual void invoke(Game& game, unsigned char curr_player);

private:
	unsigned char m_houses;
	unsigned char m_hotels;
};
class Empty_Action : public Action {
public:
	virtual void invoke(Game& game, unsigned char curr_player);

private:
};

class GoToJailAction : public Action {
public:
	virtual void invoke(Game& game, unsigned char curr_player);

private:

};

class EscapeFromJailAction : public Action {
public:
	virtual void invoke(Game& game, unsigned char curr_player);

private:

};

class MoveToRailroad : public Action {
public:
	MoveToRailroad(unsigned char rail_index);
	virtual void invoke(Game& game, unsigned char curr_player);

private:
	unsigned char m_rail_index;
};

class MoveToNearest : public Action {
public:
	MoveToNearest(Tile::Tile_Type tile_type);
	virtual void invoke(Game& game, unsigned char curr_player);

private:
	Tile::Tile_Type m_tile_type;
};

class Move : public Action {
public:
	Move(unsigned char dice1, unsigned char dice2);
	virtual void invoke(Game& game, unsigned char curr_player);

private:
	unsigned char m_dice1;
	unsigned char m_dice2;
};

class Move_Abs_Action : public Action {
public:
	Move_Abs_Action(unsigned char to);
	virtual void invoke(Game& game, unsigned char curr_player);

private:
	unsigned char m_to;
};

class Take_Chance_Card : public Action {
public:
	virtual void invoke(Game& game, unsigned char curr_player);

private:

};

class Take_Community_Chest_Card : public Action {
public:
	virtual void invoke(Game& game, unsigned char curr_player);

private:

};

class MoneyFromPlayers : public Action {
public:
	MoneyFromPlayers(unsigned int sum);
	virtual void invoke(Game& game, unsigned char index);
private:
	unsigned int m_sum;
};

class MoneyToPlayers : public Action {
public:
	MoneyToPlayers(unsigned int sum);
	virtual void invoke(Game& game, unsigned char index);
private:
	unsigned int m_sum;
};

class MoneyFromBank : public Action {
public:
	MoneyFromBank(unsigned int sum);
	virtual void invoke(Game& game, unsigned char index);
private:
	unsigned int m_sum;
};

class Recieve_Money : public Action {
public:
	Recieve_Money(unsigned short sum);

	virtual void invoke(Game& game, unsigned char curr_player);
private:
	unsigned short m_sum;
};

class Pay2BankAction : public Action {
public:
	Pay2BankAction(unsigned short sum);

	virtual void invoke(Game& game, unsigned char curr_player);
private:
	unsigned short m_sum;
};