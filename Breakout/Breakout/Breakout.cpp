#include "Framework.h"
#include "GameState.h"
#include "Definitions.h"
#include "Paddle.h"
#include "Ball.h"
#include "Bricks.h"
#include "GameElements.h"

GameState currentState;
void SetState(); // Allows me to debug and switch between states
int roundCount = 0;
int points;

template<class T1, class T2>
bool isIntersecting(T1& thisShape, T2& otherShape)
{
    return thisShape.right() >= otherShape.left() && thisShape.left() <= otherShape.right() &&
        thisShape.bottom() >= otherShape.top() && thisShape.top() <= otherShape.bottom();
}

void CheckCollisions(Paddle& _paddle, Ball& _ball)
{
    float _ballVelocity = 10.f;

    // If there's no intersection, return
    if (!isIntersecting(_paddle, _ball)) return;
    paddleSound.play();

    _ball.velocity.y = -_ball.velocity.y ;

    // Direct the balls direction depending on the position where it hit the paddle
    if (_ball.x() < _paddle.x())
        _ball.velocity.x = -_ballVelocity;
    else if(_ball.x() < _paddle.x())
        _ball.velocity.x = _ballVelocity;
    else
        _ball.velocity.x = _ballVelocity - 5;
}

void BrickCollisions(Bricks& _brick, Ball& _ball)
{
    float ballVelocity = 10.f;

    // If there's no intersection, return
    if (!isIntersecting(_brick, _ball)) return;

    // Otherwise, the brick has been hit!
    _brick.Destroyed = true;
    brickSound.play(); 
    points += 5;

    // Calculates when magnitude of the overlap between the ball and the brick
    float overlapLeft{ _ball.right() - _brick.left() };
    float overlapRight{ _brick.right() - _ball.left() };
    float overlapTop{ _ball.bottom() - _brick.top() };
    float overlapBottom{ _brick.bottom() - _ball.top() };

    // If the magnitude of the left overlap is smaller than the
    // right one we can safely assume the ball hit the brick
    // from the left
    bool ballFromLeft(abs(overlapLeft) < abs(overlapRight));

    // We can apply the same idea for top/bottom collisions
    bool ballFromTop(abs(overlapTop) < abs(overlapBottom));

    // Let's store the minimum overlaps for the X and Y axes
    float minOverlapX{ ballFromLeft ? overlapLeft : overlapRight };
    float minOverlapY{ ballFromTop ? overlapTop : overlapBottom };

    // If the magnitude of the X overlap is less than the magnitude
    // of the Y overlap, we can safely assume the ball hit the brick
    // horizontally - otherwise, the ball hit the brick vertically

    // Then, upon our assumptions, we change either the X or Y velocity
    // of the ball, creating a "realistic" response for the collision
    if (abs(minOverlapX) < abs(minOverlapY))
        _ball.velocity.x = ballFromLeft ? -ballVelocity : ballVelocity;
    else
        _ball.velocity.y = ballFromTop ? -ballVelocity : ballVelocity;
}

