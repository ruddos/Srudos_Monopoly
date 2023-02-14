#include "Game.h"
#include <iostream>
#include <vector>
#include "Tile.h"
#include "Action.h"
#include <random>

using namespace std;

vector<unique_ptr<Tile>> create_field(void);

Game::Game(unsigned char count) : m_Players(count) {
	m_field = create_field();
	m_chance = create_pack_of_chances();
	m_community_chest = create_pack_of_community_chests();
}

Game::PlayerData::PlayerData() : player(), pos(0), prison_counter(0), is_prisonDenied(false)
{

}

Game::~Game()
{

}

void Game::Player_to_Jail(unsigned char index) {
	m_Players[index].pos = 10;
	m_Players[index].prison_counter = 3;
}

void Game::EscapeFromJail(unsigned char index) {
	m_Players[index].is_prisonDenied = true;
}

unsigned char Game::time_to_ban(unsigned char curr_player) {
	char i = 0;
	char result = (curr_player + 1) % m_Players.size();
	while (i < m_Players.size()) {
		if (m_Players[i].player.get_money() < 0) {
			cout << "Deleted player " << i << endl;
			m_Players.erase(m_Players.begin() + i);
			if (i < result)
				--result;
		}
		else
			++i;
	}
	if (result >= m_Players.size())
		result = 0;
	return result;
}

void Game::move_Player(unsigned char player_index, unsigned char dice1, unsigned char dice2) {
	unsigned char dice = dice1 + dice2;
	if (m_Players[player_index].prison_counter != 0) {
		if (m_Players[player_index].is_prisonDenied) {
			m_Players[player_index].is_prisonDenied = false;
			m_Players[player_index].prison_counter = 0;
		}
		else if (dice1 != dice2) {
			m_Players[player_index].prison_counter -= 1;
			return;
		}
		else
			m_Players[player_index].prison_counter = 0;
	}
	unsigned char old_pos = m_Players[player_index].pos;
	m_Players[player_index].pos = (m_Players[player_index].pos + dice) % 40;
	if (old_pos > m_Players[player_index].pos)
		m_Players[player_index].player.claim_money(200);
	m_field[m_Players[player_index].pos]->get_Action().invoke(*this, player_index);
}

void Game::move_Player_abs(unsigned char player_index, unsigned char cell) {
	unsigned char old_pos = m_Players[player_index].pos;
	m_Players[player_index].pos = cell;
	if (old_pos > m_Players[player_index].pos)
		m_Players[player_index].player.claim_money(200);
	m_field[m_Players[player_index].pos]->get_Action().invoke(*this, player_index);
}

void Game::move_until(unsigned char player_index, const std::function<bool(const Tile&)>& f) {
	unsigned char old_pos = m_Players[player_index].pos;
	while (!f(*m_field[m_Players[player_index].pos])) {
		m_Players[player_index].pos = (m_Players[player_index].pos + 1) % m_field.size();
	}
	if (old_pos > m_Players[player_index].pos)
		m_Players[player_index].player.claim_money(200);
	m_field[m_Players[player_index].pos]->get_Action().invoke(*this, player_index);
}

Action& Game::get_next_chance_card(void)
{
	m_chance.push_back(unique_ptr<Action>());
	swap(m_chance.front(), m_chance.back());
	m_chance.pop_front();
	return *m_chance.back();
}

pair<unsigned char, unsigned char> Game::get_dices() {
	pair<unsigned char, unsigned char> dices;
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<unsigned short> uni(1, 6);

	dices.first = uni(rng);
	dices.second = uni(rng);
	return dices;
}

unsigned char Game::how_many_houses(Player& player, unsigned char forhotels, unsigned char forhouses) {
	unsigned short houses = 0;
	unsigned short hotels = 0;
	for (int i = 0; i < m_field.size(); ++i) {
		if (m_field[i]->get_tile_type() == Tile::Tile_Type::street) {
			Street* s = dynamic_cast<Street*>(m_field[i].get());
			if (s->get_OwnerShip() == &player)
				if (s->get_houses_count() == 3)
					hotels += 1;
				else
					houses += s->get_houses_count();
		}
	}
	return houses * forhouses + hotels * forhotels;
}

unsigned char Game::how_many_captured(Player& player) {
	unsigned char n = 0;
	for (int i = 0; i < m_field.size(); ++i) {
		if (m_field[i]->get_tile_type() == Tile::Tile_Type::railroad) {
			Railroad* r = dynamic_cast<Railroad*>(m_field[i].get());
			if (r->get_OwnerShip() == &player)
				n += 1;
		}
	}
	return n;
}

