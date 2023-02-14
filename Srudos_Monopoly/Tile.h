#pragma once
#include <vector>
#include <iostream>
#include "Player.h"

using namespace std;

class Action;
class Game;

class Tile {
public:
	enum Tile_Type {
		street,
		start,
		community_chest,
		tax,
		railroad,
		chance,
		jail,
		service,
		free_parking,
		go_to_jail
	};

	virtual Action& get_Action(void) = 0;
	Tile_Type get_tile_type(void) const;

protected:
	Tile(Tile_Type t);

private:
	Tile_Type m_type;
};

class PropertyTile : public Tile {
public:
	Player* get_OwnerShip();
	bool get_Pledge();
	unsigned char get_Price();

protected:
	PropertyTile(Tile_Type Type, unsigned char Price, string Caption);

private:
	unsigned char m_Price;
	Player* m_Ownership;
	string m_Caption;
	bool m_Pledge;
};

class Action_Tile : public Tile {
public:
	Action_Tile(Action* action, Tile_Type type);

	virtual Action& get_Action(void);

private:
	unique_ptr<Action> m_action;
};

class Railroad : public PropertyTile {
public:
	Railroad(string Caption);
	unsigned short get_rent(Game& game, unsigned char player_index);
	virtual Action& get_Action(void);

private:
	const unsigned short m_Rent;
	unique_ptr<Action> m_action;
};

class Service : public PropertyTile {
public:
	Service(string caption);
	unsigned short get_rent(Game& game, unsigned char player_index);
	virtual Action& get_Action(void);

private:
	unique_ptr<Action> m_action;
};

class Street : public PropertyTile {
public:
	virtual Action& get_Action(void);

	enum StreetColors {
		brown,
		blue,
		pink,
		orange,
		red,
		yellow,
		green,
		dark_blue
	};

	Street(StreetColors color, const string& caption, const vector<unsigned short>& rent, const unsigned short& price);
	unsigned short get_rent(Game& game, unsigned char player_index);
	unsigned char get_houses_count();
	Street::StreetColors get_street_color();

private:
	const StreetColors m_Color;
	const vector<unsigned short> m_Rent;
	unsigned char m_Houses;
	unique_ptr<Action> m_action;
};