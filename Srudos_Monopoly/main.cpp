//#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
#include "Game.h"
#include <random>

using namespace std;


int main() {
	Game game(4);

	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<unsigned short> uni(1, 6);

	unsigned char current_player = 0;
	while (true) {
		unsigned char dice1 = uni(rng);
		unsigned char dice2 = uni(rng);
		game.move_Player(current_player, dice1, dice2);

		unsigned char next = game.time_to_ban(current_player);

		current_player = next;
		if (game.get_players_count() == 1)
		{
			std::cout << "Game ended" << endl;
			break;
		}
	}
}










//int main()
//{
//    sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "DD3V");
//    sf::CircleShape shape(100.f);
//    shape.setFillColor(sf::Color::Cyan);
//
//    while (window.isOpen())
//    {
//        sf::Event event;
//        while (window.pollEvent(event))
//        {
//            if (event.type == sf::Event::Closed)
//                window.close();
//        }
//
//        window.clear();
//        window.draw(shape);
//        window.display();
//    }
//
//    return 0;
//}