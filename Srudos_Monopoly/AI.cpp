#include"AI.h"
#include"Player.h"
AI::AI(Player& p)
	:m_player(p)
{
}

First_AI::First_AI(Player& p)
	: AI(p)
{
}

Second_AI::Second_AI(Player& p)
	: AI(p)
{
}
Player& AI::get_player() const
{
	return m_player;
}

//First_AI:Have Enough Money - BUY!
bool First_AI::should_buy(PropertyTile& prop_tile) const
{
	if (get_player().skokadenegnischeta() >= prop_tile.get_price())
		return true;
	return false;
}

bool First_AI::jail_break(void) const
{
	if (get_player().skokadenegnischeta() >= 50)
		return true;
	return false;
}

bool First_AI::should_build(Street& s) const
{
	if ((*this).get_player().skokadenegnischeta() >= ((*this).get_player().get_pos() / 10 + 1) * 50)
		return true;
	return false;
}

void First_AI::pledge(PropertyTile& prop_tile) const
{
}



//Second_AI:Money>price+500
bool Second_AI::should_buy(PropertyTile& prop_tile) const
{
	if (get_player().skokadenegnischeta() >= (prop_tile.get_price() + 500))
		return true;
	return false;
}

bool Second_AI::jail_break(void) const
{
	if (get_player().skokadenegnischeta() >= 550)
		return true;
	return false;
}

bool Second_AI::should_build(Street&) const
{
	return 1;
}

void Second_AI::pledge(PropertyTile& prop_tile) const
{
}

