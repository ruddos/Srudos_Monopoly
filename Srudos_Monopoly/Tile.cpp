#include "Tile.h"
#include "Action.h"
#include "Game.h"

PropertyTile::PropertyTile(Tile_Type Type, unsigned char Price, string Caption) : Tile(Type), m_Price(Price), m_Ownership(nullptr), m_Caption(Caption), m_Pledge(false) {

}

Street::Street(StreetColors color, const string& caption, const vector<unsigned short>& rent, const unsigned short& price) : PropertyTile(Tile_Type::street, price, caption),
m_Color(color), m_Rent(rent), m_action(new StreetAction(*this))
{
}

Player* PropertyTile::get_OwnerShip() {
	return m_Ownership;
};

bool PropertyTile::get_Pledge() {
	return m_Pledge;
}

unsigned char PropertyTile::get_Price() {
	return m_Price;
}

unsigned short Street::get_rent(Game& game, unsigned char player_index) {
	bool is_captured = game.is_street_captured(game.get_player(player_index), m_Color);
	if (!get_Pledge()) {
		if (m_Houses > 0)
			return m_Rent[m_Houses];

		else if (is_captured)
			return m_Rent[0] * 2;

		else
			return m_Rent[0];
	}
	return 0;
}

Street::StreetColors Street::get_street_color() {
	return m_Color;
}

unsigned char Street::get_houses_count() {
	return m_Houses;
}

Railroad::Railroad(string caption) : PropertyTile(Tile_Type::railroad, 200, caption),
m_Rent(100), m_action(new RailRoadAction(*this))
{
}

unsigned short Railroad::get_rent(Game& game, unsigned char player_index) {
	if (!get_Pledge() && get_OwnerShip() != nullptr && get_OwnerShip() != &game.get_player(player_index)) {
		unsigned char n = game.how_many_captured(game.get_player(player_index));
		return m_Rent * n;
	}
	return 0;
}

Service::Service(string caption) : PropertyTile(Tile_Type::service, 150, caption), m_action(new ServiceAction(*this))
{
}

unsigned short Service::get_rent(Game& game, unsigned char player_index) {
	if (!get_Pledge())
		return game.service_rent(game.get_player(player_index));
	return 0;
}

Action_Tile::Action_Tile(Action* action, Tile::Tile_Type type) : Tile(type), m_action(action) {
}

Tile::Tile(Tile::Tile_Type type) : m_type(type) {

}

Tile::Tile_Type Tile::get_tile_type(void) const {
	return m_type;
}

Action& Street::get_Action(void) {
	return *m_action;
}

Action& Railroad::get_Action(void) {
	return *m_action;
}

Action& Service::get_Action(void) {
	return *m_action;
}

Action& Action_Tile::get_Action(void) {
	return *m_action;
}