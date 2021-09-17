#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	float windowWidth = 800.f;
	float windowHeight = 600.f;
	
	float paddleHeight = 20.f;
	float paddleWidth = 100.f;
	float ballRadius = 10.f;

	sf::RectangleShape leftPaddle;
	leftPaddle.setSize(sf::Vector2f(paddleHeight, paddleWidth));
	leftPaddle.setFillColor(sf::Color::Blue);
	leftPaddle.setOutlineColor(sf::Color::White);
	leftPaddle.setOutlineThickness(2.f);
	leftPaddle.setOrigin(sf::Vector2f(paddleHeight, paddleWidth) / 2.f);
	leftPaddle.setPosition(sf::Vector2f((paddleHeight / 2.f + 10.f), (windowHeight / 2.f)));

	sf::RectangleShape rightPaddle;
	rightPaddle.setSize(sf::Vector2f(paddleHeight, paddleWidth));
	rightPaddle.setFillColor(sf::Color::Red);
	rightPaddle.setOutlineColor(sf::Color::White);
	rightPaddle.setOutlineThickness(2.f);
	rightPaddle.setOrigin(sf::Vector2f(paddleHeight, paddleWidth) / 2.f);
	rightPaddle.setPosition(sf::Vector2f((windowWidth - (paddleHeight / 2.f + 10.f)), (windowHeight / 2.f)));

	sf::CircleShape ball;
	ball.setRadius(ballRadius);
	ball.setFillColor(sf::Color::Cyan);
	ball.setOutlineColor(sf::Color::White);
	ball.setOutlineThickness(2.f);
	ball.setOrigin(sf::Vector2f(ballRadius, ballRadius) / 2.f);
	ball.setPosition(sf::Vector2f(windowWidth, windowHeight) / 2.f);

	sf::RectangleShape middleLine;
	middleLine.setSize(sf::Vector2f(2.f, windowHeight));
	middleLine.setFillColor(sf::Color::White);
	middleLine.setOrigin(sf::Vector2f(1.f, (windowHeight / 2.f)));
	middleLine.setPosition(sf::Vector2f(windowWidth / 2.f, windowHeight / 2.f));

	sf::SoundBuffer ballSoundbuffer;
	if (!ballSoundbuffer.loadFromFile("ball.wav")) std::cout << "Faild to load font" << std::endl;

	sf::Sound ballSound(ballSoundbuffer);
	ballSound.setVolume(15);

	sf::Font textFont;
	if (!textFont.loadFromFile("sansation.ttf")) std::cout << "Failed to load font" << std::endl;

	int leftScore = 0;
	int rightScore = 0;

	sf::Text leftText;
	leftText.setFont(textFont);
	leftText.setFillColor(sf::Color::White);
	leftText.setCharacterSize(40);
	leftText.setString(std::to_string(leftScore));
	leftText.setOrigin(sf::Vector2f((leftText.getGlobalBounds().width / 2.f), (leftText.getGlobalBounds().height / 2.f)));
	leftText.setPosition(sf::Vector2f(windowWidth / 2.f - 30.f, 15.f));

	sf::Text rightText;
	rightText.setFont(textFont);
	rightText.setFillColor(sf::Color::White);
	rightText.setCharacterSize(40);
	rightText.setString(std::to_string(rightScore));
	rightText.setOrigin(sf::Vector2f((rightText.getGlobalBounds().width / 2.f), (rightText.getGlobalBounds().height / 2.f)));
	rightText.setPosition(sf::Vector2f(windowWidth / 2.f + 30.f, 15.f));

	sf::Text pauseText;
	pauseText.setFont(textFont);
	pauseText.setFillColor(sf::Color::Green);
	pauseText.setCharacterSize(60);
	pauseText.setString("Game Paused!");
	pauseText.setOrigin(sf::Vector2f((pauseText.getGlobalBounds().width / 2.f), (pauseText.getGlobalBounds().height / 2.f)));
	pauseText.setPosition(sf::Vector2f((windowWidth / 2.f), (windowHeight / 2.f)));

	sf::Text leftWins;
	leftWins.setFont(textFont);
	leftWins.setFillColor(sf::Color::Blue);
	leftWins.setCharacterSize(60);
	leftWins.setString("Blue Wins");
	leftWins.setOrigin(sf::Vector2f((leftWins.getGlobalBounds().width / 2.f), (leftWins.getGlobalBounds().height / 2.f)));
	leftWins.setPosition(sf::Vector2f((windowWidth / 2.f), (windowHeight / 2.f)));

	sf::Text rightWins;
	rightWins.setFont(textFont);
	rightWins.setFillColor(sf::Color::Red);
	rightWins.setCharacterSize(60);
	rightWins.setString("Red Wins");
	rightWins.setOrigin(sf::Vector2f((rightWins.getGlobalBounds().width / 2.f), (rightWins.getGlobalBounds().height / 2.f)));
	rightWins.setPosition(sf::Vector2f((windowWidth / 2.f), (windowHeight / 2.f)));


	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Pong P1", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);
	sf::Event ev;

	bool isplaying = true;
	bool won = false;
	bool drawLeft = false;
	bool drawRight = false;

	float ballSpeed = 5.f;
	float ballX = -ballSpeed;
	float ballY = ballSpeed;
	

	// Gameloop
	while (window.isOpen())
	{
		// Event handling
		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Space) isplaying = !isplaying;
				if (ev.key.code == sf::Keyboard::R)
				{
					won = false;
					drawLeft = false;
					drawRight = false;

					ball.setPosition(sf::Vector2f(windowWidth, windowHeight) / 2.f);
					ballX = -ballSpeed;
					ballY = ballSpeed;

					leftScore = 0;
					rightScore = 0;

					leftText.setString(std::to_string(leftScore));
					rightText.setString(std::to_string(rightScore));

					leftPaddle.setPosition(sf::Vector2f((paddleHeight / 2.f + 10.f), (windowHeight / 2.f)));
					rightPaddle.setPosition(sf::Vector2f((windowWidth - (paddleHeight / 2.f + 10.f)), (windowHeight / 2.f)));
				}
				break;
			default:
				break;
			}
		}

		// GAME LOGIC

		float leftPaddleX = leftPaddle.getPosition().x + (paddleHeight / 2.f);
		float rightPaddleX = rightPaddle.getPosition().x - (paddleHeight / 2.f);

		float lpu = leftPaddle.getPosition().y - (paddleWidth / 2.f + 15.f);
		float lpm = leftPaddle.getPosition().y;
		float lpd = leftPaddle.getPosition().y + (paddleWidth / 2.f + 15.f);

		float rpu = rightPaddle.getPosition().y - (paddleWidth / 2.f + 15.f);
		float rpm = rightPaddle.getPosition().y;
		float rpd = rightPaddle.getPosition().y + (paddleWidth / 2 + 15.f);

		if (isplaying && !won)
		{
			// Left paddle movement
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && ((leftPaddle.getPosition().y - (paddleWidth / 2.f)) > 0))
			{
				leftPaddle.move(0.f, -5.f);
				// Save the new position of the left paddle
				lpu = leftPaddle.getPosition().y - (paddleWidth / 2);
				lpm = leftPaddle.getPosition().y;
				lpd = leftPaddle.getPosition().y + (paddleWidth / 2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && ((leftPaddle.getPosition().y + (paddleWidth / 2.f)) < windowHeight))
			{
				leftPaddle.move(0.f, 5.f);
				// Save the left paddle's new position
				lpu = leftPaddle.getPosition().y - (paddleWidth / 2);
				lpm = leftPaddle.getPosition().y;
				lpd = leftPaddle.getPosition().y + (paddleWidth / 2);
			}

			// Right paddle movement
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && ((rightPaddle.getPosition().y - (paddleWidth / 2.f)) > 0))
			{
				rightPaddle.move(0.f, -5.f);
				// Save the right paddle's new position
				rpu = rightPaddle.getPosition().y - (paddleWidth / 2);
				rpm = rightPaddle.getPosition().y;
				rpd = rightPaddle.getPosition().y + (paddleWidth / 2);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && ((rightPaddle.getPosition().y + (paddleWidth / 2.f)) < windowHeight))
			{
				rightPaddle.move(0.f, 5.f);
				// Save the right paddle's new position
				rpu = rightPaddle.getPosition().y - (paddleWidth / 2);
				rpm = rightPaddle.getPosition().y;
				rpd = rightPaddle.getPosition().y + (paddleWidth / 2);
			}

			// Ball Logic

			// Scoring
			// Left side scoring
			if (ball.getPosition().x >= windowWidth) leftScore += 1;

			// Right side scoring
			if (ball.getPosition().x <= 0.f) rightScore += 1;

			// Reset position if ball goes outside the screen
			if ((ball.getPosition().x >= windowWidth) || (ball.getPosition().x <= 0.f))
			{
				ball.setPosition(sf::Vector2f(windowWidth, windowHeight) / 2.f);
				ballX = -ballSpeed;
				ballY = ballSpeed;
				
				leftText.setString(std::to_string(leftScore));
				rightText.setString(std::to_string(rightScore));
			}

			// Hit down wall physics
			if ((ball.getPosition().y + ball.getRadius() + 7.f) >= windowHeight)
			{
				ballY = -3.f;
				ballSound.play();
			}

			// Hit up wall
			if (((ball.getPosition().y - ball.getRadius()) - 7.f) <= 0)
			{
				ballY = 3.f;
				ballSound.play();
			}

			// Ball collision with paddles
			// Left Paddle
			if (((ball.getPosition().x - ballRadius) == leftPaddleX) && (ball.getPosition().y >= lpu && ball.getPosition().y <= lpm)) // Upper side of the paddle 
			{
				ballX = ballSpeed;
				ballY = -ballSpeed;
				ballSound.play();
			}
			else if (((ball.getPosition().x - ballRadius) == leftPaddleX) && (ball.getPosition().y <= lpd && ball.getPosition().y >= lpm)) // Lower side of the paddle
			{
				ballX = ballSpeed;
				ballY = ballSpeed;
				ballSound.play();
			}
			else if (((ball.getPosition().x - ballRadius) == leftPaddleX) && (ball.getPosition().y == rpm)) // Middle of the paddle
			{
				ballX = -ballSpeed;
				ballY = 0.f;
				ballSound.play();
			}

			// Right Paddle
			if (((ball.getPosition().x + ballRadius) == rightPaddleX) && (ball.getPosition().y >= rpu && ball.getPosition().y <= rpm)) // Upper side of the paddle
			{
				ballX = -ballSpeed;
				ballY = -ballSpeed;
				ballSound.play();
			}
			else if (((ball.getPosition().x + ballRadius) == rightPaddleX) && (ball.getPosition().y <= rpd && ball.getPosition().y >= rpm)) // Lower side of the paddle
			{
				ballX = -ballSpeed;
				ballY = ballSpeed;
				ballSound.play();
			}
			else if (((ball.getPosition().x + ballRadius) == rightPaddleX) && (ball.getPosition().y == rpm)) // Middle of the paddle
			{
				ballX = -ballSpeed;
				ballY = 0.f;
				ballSound.play();
			}


			// Winning
			if (leftScore >= 5 || rightScore >= 5)
			{
				if (leftScore >= 5)
				{
					drawLeft = true;
				}

				if (rightScore >= 5)
				{
					drawRight = true;
				}

				won = true;
			}

			// Move ball
			sf::Vector2f ballVelocity(ballX, ballY);
			ball.move(ballVelocity);
			
		}
		

		// RENDERER

		// Clear previous frame
		window.clear(sf::Color::Black);
		
		// Draw objects
		window.draw(middleLine);
		window.draw(leftText);
		window.draw(rightText);
		window.draw(leftPaddle);
		window.draw(rightPaddle);
		window.draw(ball);

		if (!isplaying) window.draw(pauseText);
		if (drawLeft) window.draw(leftWins);
		if (drawRight) window.draw(rightWins);

		// Display frame
		window.display();
	}
}