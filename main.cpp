#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>

const int M = 25; // height
const int N = 15; // width

int field[M][N] = {0};
sf::Color fieldColor[M][N]; // store colors for landed blocks

struct Point { int x, y; } a[4], b[4];

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

    sf::RenderWindow window(sf::VideoMode(N*30, M*30), "Tetris with Score");
    sf::RectangleShape block(sf::Vector2f(29,29));
    block.setFillColor(sf::Color::Blue);
    
    // Background image (optional)
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool hasBackground = false;

    if (backgroundTexture.loadFromFile("C:\\Users\\Acer\\Documents\\NewGameDevelopment\\Game_Tetris\\Tetris_1.jpg")) {
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            float(N*30) / backgroundTexture.getSize().x,
            float(M*30) / backgroundTexture.getSize().y
        );
        hasBackground = true;
    }
    
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf"); // fallback
    }
    		
    int dx = 6; bool rotate = false; int colorNum = 1;
    float timer = 0, delay = 0.3;
    sf::Clock clock;
    int score = 0;
    bool gameOver = false;
    
    // current piece color
    sf::Color currentColor(rand()%256, rand()%256, rand()%256);

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
            if (e.type == sf::Event::KeyPressed && !gameOver)
            {
                if (e.key.code == sf::Keyboard::Up) rotate = true;
                else if (e.key.code == sf::Keyboard::Left) dx = -1;
                else if (e.key.code == sf::Keyboard::Right) dx = 1;
                else if (e.key.code == sf::Keyboard::Down) delay = 0.05;
            }
        }

        if (!gameOver)
        {
            // Move
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
            if (!check()) for (int i = 0; i < 4; i++) a[i] = b[i];

            // Rotate
            if (rotate)
            {
                Point p = a[1];
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
        for (int i = 0; i < 4; i++)
        {
            field[b[i].y][b[i].x] = 1;
            fieldColor[b[i].y][b[i].x] = currentColor;
        }

        // Spawn new piece
        n = rand() % 7;
        currentColor = sf::Color(rand()%256, rand()%256, rand()%256);
        for (int i = 0; i < 4; i++)
        {
            a[i].x = figures[n][i] % 2;
            a[i].y = figures[n][i] / 2;
        }


        // Game Over check: if new piece collides immediately
        if (!check())
        {
            gameOver = true;
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
                    fieldColor[k][j] = fieldColor[i][j];
                }
                if (count < N) k--;
                else score += 100; // add points per cleared line
            }
        }

        // Draw
        window.clear();
        window.draw(backgroundSprite); // draw background first


        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (field[i][j])
                {
                    block.setFillColor(fieldColor[i][j]);
                    block.setPosition(j*30, i*30);
                    window.draw(block);
                }
            }

        if (!gameOver)
        {
            for (int i = 0; i < 4; i++)
            {
                block.setFillColor(currentColor);
                block.setPosition(a[i].x*30, a[i].y*30);
                window.draw(block);
            }

            std::ostringstream ss;
            ss << "Score: " << score;
            sf::Text scoreText(ss.str(), font, 30);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(5, 5);
            window.draw(scoreText);
        }
        else
        {
            sf::Text text("GAME OVER", font, 40);
            text.setFillColor(sf::Color::Red);
            text.setPosition(N*30/4, M*30/2 - 40);
            window.draw(text);

            std::ostringstream ss;
            ss << "Final Score: " << score;
            sf::Text scoreText(ss.str(), font, 30);
            scoreText.setFillColor(sf::Color::Yellow);
            scoreText.setPosition(N*30/4, M*30/2 + 20);
            window.draw(scoreText);
        }

        window.display();
    }

    return 0;
}
