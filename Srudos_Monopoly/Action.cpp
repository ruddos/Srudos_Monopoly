#include "Action.h"
Pay2BankAction::Pay2BankAction(unsigned short sum)
{
	m_sum = sum;
}

void Pay2BankAction::invoke(Game& game, unsigned char ind)
{
	game.get_player(ind).pay(m_sum);
}

void EmptyAction::invoke(Game& game, unsigned char ind)
{

}
GoToAction::GoToAction(unsigned char where)
	:m_where(where)
{

}
void GoToAction::invoke(Game& game, unsigned char ind)
{
	game.GoTo(m_where, ind);
}
void TakeChanceCard::invoke(Game& game, unsigned char ind)
{
	game.get_next_chance().invoke(game, ind);
}
void TakeCommunityCard::invoke(Game& game, unsigned char ind)
{
	game.get_next_com_chest().invoke(game, ind);
}
Move2Nearest::Move2Nearest(Tile::Tile_Type type)
{
	m_tile_type = type;
}
void Move2Nearest::invoke(Game& game, unsigned char ind)
{
	game.MoveUntil([=](const Tile& tile) {return tile.get_tile_type() == m_tile_type; }, ind);
}

void Go2JailAction::invoke(Game& game, unsigned char ind)
{
	game.GoToJail(ind);
}

void MoveAction::invoke(Game& game, unsigned char ind)
{
	game.MovePlayer(m_naskoka, 0, ind);
}

MoveAction::MoveAction(unsigned char naskoka)
{
	m_naskoka = naskoka;
}

void PrisonDenierAction::invoke(Game& game, unsigned char ind)
{
	game.SetPrisonDenied(ind);
}

GetFromBank::GetFromBank(unsigned short sum)
{
	m_sum = sum;
}

void GetFromBank::invoke(Game& game, unsigned char ind)
{
	game.get_player(ind).get(m_sum);
}

PayEverybodyAction::PayEverybodyAction(unsigned char sum) :
	m_sum(sum)
{

}

void PayEverybodyAction::invoke(Game& game, unsigned char ind)
{
	game.get_player(ind).pay(3 * m_sum);
	for (int i = 0; i < game.GetPlayersAmount(); i++)
	{
		if (i != ind)
			game.get_player(i).get(m_sum);
	}
}

GetFromEverybodyAction::GetFromEverybodyAction(unsigned char sum)
	:m_sum(sum)
{

}

void GetFromEverybodyAction::invoke(Game& game, unsigned char ind)
{
	game.get_player(ind).get(3 * m_sum);
	for (int i = 0; i < game.GetPlayersAmount(); i++)
	{
		if (i != ind)
			game.get_player(i).pay(m_sum);
	}
}

StreetAction::StreetAction(Street& street) :
	m_street(street)
{

}

RailRoadAction::RailRoadAction(RailRoad& railroad) :
	m_railroad(railroad)
{
}

ServiceAction::ServiceAction(Service& service) :
	m_service(service)
{
}

void StreetAction::invoke(Game& game, unsigned char ind)
{
	if (m_street.get_ownership() != nullptr)
	{
		if (m_street.get_ownership() != &game.get_player(ind))
		{
			game.get_player(ind).pay(m_street.get_rent(game, ind));
			m_street.get_ownership()->get(m_street.get_rent(game, ind));
		}
	}
}

void RailRoadAction::invoke(Game& game, unsigned char ind)
{
	if (m_railroad.get_ownership() != nullptr)
	{
		if (m_railroad.get_ownership() != &game.get_player(ind))
		{
			unsigned short sum = game.RailroadsCount(game.get_player(ind));
			game.get_player(ind).pay(50 * sum);
			m_railroad.get_ownership()->get(50 * sum);
		}
	}
}

void ServiceAction::invoke(Game& game, unsigned char ind)
{
	if (m_service.get_ownership() != nullptr)
	{
		if (m_service.get_ownership() != &game.get_player(ind))
		{
			pair<unsigned short, unsigned short>p = game.throw_dices();
			unsigned short sum = p.first + p.second;
			if (game.ServiceCount(*m_service.get_ownership()) == 1)
				sum *= 4;
			else
				sum *= 10;
			game.get_player(ind).pay(sum);
			m_service.get_ownership()->get(sum);
		}
	}
}

void Go2NearestRailroadPayTwiceAction::invoke(Game& game, unsigned char ind)
{
	game.MoveUntil([=](const Tile& tile) {return tile.get_tile_type() == Tile::Tile_Type::RailRoad; }, ind);
	if (dynamic_cast<RailRoad*>(&game.get_tile(ind))->get_ownership() != nullptr)
	{
		game.get_player(ind).pay(game.RailroadsCount(*dynamic_cast<RailRoad*>(&game.get_tile(ind))->get_ownership()) * 100);
		dynamic_cast<RailRoad*>(&game.get_tile(ind))->get_ownership()->get(game.RailroadsCount(*dynamic_cast<RailRoad*>(&game.get_tile(ind))->get_ownership()) * 100);
	}
}

void PayForHousesActionCommun::invoke(Game& game, unsigned char ind)
{
	pair<unsigned short, unsigned short>p = game.CountOfHousesAndHotels(game.get_player(ind));
	game.get_player(ind).pay(p.first * 40 + p.second * 115);
}

void PayForHousesActionChance::invoke(Game& game, unsigned char ind)
{
	pair<unsigned short, unsigned short>p = game.CountOfHousesAndHotels(game.get_player(ind));
	game.get_player(ind).pay(p.first * 25 + p.second * 100);
}