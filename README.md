//Tetris Game Development

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int width = 20;      // grid width
const int height = 20;     // grid height
const int blockSize = 25;  // pixel size

enum Direction { Up, Down, Left, Right };

struct SnakeSegment {
    int x, y;
};

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(width*blockSize, height*blockSize), "Snake SFML");
    window.setFramerateLimit(10); // initial speed

    // Snake setup
    vector<SnakeSegment> snake;
    snake.push_back({width/2, height/2});
    Direction dir = Right;

    // Food setup
    SnakeSegment food = {rand() % width, rand() % height};

    int score = 0;
    bool gameOver = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::W: if(dir!=Down) dir=Up; break;
                    case sf::Keyboard::S: if(dir!=Up) dir=Down; break;
                    case sf::Keyboard::A: if(dir!=Right) dir=Left; break;
                    case sf::Keyboard::D: if(dir!=Left) dir=Right; break;
                    default: break;
                }
            }
        }

        if (gameOver) continue;

        // Move snake
        SnakeSegment prevHead = snake[0];
        switch(dir) {
            case Up: prevHead.y--; break;
            case Down: prevHead.y++; break;
            case Left: prevHead.x--; break;
            case Right: prevHead.x++; break;
        }

        // Wall collision
        if(prevHead.x<0 || prevHead.x>=width || prevHead.y<0 || prevHead.y>=height)
        {
            gameOver = true;
        }

        // Self collision
        for(auto &seg : snake)
            if(seg.x==prevHead.x && seg.y==prevHead.y)
                gameOver=true;

        if(gameOver) continue;

        // Move tail
        snake.insert(snake.begin(), prevHead);

        // Eating food
        if(prevHead.x==food.x && prevHead.y==food.y)
        {
            score++;
            food = {rand()%width, rand()%height};
        }
        else
        {
            snake.pop_back(); // remove last segment if no food eaten
        }

        // Draw everything
        window.clear(sf::Color::Black);

        // Draw food
        sf::RectangleShape foodRect(sf::Vector2f(blockSize-2, blockSize-2));
        foodRect.setFillColor(sf::Color::Red);
        foodRect.setPosition(food.x*blockSize+1, food.y*blockSize+1);
        window.draw(foodRect);

        // Draw snake
        sf::RectangleShape snakeRect(sf::Vector2f(blockSize-2, blockSize-2));
        snakeRect.setFillColor(sf::Color::Green);
        for(auto &seg : snake)
        {
            snakeRect.setPosition(seg.x*blockSize+1, seg.y*blockSize+1);
            window.draw(snakeRect);
        }

        window.display();
    }

    printf("Game Over! Score: %d\n", score);
    return 0;
}
