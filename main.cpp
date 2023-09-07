#include <SFML/Graphics.hpp>
#include "Player.h"
#include "GameWorld.h"
#include "AIStrategy.h"  // Include the AI strategy header
#include "AStar.h"  // Include the AStar header
#include <iostream>
int main() {
    sf::RenderWindow window(sf::VideoMode(2560, 1600), "SFML Window");
    window.setFramerateLimit(120);  // Limit the frame rate to 60 FPS

    GameWorld world("./untitled.tmx");
    Player player(world, window);

    sf::View cameraView(sf::FloatRect(0, 0, 2560, 1600));
    cameraView.setCenter(player.getPosition());

    sf::Clock clock;

    // Placeholder for the goal point (you should set this dynamically based on your game logic)
    sf::Vector2i goal(50, 2);

    std::list<sf::Vector2i> path;

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

        // Update A* path based on the current player position
        AStar astar(world);

        // Convert player position from pixels to grid coordinates by dividing by 20
        sf::Vector2i start(
            static_cast<int>(player.getPosition().x / 32),
            static_cast<int>(player.getPosition().y / 32)
        );

        path = astar.findPath(start, goal);

        window.clear(sf::Color::Black);
        window.setView(cameraView);
        world.draw(window, cameraView);

        if (path.empty()) {
            std::cout << "No path found!" << std::endl;
        }

         // Draw the path
        for (const auto& point : path) {
            sf::RectangleShape rect(sf::Vector2f(5, 5));  // 5x5 size, you can change this
            rect.setFillColor(sf::Color::Red);  // The rectangle will be red
            int yOffset = static_cast<int>(player.getPosition().y) - (start.y * 32);
            rect.setPosition(static_cast<float>(point.x * 32), static_cast<float>(point.y * 32 + yOffset));
            // Assuming each grid cell is 32x32, adjust as needed
            window.draw(rect);
        }


        player.draw(window);
        window.display();
    }

    return 0;
}
