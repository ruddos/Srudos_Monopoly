#pragma once
#include <iostream>
#include <vector>
#include "Tile.h"
#include "Player.h"
#include <list>
#include <functional>

using namespace std;

class Action;

class Game {
public:
	Game(unsigned char count);
	~Game();
	list<unique_ptr<Action>> create_pack_of_chances(void);
	list<unique_ptr<Action>> create_pack_of_community_chests(void);

	void move_Player(unsigned char player_index, unsigned char dice1, unsigned char dice2);
	void move_Player_abs(unsigned char player_index, unsigned char cell);
	void move_until(unsigned char player_index, const std::function<bool(const Tile&)>& f); //[](){} - ������. �������������: [](const Tile& t) -> bool {����������}
	void Player_to_Jail(unsigned char player_index);
	void EscapeFromJail(unsigned char player_index);

	pair<unsigned char, unsigned char> get_dices();
	Action& get_next_chance_card(void);
	Action& get_next_community_chest_card(void);
	int get_players_count(void);
	Player& get_player(unsigned char index);
	Tile& get_player_tile(unsigned char index);
	bool is_street_captured(Player& player, Street::StreetColors color);
	unsigned char how_many_captured(Player& player);
	unsigned char service_rent(Player& player);
	unsigned char how_many_houses(Player& player, unsigned char forhouse, unsigned char forhotel);
	unsigned char time_to_ban(unsigned char curr_player);

private:
	struct PlayerData {
		PlayerData();
		Player player;
		unsigned char pos;
		unsigned char prison_counter;
		bool is_prisonDenied;
	};

	vector<unique_ptr<Tile>> m_field;
	vector<PlayerData> m_Players;
	list<unique_ptr<Action>> m_chance;
	list<unique_ptr<Action>> m_community_chest;
};