#include <SFML/Graphics.hpp>
#include <random>
#include "Game.h"
#include "Action.h"
using namespace std;
using namespace sf;
int main()
{
    Game game(4);

    random_device rd;     // Only used once to initialise (seed) engine
    mt19937 rng(rd());    // Random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(1, 6);// Guaranteed unbiased
    unsigned char current_player = 0;
    int a = 0;
    while (true)
    {
        unsigned char dice1 = uni(rng);
        unsigned char dice2 = uni(rng);
        game.MovePlayer(dice1, dice2, current_player);
        a++;;
        game.log(cout, current_player, a);
        unsigned char t = game.CheckingForNischeta(current_player);
        if (game.GetPlayersAmount() <= 1)
        {
            cout << "Game Over";
            break;
        }
        current_player = t;
    }


























    /*sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!",Style::Fullscreen);
    sf::Texture texture;
    texture.loadFromFile("image.jpg");
    sf::Sprite sprite;
    sprite.setTexture(texture);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }











    return 0;*/
}