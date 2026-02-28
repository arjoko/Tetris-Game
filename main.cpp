#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

const int M = 20; // height
const int N = 10; // width

int field[M][N] = {0};

struct Point { int x, y; } a[4], b[4];

// Tetromino shapes
int figures[7][4] = {
    {1,3,5,7}, // I
    {2,4,5,7}, // Z
    {3,5,4,6}, // S
    {3,5,4,7}, // T
    {2,3,5,7}, // L
    {3,5,7,6}, // J
    {2,3,4,5}, // O
};

bool check()
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return false;
        else if (field[a[i].y][a[i].x]) return false;
    }
    return true;
}

int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode(N*30, M*30), "Minimal Tetris");
    sf::RectangleShape block(sf::Vector2f(29,29));
    block.setFillColor(sf::Color::Green);

    int dx = 0; bool rotate = false; int colorNum = 1;
    float timer = 0, delay = 0.3;
    sf::Clock clock;

    int n = rand() % 7;
    for (int i = 0; i < 4; i++)
    {
        a[i].x = figures[n][i] % 2;
        a[i].y = figures[n][i] / 2;
    }

    while (window.isOpen())
    {
        float time = clock.restart().asSeconds();
        timer += time;

        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed) window.close();
            if (e.type == sf::Event::KeyPressed)
            {
                if (e.key.code == sf::Keyboard::Up) rotate = true;
                else if (e.key.code == sf::Keyboard::Left) dx = -1;
                else if (e.key.code == sf::Keyboard::Right) dx = 1;
                else if (e.key.code == sf::Keyboard::Down) delay = 0.05;
            }
        }

        // Move
        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
        if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

        // Rotate
        if (rotate)
        {
            Point p = a[1]; // center
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - p.y;
                int y = a[i].x - p.x;
                a[i].x = p.x - x;
                a[i].y = p.y + y;
            }
            if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];
        }

        // Tick
        if (timer > delay)
        {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
            if (!check())
            {
                for (int i = 0; i < 4; i++) field[b[i].y][b[i].x] = colorNum;

                n = rand() % 7; colorNum = 1;
                for (int i = 0; i < 4; i++)
                {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }
            timer = 0;
        }

        dx = 0; rotate = false; delay = 0.3;

        // Line clearing
        int k = M-1;
        for (int i = M-1; i >= 0; i--)
        {
            int count = 0;
            for (int j = 0; j < N; j++)
            {
                if (field[i][j]) count++;
                field[k][j] = field[i][j];
            }
            if (count < N) k--;
        }

        // Draw
        window.clear(sf::Color::Black);

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (field[i][j])
                {
                    block.setPosition(j*30, i*30);
                    window.draw(block);
                }
            }

        for (int i = 0; i < 4; i++)
        {
            block.setPosition(a[i].x*30, a[i].y*30);
            window.draw(block);
        }

        window.display();
    }

    return 0;
}
