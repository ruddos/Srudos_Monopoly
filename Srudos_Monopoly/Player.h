#pragma once

using namespace std;

class Player {
public:
	Player();

	void pay(unsigned short sum);
	void claim_money(unsigned short sum);
	short get_money();

private:
	short m_money = 1500;
};