unsigned char Game::service_rent(Player& player) {
	unsigned char n = 0;
	for (int i = 0; i < m_field.size(); ++i) {
		if (m_field[i]->get_tile_type() == Tile::Tile_Type::service) {
			Service* s = dynamic_cast<Service*>(m_field[i].get());
			if (s->get_OwnerShip() == &player)
				n += 1;
		}
	}
	pair<unsigned char, unsigned char> dices = get_dices();
	return (dices.first + dices.second) * n;
}

bool Game::is_street_captured(Player& player, Street::StreetColors color) {
	unsigned char n = 0;
	unsigned char street_at_all = 0;
	for (int i = 0; i < m_field.size(); ++i) {
		if (m_field[i]->get_tile_type() == Tile::Tile_Type::street) {
			Street* s = dynamic_cast<Street*>(m_field[i].get());
			if (s->get_street_color() == color) {
				street_at_all += 1;
				if (s->get_OwnerShip() == &player)
					n += 1;
			}
		}
	}
	if (n == street_at_all)
		return true;
	return false;
}

Tile& Game::get_player_tile(unsigned char index) {
	return *m_field[m_Players[index].pos];
}

Action& Game::get_next_community_chest_card(void) {
	m_community_chest.push_back(unique_ptr<Action>());
	swap(m_community_chest.front(), m_community_chest.back());
	m_community_chest.pop_front();
	return *m_community_chest.back();

}

int Game::get_players_count(void) {
	return m_Players.size();
}

Player& Game::get_player(unsigned char index) {
	return m_Players[index].player;
}

list<unique_ptr<Action>> Game::create_pack_of_chances(void) {
	list<unique_ptr<Action>> chance;
	chance.push_back(unique_ptr<Move_Abs_Action>(new Move_Abs_Action(24))); // ������������� to Illinois ave. 200							PASS
	chance.push_back(unique_ptr<Pay2BankAction>(new Pay2BankAction(50))); // pay poor tax | ���� - 15, ���� 50
	chance.push_back(unique_ptr<Move>(new Move(-3, 0))); // ��������� �� ��� �������� �����
	chance.push_back(unique_ptr<MoveToNearest>(new MoveToNearest(Tile::Tile_Type::service))); //to nearest utility
	chance.push_back(unique_ptr<MoveToNearest>(new MoveToNearest(Tile::Tile_Type::railroad))); // ������������� �� ��������� �����. ������ + twice rent
	chance.push_back(unique_ptr<EscapeFromJailAction>(new EscapeFromJailAction())); // ������������ �� ������
	chance.push_back(unique_ptr<GoToJailAction>(new GoToJailAction())); // ������������� � ������
	chance.push_back(unique_ptr<Pay2BankAction>(new Pay2BankAction(150))); // Chto-to tam buildings
	chance.push_back(unique_ptr<Move_Abs_Action>(new Move_Abs_Action(39))); // Advance to Boardwalk
	chance.push_back(unique_ptr<MoneyFromBank>(new MoneyFromBank(50))); // ���������� ���������, �������� 50
	chance.push_back(unique_ptr<MoveToNearest>(new MoveToNearest(Tile::Tile_Type::railroad))); // ������������� �� ��������� �����. ������ + twice rent
	chance.push_back(unique_ptr<MoneyToPlayers>(new MoneyToPlayers(50))); //ELECTED pay each player 50
	chance.push_back(unique_ptr<Move_Abs_Action>(new Move_Abs_Action(0))); // Advance to go
	chance.push_back(unique_ptr<Move_Abs_Action>(new Move_Abs_Action(12))); // Advance to St. Charles Place
	chance.push_back(unique_ptr<MoveToNearest>(new MoveToNearest(Tile::Tile_Type::railroad))); // ������������� �� ��������� �����. ������ + if start + 200
	chance.push_back(unique_ptr<EachHousesAction>(new EachHousesAction(25, 100)));

	return chance;
}

