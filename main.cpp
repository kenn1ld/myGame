#include <SFML/Graphics.hpp>
#include "Player.h"
#include "GameWorld.h"

int main() {
	sf::RenderWindow window(sf::VideoMode(2560, 1600), "SFML Window");
	window.setFramerateLimit(60);  // Limit the frame rate to 60 FPS

	GameWorld world("./untitled.tmx");
	Player player(world, window);

	sf::View cameraView(sf::FloatRect(0, 0, 2560, 1600));
	cameraView.setCenter(player.getPosition());

	sf::Clock clock;

	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
				// Handle other events here if needed.
			default:
				// Do nothing or handle generically.
				break;
			}
		}

		player.handleInput(deltaTime);
		player.updateProjectiles(deltaTime);

		// Zoom in by modifying the view size
		float zoomFactor = 0.3f; // Adjust the zoom factor as needed
		cameraView.setSize(static_cast<float>(window.getSize().x) * zoomFactor,
			static_cast<float>(window.getSize().y) * zoomFactor);

		world.update(deltaTime, cameraView, player);

		window.clear(sf::Color::Black);
		window.setView(cameraView);
		world.draw(window, cameraView);
		player.draw(window);
		window.display();
	}

	return 0;
}