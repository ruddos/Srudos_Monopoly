#include "Action.h"
#include "Game.h"

using namespace std;

Pay2BankAction::Pay2BankAction(unsigned short sum) : m_sum(sum) {

}

Recieve_Money::Recieve_Money(unsigned short sum) : m_sum(sum) {

}

MoneyFromPlayers::MoneyFromPlayers(unsigned int sum) : m_sum(sum) {

}

MoneyFromBank::MoneyFromBank(unsigned int sum) : m_sum(sum) {

}

MoveToNearest::MoveToNearest(Tile::Tile_Type tile_type) : m_tile_type(tile_type) {

}

Move::Move(unsigned char d1, unsigned char d2) : m_dice1(d1), m_dice2(d2) {

}

MoneyToPlayers::MoneyToPlayers(unsigned int sum) : m_sum(sum) {

}

MoveToRailroad::MoveToRailroad(unsigned char rail_index) : m_rail_index(rail_index) {

}

StreetAction::StreetAction(Street& street) : m_street_Tile(street) {

}

RailRoadAction::RailRoadAction(Railroad& railroad) : m_railroad_tile(railroad) {

}

ServiceAction::ServiceAction(Service& service) : m_service_tile(service) {

}

void ServiceAction::invoke(Game& game, unsigned char index) {
	if (m_service_tile.get_OwnerShip() == nullptr)
		return;
	game.get_player(index).pay(m_service_tile.get_rent(game, index));
	m_service_tile.get_OwnerShip()->claim_money(m_service_tile.get_rent(game, index));
}

void RailRoadAction::invoke(Game& game, unsigned char index) {
	if (m_railroad_tile.get_OwnerShip() == nullptr)
		return;
	game.get_player(index).pay(m_railroad_tile.get_rent(game, index));
	m_railroad_tile.get_OwnerShip()->claim_money(m_railroad_tile.get_rent(game, index));
}

EachHousesAction::EachHousesAction(unsigned char house, unsigned char hotel) : m_houses(house), m_hotels(hotel) {

}

void EachHousesAction::invoke(Game& game, unsigned char index) {
	game.get_player(index).pay(game.how_many_houses(game.get_player(index), m_houses, m_hotels));
}

void StreetAction::invoke(Game& game, unsigned char index) {
	if (m_street_Tile.get_OwnerShip() == nullptr || m_street_Tile.get_OwnerShip() == &game.get_player(index)) {
		return;
	}
	game.get_player(index).pay(m_street_Tile.get_rent(game, index));
}

void MoveToRailroad::invoke(Game& game, unsigned char index) {
	game.move_until(index, [=](const Tile& tile) {
		return tile.get_tile_type() == Tile::Tile_Type::railroad;
		});
	game.get_player_tile(index);
}

void MoneyToPlayers::MoneyToPlayers::invoke(Game& game, unsigned char index) {
	for (int i = 0; i < game.get_players_count(); ++i) {
		if (i == index)
			game.get_player(index).pay(m_sum);
		else
			game.get_player(index).claim_money(game.get_players_count() - 1 * m_sum);
	}
}

void EscapeFromJailAction::invoke(Game& game, unsigned char index) {
	game.EscapeFromJail(index);
}

void GoToJailAction::invoke(Game& game, unsigned char index) {
	game.Player_to_Jail(index);
}

void MoveToNearest::invoke(Game& game, unsigned char index) {
	game.move_until(index, [=](const Tile& tile) {
		return tile.get_tile_type() == m_tile_type;
		});
}

void Move::invoke(Game& game, unsigned char index) {
	game.move_Player(index, m_dice1, m_dice2);
}


void MoneyFromPlayers::invoke(Game& game, unsigned char index) {
	for (int i = 0; i < game.get_players_count(); ++i) {
		if (i != index)
			game.get_player(index).pay(m_sum);
		else
			game.get_player(index).claim_money(game.get_players_count() - 1 * m_sum);
	}
}

void MoneyFromBank::invoke(Game& game, unsigned char index) {
	game.get_player(index).claim_money(m_sum);
}

void Empty_Action::invoke(Game& game, unsigned char curr_player) {

}

Move_Abs_Action::Move_Abs_Action(unsigned char to) : m_to(to)
{
}

void Move_Abs_Action::invoke(Game& game, unsigned char curr_player) {
	game.move_Player_abs(curr_player, m_to);
}

void Take_Chance_Card::invoke(Game& game, unsigned char curr_player) {
	game.get_next_chance_card().invoke(game, curr_player);
}

void Take_Community_Chest_Card::invoke(Game& game, unsigned char curr_player) {
	game.get_next_community_chest_card().invoke(game, curr_player);
}

void Recieve_Money::invoke(Game& game, unsigned char curr_player) {
	game.get_player(curr_player).claim_money(m_sum);
}


void Pay2BankAction::invoke(Game& game, unsigned char curr_player) {
	game.get_player(curr_player).pay(m_sum);
}