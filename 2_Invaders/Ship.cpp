#include "Ship.h"
#include "Game.h"
#include "Bullet.h"
using namespace sf;
using namespace std;

bool Invader::direction;
float Invader::speed;



const Keyboard::Key controls[2] = {
    Keyboard::A,   // Player1 Right
    Keyboard::D,   // Player1 Left
};

Ship::Ship() {};

Ship::Ship(IntRect ir) : Sprite() 
{
    _sprite = ir;
    setTexture(spritesheet);
    setTextureRect(_sprite);
};

void Ship::Update(const float& dt) {}

//Define the ship deconstructor. 
//Although we set this to pure virtual, we still have to define it.
Ship::~Ship() = default;


Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir) 
{
    speed = 10.0f;
    setOrigin(16, 16);
    setPosition(pos);
}

void Invader::Update(const float& dt) 
{
    Ship::Update(dt);

    move(dt * (direction ? 1.0f : -1.0f) * speed, 0);

    if ((direction && getPosition().x > gameWidth - 16) ||
        (!direction && getPosition().x < 16)) 
    {
        direction = !direction;
        for (int i = 0; i < ships.size(); ++i) 
        {

            if(typeid(*ships[i]) == typeid(Invader))
            ships[i]->move(0, 24);
        }
    }
}

Player::Player() : Ship(IntRect(160, 32, 32, 32)) 
{
    setPosition({ gameHeight * .5f, gameHeight - 32.f });
}

void Player::Update(const float& dt) {
    Ship::Update(dt);

    static vector<Bullet*> bullets;

    // handle Ship movement
    float direction = 0.0f;
    if (Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }

    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        bullets.push_back(new Bullet(getPosition(), false));
    }
    for (const auto s : bullets) 
    {
        s->Update(dt);
    }

    //If Ship reaches left or right of screen, don't allow movement further in that direction
    if (getPosition().x / 2 > gameWidth / 2 - 16)
    {

        if (direction == 1.0f) {
            direction = 0.0f;
        }
    }

    if (getPosition().x / 2 < 0) 
    {

        if (direction == -1.0f) {
            direction = 0.0f;
        }
    }

    move(direction * 300.0f * dt, 0);
}

void Ship::Explode() 
{
    setTextureRect(IntRect(128, 32, 32, 32));
    _exploded = true;
}

void Ship::is_exploded()
{

}