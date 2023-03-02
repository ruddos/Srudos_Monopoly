#include "Tile.h"
#include "Action.h"

using namespace std;

Tile::Tile(Tile_Type type) :
	m_tile_type(type)
{
}

Tile::Tile_Type Tile::get_tile_type(void) const
{
	return m_tile_type;
}

Street::Street(const StreetColors color, const std::string& caption, const vector<unsigned short>& rent, const unsigned short price) :
	PropertyTile(price, Tile_Type::Street, caption),
	m_houses(0),
	m_rent(rent),
	m_action(new StreetAction(*this)),
	m_color_type(color)
{
}

RailRoad::RailRoad(const std::string& caption, const unsigned short price) :
	PropertyTile(price, Tile_Type::RailRoad, caption),
	m_action(new RailRoadAction(*this))
{
}

Service::Service(const std::string& caption, const unsigned short price) :
	PropertyTile(price, Tile_Type::Service, caption),
	m_action(new ServiceAction(*this))
{
}
Action& Street::get_action(void)
{
	return *m_action;
}
Action& ActionTile::get_action(void)
{
	return *m_action;
}

Action& RailRoad::get_action(void)
{
	return *m_action;
}

Action& Service::get_action(void)
{
	return *m_action;
}

ActionTile::ActionTile(Tile_Type type, Action* action) :
	Tile(type),
	m_action(action)
{

}


unsigned short Street::get_rent(Game& game, unsigned char ind)
{
	if (!(get_pledge()))
	{
		if (m_houses == 0)
		{
			if (game.IsStreetCaptured(game.get_player(ind), m_color_type))
				return m_rent[0] * 2;
			else
				return m_rent[0];
		}
		return m_rent[m_houses];
	}
	else
		return 0;
}

Street::StreetColors Street::get_color_type(void)const
{
	return m_color_type;
}


unsigned short Street::get_houses()
{
	return m_houses;
}

Player* PropertyTile::get_ownership()
{
	return m_ownership;
}

PropertyTile::PropertyTile(unsigned short price, Tile_Type type, std::string caption)
	:Tile(type),
	m_price(price),
	m_caption(caption),
	m_tile_type(type),
	m_ownership(nullptr),
	m_pledge(false)
{
}

bool PropertyTile::get_pledge()
{
	return m_pledge;
}

unsigned short PropertyTile::get_price()
{
	return m_price;
}

void PropertyTile::set_ownership(Player& p)
{
	(*this).m_ownership = &p;
}

void Street::set_houses(unsigned short h)
{
	m_houses = h;
}

Street::StreetColors Street::get_color() const
{
	return m_color_type;
}