int main()
{   
 // Graphical Elements can be hidden with their respective regions for readibility
 #pragma region Textures
    if (!redBlock.loadFromFile(IMAGE_PATH "red.png"))
        return EXIT_FAILURE;
    if (!orangeBlock.loadFromFile(IMAGE_PATH "orange.png"))
        return EXIT_FAILURE;
    if (!yellowBlock.loadFromFile(IMAGE_PATH "yellow.png"))
        return EXIT_FAILURE;
    if (!greenBlock.loadFromFile(IMAGE_PATH "green.png"))
        return EXIT_FAILURE;
    if (!menuOverlay.loadFromFile(IMAGE_PATH "MainScreen.png"))
        return EXIT_FAILURE;
    if (!gameOverlay.loadFromFile(IMAGE_PATH "GameOverlay.png"))
        return EXIT_FAILURE;

    // Paddle Texture
    Texture paddleTexture;

    if (!paddleTexture.loadFromFile(IMAGE_PATH "paddle.png"))
        return EXIT_FAILURE;
    paddleTexture.setSmooth(false);

    // Ball Texture
    Texture ballTexture;
    if (!ballTexture.loadFromFile(IMAGE_PATH "ball.png"))
        return EXIT_FAILURE;
    ballTexture.setSmooth(false);

#pragma endregion

#pragma region SoundBuffers
    if (!brickHit.loadFromFile(AUDIO_PATH "brickHit.wav"))
        return EXIT_FAILURE;
    if (!gameOver.loadFromFile(AUDIO_PATH "gameOver.wav"))
        return EXIT_FAILURE;
    if (!loseLife.loadFromFile(AUDIO_PATH "hurt.wav"))
        return EXIT_FAILURE;
    if (!paddleHit.loadFromFile(AUDIO_PATH "paddleHit.wav"))
        return EXIT_FAILURE;
    if (!roundComplete.loadFromFile(AUDIO_PATH "roundWin.wav"))
        return EXIT_FAILURE;


#pragma endregion

#pragma region Sounds
    brickSound.setBuffer(brickHit); brickSound.setVolume(50);
    wallSound.setBuffer(wallHit); wallSound.setVolume(50);
    paddleSound.setBuffer(paddleHit); wallSound.setVolume(50);
    lifeSound.setBuffer(loseLife); lifeSound.setVolume(50);
    pointSound.setBuffer(gainPoints); pointSound.setVolume(50);
    gameOverSound.setBuffer(gameOver); gameOverSound.setVolume(50);
    winSound.setBuffer(roundComplete); winSound.setVolume(50);
#pragma endregion

#pragma region Font
    if (!PressStart.loadFromFile(FONT_PATH "PressStart2P.ttf"))
        return EXIT_FAILURE;
#pragma endregion

#pragma region Text
  /*  stateIndicator.setPosition(0, 10);
    stateIndicator.setFont(PressStart); 
    stateIndicator.setFillColor(Color::White);
    stateIndicator.setCharacterSize(14);
    stateIndicator.setString("Current State: ");*/

  /*  stateText.setPosition(stateIndicator.getPosition().x + 210, stateIndicator.getPosition().y);
    stateText.setFont(PressStart);
    stateText.setFillColor(Color::Red); 
    stateText.setCharacterSize(16);*/

    startText.setPosition(450, 500);
    startText.setFont(PressStart);
    startText.setFillColor(Color::White);
    startText.setCharacterSize(16); 
    startText.setString("Press Enter To Start Game");

    livesText.setPosition(1100, 30);
    livesText.setFont(PressStart); 
    livesText.setFillColor(Color::White); 
    livesText.setCharacterSize(16);
    livesText.setString("Lives: ");

    scoreText.setPosition(600, 0);
    scoreText.setFont(PressStart);
    scoreText.setFillColor(Color::White);
    scoreText.setCharacterSize(40);

    livesCount.setPosition(1210, 30);
    livesCount.setFont(PressStart);
    livesCount.setFillColor(Color::Yellow);
    livesCount.setCharacterSize(16);

    gameOverText.setPosition(500 , HEIGHT / 2);
    gameOverText.setFont(PressStart);
    gameOverText.setFillColor(Color::White); 
    gameOverText.setCharacterSize(30);
    gameOverText.setString("Game Over");

    pressR.setPosition(490, 420);
    pressR.setFont(PressStart);
    pressR.setFillColor(Color::Blue);
    pressR.setCharacterSize(16);
    pressR.setString("Press R to restart");

    roundText.setPosition(500, HEIGHT / 2);
    roundText.setFont(PressStart);
    roundText.setFillColor(Color::Red);
    roundText.setCharacterSize(30);

#pragma endregion

#pragma region Sprites
    Sprite MainMenuOverlay, GameOverlay;
    MainMenuOverlay.setTexture(menuOverlay);
    MainMenuOverlay.setPosition(0, 0);

    GameOverlay.setTexture(gameOverlay);
    MainMenuOverlay.setPosition(0, 0);

#pragma endregion

    Clock clock; // System Clock
    float deltaTime = 0.0f;
    float timerTextFloat = 1.0f;

    // Defines the window
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Breakout!", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    
    // Create Paddle
    Paddle playerPaddle(&paddleTexture, WIDTH /2 , 670);

    // Create Ball
    Ball ball{ 0,0 };
    ball.shape.setTexture(&ballTexture);

    float brickCountX = 20.f;
    float brickCountY = 4.f;
    float brickWidth = 60.f;
    float brickHeight = 20.f;

    // Create Bricks
    vector<Bricks> bricks;
    for (int x{ 0 }; x < brickCountX; x++)
        for (int y{ 0 }; y < brickCountY; y++)
            bricks.emplace_back(
                (x + 0.8f) * (brickWidth + 2), (y + 2) * (brickHeight + 20));

    while (window.isOpen() && !Keyboard::isKeyPressed(Keyboard::Escape))
    {
        Event event;
        while (window.pollEvent(event))
        {          
            SetState();
            deltaTime = clock.getElapsedTime().asSeconds();
            clock.restart();
            
            // State Machine Switch
            switch (currentState) {
            case 0: // Menu
                
                //stateText.setString("Menu");
                window.draw(MainMenuOverlay);
                if (Keyboard::isKeyPressed(Keyboard::Enter)) {
                    currentState = RoundStart;
                    ball.lives = 4;
                    roundCount++;
                }
                break;
            case 1: // Aim
                
                //stateText.setString("Aiming");

                // Wait for player input to start playing
                ball.shape.setPosition(playerPaddle.shape.getPosition().x, playerPaddle.shape.getPosition().y - 40); // Sets the ball to follow the paddle
                if (Keyboard::isKeyPressed(Keyboard::Space) || Mouse::isButtonPressed(Mouse::Left)) {
                    currentState = Playing;
                }
                break;
            case 2: // Round Start    

               // stateText.setString("Round Start");
                bricks.clear(); // Clear bricks vector

                //Init bricks
                for (int x{ 0 }; x < brickCountX; x++)
                    for (int y{ 0 }; y < brickCountY; y++)
                        bricks.emplace_back(
                            (x + 0.8f) * (brickWidth + 2), (y + 2) * (brickHeight + 20));
                break;
            case 3: // Playing
                
               // stateText.setString("Playing");
                break;
            case 4: // Pause
                
                //stateText.setString("Round Ended");
                break;
            case 5: // Out Of Lives
                //stateText.setString("Out Of Lives");
                break;
            case 6: // Switch Rounds                
                //stateText.setString("Round Over");
                break;
            }
            if (event.type == Event::Closed)
                window.close();          
        }

        //Updating Text
        auto _points = to_string(points);
        scoreText.setString(_points);

        auto _lives = to_string(ball.lives);
        livesCount.setString(_lives);

        auto _rounds = to_string(roundCount);

        //Update Ball, Paddle, Bricks Vector, Game State and other graphical elements
        window.clear();
        window.draw(GameOverlay);
        playerPaddle.draw(window);
        ball.Draw(window);        
        window.draw(livesText);    
        window.draw(livesCount);
        window.draw(scoreText);
      
       //Draw each 'brick' in bricks and set the texture of the brick based on its height
        for (auto& brick : bricks) {
            if (brick.shape.getPosition().y == 80)
                brick.shape.setTexture(&redBlock);

            if (brick.shape.getPosition().y == 120)
                brick.shape.setTexture(&orangeBlock);

            if (brick.shape.getPosition().y == 160)
                brick.shape.setTexture(&yellowBlock);

            if (brick.shape.getPosition().y ==200)
                brick.shape.setTexture(&greenBlock);

            brick.Draw(window);
        }

        if (currentState == Menu) {
            window.draw(MainMenuOverlay);
            window.draw(startText);
        }

        if (currentState == Aiming) {
            startText.setString("Press Space or LMB to Launch");
            window.draw(startText);
            ball.shape.setPosition(playerPaddle.shape.getPosition().x, playerPaddle.shape.getPosition().y - 40); // Sets the ball to follow the paddle
            playerPaddle.Update(deltaTime, window);
        }

        if (currentState == RoundStart)
        {
            if (roundCount == 0) {
                roundCount = 1;
            }

            if (timerTextFloat >= 0)
            {
                timerTextFloat -= 0.01f;
            }

            if (timerTextFloat <= 0.1f) {
                currentState = Aiming;
            }

            roundText.setString("Round " + _rounds);
            window.draw(roundText);
        }

        if (currentState == Playing) {
                     

            ball.Update(deltaTime);
            playerPaddle.Update(deltaTime, window);
            CheckCollisions(playerPaddle, ball);
            currentState = ball.state;
            for (auto& brick : bricks) BrickCollisions(brick, ball);

            // And we use the "erase-remove idiom" to remove all `destroyed`
            // blocks from the block vector - using a cool C++11 lambda!
            bricks.erase(remove_if(begin(bricks), end(bricks),
                [](const Bricks& mBrick)
            {
                return mBrick.Destroyed;
            }),
                end(bricks));

            ball.Draw(window);
            playerPaddle.draw(window);
            for (auto& brick : bricks) window.draw(brick.shape);
           

            // If the player reaches the max score switch rounds 
            if (points >= SCORE_MAX) {
                currentState = Win;
            }
        }

        if (currentState == OutOfLives) {
            
            window.draw(gameOverText);
            window.draw(pressR);

            if (Keyboard::isKeyPressed(Keyboard::R)) {
                gameOverSound.play();
                currentState = RoundStart;
                ball.lives = 3;
                points = 0;
                roundCount = 0;
                timerTextFloat = 1;
                bricks.clear();
            }
        }

        if (currentState == Win) {
            winSound.play();
            ball.ballVelocity += 10;
            points = 0; //Reset Score
            roundCount+=1;
            currentState = RoundStart;
            timerTextFloat = 1;
        }

        window.draw(stateIndicator);
        window.draw(stateText);
        window.display();
    }
    return 0;
}

// Allows me to debug and switch between states
void SetState() {

    if (Keyboard::isKeyPressed(Keyboard::Num1))
        currentState = Menu;

    if (Keyboard::isKeyPressed(Keyboard::Num2))
        currentState = Aiming;

    if (Keyboard::isKeyPressed(Keyboard::Num3))
        currentState = RoundStart;

    if (Keyboard::isKeyPressed(Keyboard::Num4))
        currentState = Playing;

    if (Keyboard::isKeyPressed(Keyboard::Num5))
        currentState = OutOfLives;

    if (Keyboard::isKeyPressed(Keyboard::Num6))
        currentState = Win;
}