list<unique_ptr<Action>> Game::create_pack_of_community_chests(void) {
	list<unique_ptr<Action>> community_chest;
	community_chest.push_back(unique_ptr<Move_Abs_Action>(new Move_Abs_Action(0))); // Advance to go collect 200
	community_chest.push_back(unique_ptr<Pay2BankAction>(new Pay2BankAction(100))); // ������ �������
	community_chest.push_back(unique_ptr<Pay2BankAction>(new Pay2BankAction(50))); // ������ ���������
	community_chest.push_back(unique_ptr<MoneyFromBank>(new MoneyFromBank(100))); // �� �������� ���������� 
	community_chest.push_back(unique_ptr<MoneyFromBank>(new MoneyFromBank(20))); // Income tax collect 20
	community_chest.push_back(unique_ptr<EscapeFromJailAction>(new EscapeFromJailAction())); // ������������ �� ������
	community_chest.push_back(unique_ptr<MoneyFromPlayers>(new MoneyFromPlayers(50))); //Collect 50 from every player
	community_chest.push_back(unique_ptr<MoneyFromBank>(new MoneyFromBank(100))); // Life insurance
	community_chest.push_back(unique_ptr<GoToJailAction>(new GoToJailAction())); // Go to jail
	community_chest.push_back(unique_ptr<MoneyFromBank>(new MoneyFromBank(200))); // Bank error in your favor
	community_chest.push_back(unique_ptr<Pay2BankAction>(new Pay2BankAction(150))); //Pay school tax
	community_chest.push_back(unique_ptr<MoneyFromBank>(new MoneyFromBank(100))); // Xmax fund matures
	community_chest.push_back(unique_ptr<MoneyFromBank>(new MoneyFromBank(10))); // �� �������� �� �������� �������
	community_chest.push_back(unique_ptr<MoneyFromBank>(new MoneyFromBank(45))); // From sale of stock
	community_chest.push_back(unique_ptr<MoneyFromBank>(new MoneyFromBank(25))); // ���������� ������
	community_chest.push_back(unique_ptr<EachHousesAction>(new EachHousesAction(40, 115)));

	return community_chest;
}

vector<unique_ptr<Tile>> create_field(void) {
	vector<unique_ptr<Tile>> field;

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Empty_Action(), Tile::Tile_Type::start)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::brown, "Mediter-Ranean Avenue", { 14 }, 60)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Take_Community_Chest_Card(), Tile::Tile_Type::community_chest)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::brown, "Baltic Avenue", { 14 }, 60)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Pay2BankAction(200), Tile::Tile_Type::tax)));
	field.push_back(unique_ptr<Tile>(new Railroad("Reading railroad")));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::blue, "Oriental Avenue", { 14 }, 100)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Take_Chance_Card(), Tile::Tile_Type::chance)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::blue, "Vermont Avenue", { 14 }, 100)));
	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::blue, "Connecticut Avenue", { 14 }, 100)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Empty_Action(), Tile::Tile_Type::jail)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::pink, "St.Charles Place", { 14 }, 140)));

	field.push_back(unique_ptr<Tile>(new Service("Electic company")));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::pink, "States Avenue", { 14 }, 140)));
	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::pink, "Vegrinia Avenue", { 14 }, 160)));

	field.push_back(unique_ptr<Tile>(new Railroad("Pensilvania railroad")));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::orange, "St.James Place", { 14 }, 180)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Take_Community_Chest_Card(), Tile::Tile_Type::community_chest)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::orange, "Tennessee Avenue", { 14 }, 180)));
	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::orange, "New York Avenue", { 14 }, 200)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Empty_Action(), Tile::Tile_Type::free_parking)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::red, "Kentucky Avenue", { 14 }, 220)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Take_Chance_Card(), Tile::Tile_Type::chance)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::red, "Indiana Avenue", { 14 }, 220)));
	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::red, "Illinois Avenue", { 14 }, 240)));

	field.push_back(unique_ptr<Tile>(new Railroad("B. & O. railroad")));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::yellow, "Atlantic Avenue", { 14 }, 260)));
	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::yellow, "Ventnor Avenue", { 14 }, 260)));

	field.push_back(unique_ptr<Tile>(new Service("Water works")));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::yellow, "Marvin Gardens", { 14 }, 280)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new GoToJailAction(), Tile::Tile_Type::go_to_jail)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::green, "Pacific Avenue", { 14 }, 300)));
	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::green, "North Carolina Avenue", { 14 }, 300)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Take_Community_Chest_Card(), Tile::Tile_Type::community_chest)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::green, "Pennsylvania Avenue", { 14 }, 320)));

	field.push_back(unique_ptr<Tile>(new Railroad("Short Line")));
	field.push_back(unique_ptr<Tile>(new Action_Tile(new Take_Chance_Card(), Tile::Tile_Type::chance)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::dark_blue, "Park Place", { 14 }, 350)));

	field.push_back(unique_ptr<Tile>(new Action_Tile(new Pay2BankAction(100), Tile::Tile_Type::tax)));

	field.push_back(unique_ptr<Tile>(new Street(Street::StreetColors::dark_blue, "Boardwalk", { 14 }, 400)));

	return field;
}