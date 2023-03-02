#pragma once
#include "Tile.h"
#include "Player.h"
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <list>
using namespace std;

class Game
{
public:
	Game(unsigned char players_count);
	void MovePlayer(char dice_1, char dice_2, unsigned char ind);
	void MoveUntil(const std::function<bool(const Tile&)>& f, unsigned char ind);
	void GoTo(unsigned char where_to, unsigned char ind);
	void GoToJail(unsigned char ind);
	Action& get_next_chance(void);
	Action& get_next_com_chest(void);
	Player& get_player(unsigned char ind);
	Tile& get_tile(unsigned char ind);
	void SetPrisonDenied(unsigned char ind);
	bool IsStreetCaptured(Player& p, Street::StreetColors color);
	unsigned short RailroadsCount(Player& p);
	unsigned short ServiceCount(Player& p);
	pair<unsigned short, unsigned short> throw_dices();
	pair<unsigned short, unsigned short> CountOfHousesAndHotels(Player& p);
	unsigned char CheckingForNischeta(unsigned char current_player);
	unsigned short GetPlayersAmount();
	void log(ostream& s, unsigned char ind, int hod);
private:
	vector<unique_ptr<Tile>> m_field;
	list<unique_ptr<Action>>m_chance;
	list<unique_ptr<Action>>m_community_chest;
	struct PlayerData
	{
		PlayerData();
		Player player;
		unsigned char pos;
		unsigned char prison_counter;
		bool is_prison_denied;
	};
	vector<PlayerData>m_players;
};