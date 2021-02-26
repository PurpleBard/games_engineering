#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;
using namespace std;

const Keyboard::Key controls[5] = {
    Keyboard::W,   // Player1 UP
    Keyboard::S,   // Player1 Down
    Keyboard::Up,  // Player2 UP
    Keyboard::Down, // Player2 Down
    Keyboard::Enter
};
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;

int p1Count = 0;
int p2Count = 0;
Vector2f ballVelocity;
bool server = false;
bool swapMode = false;

CircleShape ball;
RectangleShape paddles[2];

sf::Font font;
sf::Text text;
sf::Text text2;

void Load() {
    // Set size and origin of paddles
    for (auto& p : paddles) {
        p.setSize(paddleSize - Vector2f(3, 3));
        p.setOrigin(paddleSize / 2.f);
    }
    // Set size and origin of ball
    ball.setRadius(ballRadius - 3);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);
    // reset paddle position
    paddles[0].setPosition(10 + paddleSize.x / 2, gameHeight / 2);
    paddles[1].setPosition(790 - paddleSize.x / 2, gameHeight / 2);
    // reset Ball Position
    ball.setPosition(gameWidth / 2, gameHeight / 2);

    //Set default velocity
    ballVelocity = { (server ? 100.0f : -100.0f), 60.0f };

    // Load font-face from res dir
    font.loadFromFile("res/img/invaders_sheet.png");
    // Set text element to use font
    text.setFont(font);
    // set the character size to 24 pixels
    text.setCharacterSize(24);

    // Set text element to use font
    text2.setFont(font);
    // set the character size to 24 pixels
    text2.setCharacterSize(24);
}

void Reset() 
{

    // reset paddle position
    paddles[0].setPosition(10 + paddleSize.x / 2, gameHeight / 2);
    paddles[1].setPosition(790 - paddleSize.x / 2, gameHeight / 2);
    // reset Ball Position
    ball.setPosition(gameWidth / 2, gameHeight / 2);

    //Set default velocity
    ballVelocity = { (server ? 100.0f : -100.0f), 60.0f };

    // Update Score Text
    text.setString(std::to_string(p1Count));
    // Keep Score Text Centered
    text.setPosition((gameWidth * .5f) - (text.getLocalBounds().width * .5f * 5), 0);

    // Update Score Text
    text2.setString(std::to_string(p2Count));
    // Keep Score Text Centered
    text2.setPosition((gameWidth * .5f) + (text.getLocalBounds().width * .5f * 5), 0);

}

void Update(RenderWindow& window) {
    // Reset clock, recalculate deltatime
    static Clock clock;
    float dt = clock.restart().asSeconds();
    // check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // handle paddle movement
    float direction = 0.0f;
    if (Keyboard::isKeyPressed(controls[0])) {
        direction--;
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction++;
    }

    float direction2p = 0.0f;

    if (swapMode == false) {
        if (Keyboard::isKeyPressed(controls[2])) {
            direction2p--;
        }
        if (Keyboard::isKeyPressed(controls[3])) {
            direction2p++;
        }
    }

    //Change gamemode upon pressing enter
    if (Keyboard::isKeyPressed(controls[4])) {

        if (swapMode) {
            swapMode = false;
        }
        else {
            swapMode = true;
        }
        p1Count = 0;
        p2Count = 0;
        Reset();
    }

    //Handle ball movement
    ball.move(ballVelocity * dt);

    // check ball collision
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by > gameHeight) { //bottom wall
      // bottom wall
        ballVelocity.x *= 1.1f;
        ballVelocity.y *= -1.1f;
        ball.move(0, -10);
    }
    else if (by < 0) { //top wall
   // top wall
        ballVelocity.x *= 1.1f;
        ballVelocity.y *= -1.1f;
        ball.move(0, 10);
    }
    else if (bx > gameWidth) {
        // right wall
        p1Count++;
        Reset();
    }
    else if (bx < 0) {
        // left wall
        p2Count++;
        Reset();
    }
    else if (
        //ball is inline or behind paddle
        bx < paddleSize.x &&
        //AND ball is below top edge of paddle
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
        //AND ball is above bottom edge of paddle
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5)
        ) {
        // bounce off left paddle
        ballVelocity.x *= -1.1f;
        ballVelocity.y *= 1.1f;
        ball.move(10, 0);
    }
    else if (//ball is inline or behind paddle
        bx > 800 - paddleSize.x &&
        //AND ball is below top edge of paddle
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
        //AND ball is above bottom edge of paddle
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5)) {
        // bounce off right paddle
        ballVelocity.x *= -1.1f;
        ballVelocity.y *= 1.1f;
        ball.move(-10, 0);
    }

    //store paddle position
    const float py = paddles[0].getPosition().y;
    const float py2 = paddles[1].getPosition().y;

    //If left paddle reaches top or bottom of screen, don't allow movement further in that direction
    if (py + paddleSize.y / 2 > gameHeight) {

        if (direction == 1.0f) {
            direction = 0.0f;
        }
    }

    if (py - paddleSize.y / 2 < 0) {

        if (direction == -1.0f) {
            direction = 0.0f;
        }
    }

    //If AI mode is on, move paddle automatically towards ball
    if (swapMode) {
        if (py2 < by) {
            direction2p = 1.0f;
        }
        if (py2 > by) {
            direction2p = -1.0f;
        }
    }

    //If right paddle reaches top or bottom of screen, don't allow movement further in that direction
    if (py2 + paddleSize.y / 2 > gameHeight) {

        if (direction2p == 1.0f) {
            direction2p = 0.0f;
        }
    }

    if (py2 - paddleSize.y / 2 < 0) {

        if (direction2p == -1.0f) {
            direction2p = 0.0f;
        }
    }

    paddles[1].move(0, direction2p * paddleSpeed * dt);
    paddles[0].move(0, direction * paddleSpeed * dt);
}
void Render(RenderWindow& window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
    window.draw(text);
    window.draw(text2);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}