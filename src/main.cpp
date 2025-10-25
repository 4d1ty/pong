// Its not fucking multiplayer bitch

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <ctime>
#include <iostream>
int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> degree_range(0, 360);

	auto window = sf::RenderWindow(sf::VideoMode::getFullscreenModes()[0], "PONG", sf::State::Fullscreen);

	auto [screenX, screenY] = window.getSize();

	sf::Music music("pong.wav");
	music.play();
	music.setLooping(true);
	sf::Font boldPixels("BoldPixels.ttf");

	unsigned int playerScore = 0;
	sf::Text playerScoreText(boldPixels);
	playerScoreText.setCharacterSize(128);
	playerScoreText.setString(std::to_string(playerScore));
	playerScoreText.setOrigin(playerScoreText.getGlobalBounds().getCenter());
	playerScoreText.setPosition({ static_cast<float>(screenX / 2 - 500), 150.f });

	unsigned int computerScore = 0;
	sf::Text computerScoreText(boldPixels);
	computerScoreText.setCharacterSize(128);
	computerScoreText.setString(std::to_string(computerScore));
	computerScoreText.setOrigin(computerScoreText.getGlobalBounds().getCenter());
	computerScoreText.setPosition({ static_cast<float>(screenX / 2 + 500), 150.f });


	sf::Clock clock;

	sf::Vector2f paddleSize({ 40.f, 200.f });
	float speed = 1500.f;
	float computerSpeed = 500.f;

	sf::RectangleShape playerPaddle(paddleSize);
	// Initial Position of the playerPaddle.
	playerPaddle.setPosition({ 0.f, static_cast<float>(screenY / 2) });
	playerPaddle.setOrigin({ 0.f, playerPaddle.getGeometricCenter().y });


	sf::RectangleShape computerPaddle(paddleSize);
	computerPaddle.setPosition({ static_cast<float>(screenX - paddleSize.x), static_cast<float>(screenY / 2) });
	computerPaddle.setOrigin({ 0.f, computerPaddle.getGeometricCenter().y });

	sf::RectangleShape ball{ {50.f, 50.f} };
	ball.setOrigin(ball.getGeometricCenter());
	ball.setPosition({ static_cast<float>(screenX / 2), static_cast<float>(screenY / 2) });
	sf::Angle ballPhi = sf::degrees(degree_range(gen));
	sf::Vector2f ballVelocity({ 800.f, sf::Angle(ballPhi) });

	while (window.isOpen())
	{
		float dt = clock.restart().asSeconds();
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window.close();
			}
			if (const auto& keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
					window.close();
				}
			}
		}

		// Clamp the paddles position to be within the screen.
		float playerYPosition = playerPaddle.getPosition().y;
		float currentPlayerY = std::clamp(playerYPosition, paddleSize.y / 2, static_cast<float>(screenY - paddleSize.y / 2));
		float computerYPosition = computerPaddle.getPosition().y;
		float computerCurrentY = std::clamp(computerYPosition, paddleSize.y / 2, static_cast<float>(screenY - paddleSize.y / 2));

		playerPaddle.setPosition({ 0.f, currentPlayerY });
		computerPaddle.setPosition({ static_cast<float>(screenX - paddleSize.x), computerCurrentY });

		// Player movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
			playerPaddle.move({ 0.f, -speed * dt });
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
			playerPaddle.move({ 0.f, speed * dt });

		}

		ball.move(ballVelocity * dt);

		const sf::FloatRect ballBounds = ball.getGlobalBounds();
		const sf::FloatRect compBounds = computerPaddle.getGlobalBounds();
		const sf::FloatRect playerBounds = playerPaddle.getGlobalBounds();

		float playerCenterY = playerBounds.getCenter().y;
		float computerCenterY = compBounds.getCenter().y;
		float ballCenterY = ballBounds.getCenter().y;

		// Top and bottom screen collision
		if (ballBounds.position.y + ballBounds.size.y > screenY ||
			ballBounds.position.y < 0.f) {
			ballVelocity.y = -ballVelocity.y;
		}

		// Paddle Collision
		if (ballBounds.findIntersection(playerBounds) || ballBounds.findIntersection(compBounds)) {
			ballVelocity.x = -ballVelocity.x;

			// Add spin based on where ball hits paddle
			if (ballBounds.findIntersection(playerBounds)) {
				float hitPos = (ballCenterY - playerCenterY) / (playerBounds.size.y / 2.f);
				ballVelocity.y += hitPos * 200.f;
			}
			else if (ballBounds.findIntersection(compBounds)) {
				float hitPos = (ballCenterY - computerCenterY) / (compBounds.size.y / 2.f);
				ballVelocity.y += hitPos * 200.f;
			}
		}

		// Computer Movement - Simple AI
		float paddleSpeed = 250.f;
		if (ballVelocity.x > 0) {  // Ball moving toward computer (right side)
			if (ballCenterY < computerCenterY - 10.f) {
				computerPaddle.move({ 0.f, -paddleSpeed * dt });
			}
			else if (ballCenterY > computerCenterY + 10.f) {
				computerPaddle.move({ 0.f, paddleSpeed * dt });
			}
		}

		// Keep computer paddle in bounds
		if (compBounds.position.y < 0.f) {
			computerPaddle.setPosition({ compBounds.position.x, 0.f });
		}
		if (compBounds.position.y + compBounds.size.y > screenY) {
			computerPaddle.setPosition({ compBounds.position.x, screenY - compBounds.size.y });
		}


		if (ball.getPosition().x > screenX + 50) {
			playerScore++;
			playerScoreText.setString(std::to_string(playerScore));
			ball.setPosition({ static_cast<float>(screenX / 2), static_cast<float>(screenY / 2) });
			ballVelocity.x = -ballVelocity.x;
			ballVelocity.y = sf::degrees(degree_range(gen)).asRadians() - ballVelocity.y;
		}
		else if (ball.getPosition().x < -50) {
			computerScore++;
			computerScoreText.setString(std::to_string(computerScore));
			ball.setPosition({ static_cast<float>(screenX / 2), static_cast<float>(screenY / 2) });
			ballVelocity.x = -ballVelocity.x;
			ballVelocity.y = sf::degrees(degree_range(gen)).asRadians() - ballVelocity.y;
		}

		window.clear();
		window.draw(playerPaddle);
		window.draw(playerScoreText);
		window.draw(computerScoreText);
		window.draw(computerPaddle);
		window.draw(ball);
		window.display();
	}
}
