#include <SFML/Graphics.hpp>
#include "Player.h"
#include "GameWorld.h"
#include "Skeleton.h"
#include "AIStrategy.h"  // Include the AI strategy header

int main() {
	sf::RenderWindow window(sf::VideoMode(2560, 1600), "SFML Window");
	window.setFramerateLimit(120);  // Limit the frame rate to 60 FPS

	GameWorld world("./untitled.tmx");
	Player player(world, window);

	sf::View cameraView(sf::FloatRect(0, 0, 2560, 1600));
	cameraView.setCenter(player.getPosition());

	sf::Clock clock;
	Skeleton skeleton;

	// Set the skeleton's behavior to "Chase" for demonstration.
	// This can be dynamically changed during gameplay based on certain conditions.
	skeleton.setStrategy(std::make_unique<ChaseStrategy>());

	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			// Handle other events here if needed.
		}

		player.handleInput(deltaTime);
		player.updateProjectiles(deltaTime);  // Pass the GameWorld reference here

		// Zoom in by modifying the view size
		float zoomFactor = 0.4f;  // Adjust the zoom factor as needed
		cameraView.setSize(static_cast<float>(window.getSize().x) * zoomFactor,
			static_cast<float>(window.getSize().y) * zoomFactor);

		world.update(deltaTime, cameraView, player);

		skeleton.handleChase(deltaTime, player.getPosition(), world);
		// Update the Skeleton's AI behavior
		skeleton.update(deltaTime, world);    // Update the Skeleton's position and other attributes

		window.clear(sf::Color::Black);
		window.setView(cameraView);
		world.draw(window, cameraView);
		player.draw(window);
		skeleton.draw(window);  // Draw the Skeleton
		window.display();
	}

	return 0;
}