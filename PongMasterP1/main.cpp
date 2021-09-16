#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
	bool debug = false;
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
	rightPaddle.setPosition(sf::Vector2f((windowWidth - (paddleHeight / 2.f + 10.f)), (windowHeight / 2)));

	sf::CircleShape ball;
	ball.setRadius(ballRadius);
	ball.setFillColor(sf::Color::Cyan);
	ball.setOutlineColor(sf::Color::White);
	ball.setOutlineThickness(2.f);
	ball.setOrigin(sf::Vector2f(ballRadius, ballRadius) / 2.f);
	ball.setPosition(sf::Vector2f(windowWidth, windowHeight) / 2.f);

	sf::SoundBuffer ballSoundbuffer;
	if (!ballSoundbuffer.loadFromFile("ball.wav")) std::cout << "Faild to load font" << std::endl;

	sf::Sound ballSound(ballSoundbuffer);
	ballSound.setVolume(15);

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Pong P1", sf::Style::Titlebar | sf::Style::Close);
	window.setFramerateLimit(60);
	sf::Event ev;
	bool isplaying = true;

	float ballSpeed = 3;
	float ballX = -ballSpeed;
	float ballY = ballSpeed;

	float leftPaddleX = leftPaddle.getPosition().x + (paddleHeight / 2);
	float rightPaddleX = rightPaddle.getPosition().x - (paddleHeight / 2);

	float lpu = leftPaddle.getPosition().y - (paddleWidth / 2);
	float lpm = leftPaddle.getPosition().y;
	float lpd = leftPaddle.getPosition().y + (paddleWidth / 2);

	float rpu = rightPaddle.getPosition().y - (paddleWidth / 2);
	float rpm = rightPaddle.getPosition().y;
	float rpd = rightPaddle.getPosition().y + (paddleWidth / 2);
	

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
				break;
			default:
				break;
			}
		}

		// GAME LOGIC
		if (isplaying)
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

			// Reset position if ball goes outside the screen
			if ((ball.getPosition().x >= windowWidth) || (ball.getPosition().x <= 0))
			{
				ball.setPosition(sf::Vector2f(windowWidth, windowHeight) / 2.f);
				ballX = -ballSpeed;
				ballY = ballSpeed;
			}
			// Hit down wall
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


			// Move ball
			sf::Vector2f ballVelocity(ballX, ballY);

			ball.move(ballVelocity);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				
			}
			
		}
		

		// RENDERER

		// Clear previous frame
		window.clear(sf::Color::Black);
		
		// Draw objects
		window.draw(leftPaddle);
		window.draw(rightPaddle);
		window.draw(ball);

		// Display frame
		window.display();
	}
}