#include "AI.h"

PlayerAI::PlayerAI(Player& player) : m_Player(player) {

}

bool PlayerAI::shouldbuy(PropertyTile& tile) {
	if (tile.get_OwnerShip() == nullptr && m_Player.get_money() - tile.get_Price() > 800)
		return true;
	return false;
}