#include "Player.h"

using namespace std;

Player::Player() {
}

void Player::pay(unsigned short sum)
{
	m_money -= sum;
}

short Player::get_money() {
	return m_money;
}

void Player::claim_money(unsigned short sum) {
	m_money += sum;
}