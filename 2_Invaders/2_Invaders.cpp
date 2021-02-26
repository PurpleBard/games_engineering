#include <SFML/Graphics.hpp>
#include <iostream>
#include "Ship.h"
#include "Game.h"

using namespace sf;
using namespace std;

sf::Texture spritesheet;
sf::Sprite invader;
std::vector<Ship*> ships;
float posx = 100.0f;
float posy = 100.0f;

void Load() 
{
    if (!spritesheet.loadFromFile("res/invaders_sheet.png")) {
        cerr << "Failed to load spritesheet!" << std::endl;
    }
    invader.setTexture(spritesheet);
    invader.setTextureRect(sf::IntRect(0, 0, 32, 32));

    Player* player = new Player();
    ships.push_back(player);

    Invader* inv = new Invader(sf::IntRect(0, 0, 32, 32), { 100,100 });
    ships.push_back(inv);

    for (int r = 0; r < invaders_rows; ++r) 
    {
        auto rect = IntRect(0, 0, 32, 32);
        for (int c = 0; c < invaders_columns; ++c) 
        {
            Vector2f position = { posx ,posy };
            posx += 50.0f;
            auto inv = new Invader(rect, position);
            ships.push_back(inv);
        }
        posx = 100.0f;
        posy += 50.0f;
    }
}

void Update(RenderWindow& window) 
{
    static Clock clock;
    float dt = clock.restart().asSeconds();
    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    for (auto& s : ships) {
        s->Update(dt);
    };
}

void Render(RenderWindow& window)
{

    for (const auto s : ships) {
        window.draw(*s);
    }
    
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "INVADERS");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}