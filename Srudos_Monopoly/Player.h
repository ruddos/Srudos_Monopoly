#pragma once
#include "AI.h"
class Player
{
public:
	Player();
	const AI& get_AI();
	void pay(unsigned short sum);
	void get(unsigned short sum);
	int skokadenegnischeta();
	unsigned char get_pos();
private:
	unique_ptr<const AI> m_AI;
	int m_money;
	unsigned char m_pos;
};