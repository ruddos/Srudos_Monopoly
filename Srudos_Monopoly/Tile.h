#pragma once
#include <vector>
#include <memory>
#include <string>


using namespace std;

class Action;
class Game;
class Player;

class Tile
{
public:
	enum class Tile_Type
	{
		Street,
		Start,
		CommChest,
		Tax,
		RailRoad,
		Chance,
		Jail,
		Service,
		FreeParking,
		GoToJail,
	};
	virtual Action& get_action(void) = 0;
	Tile_Type get_tile_type(void) const;

protected:
	Tile(Tile_Type type);
private:
	const Tile_Type m_tile_type;
};

class ActionTile :public Tile
{
public:
	ActionTile(Tile_Type type, Action* action);
	virtual Action& get_action(void);
private:
	std::unique_ptr<Action>m_action;
};

class PropertyTile :public Tile
{
public:
	Player* get_ownership();
	bool get_pledge();
	unsigned short get_price();
	PropertyTile(unsigned short price, Tile_Type type, std::string caption);
	void set_ownership(Player& p);
private:
	unsigned short m_price;
	Player* m_ownership;
	bool m_pledge;
	std::string m_caption;
	const Tile_Type m_tile_type;

};

class Street :public PropertyTile
{
public:

	enum StreetColors
	{
		Brown,
		Blue,
		Pink,
		Orange,
		Red,
		Yellow,
		Green,
		DarkBlue,
	};

	Street(const StreetColors color, const std::string& caption, const vector<unsigned short>& rent, const unsigned short price);

	virtual Action& get_action(void);

	unsigned short get_rent(Game& game, unsigned char ind);
	StreetColors get_color_type(void)const;
	unsigned short get_houses();
	void set_houses(unsigned short h);
	StreetColors get_color() const;

private:
	unsigned char m_houses;
	vector<unsigned short> m_rent;
	unique_ptr<Action>m_action;
	const StreetColors m_color_type;
};

class RailRoad :public PropertyTile
{
public:
	RailRoad(const std::string& caption, const unsigned short price);
	virtual Action& get_action(void);
private:
	unique_ptr<Action>m_action;
};

class Service :public PropertyTile
{
public:
	Service(const std::string& caption, const unsigned short price);
	virtual Action& get_action(void);
private:
	unique_ptr<Action>m_action;
};