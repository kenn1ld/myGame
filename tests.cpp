#include <gtest/gtest.h>
#include "GameWorld.h"
#include <SFML/Graphics/RenderTexture.hpp>

class GameWorldTest : public ::testing::Test {
public:
    std::unique_ptr<GameWorld> gameWorld;

    void SetUp() override {
        gameWorld = std::make_unique<GameWorld>("path_to_known_tmx_file.tmx");  // Replace with the path to a known TMX file
    }
};

TEST_F(GameWorldTest, TileDrawingWithCulling) {
    sf::RenderTexture offScreenTarget;
    offScreenTarget.create(800, 600);  // Assuming a typical screen size, adjust as needed

    sf::View view(sf::FloatRect(0, 0, 800, 600));  // A typical view, adjust as needed
    offScreenTarget.setView(view);

    gameWorld->draw(offScreenTarget, view);  // This line will still produce an error

    // Additional code might be needed to count drawn tiles based on pixel analysis
    // For now, this test can visually confirm if culling works when drawn to the main window
}


TEST_F(GameWorldTest, CollisionDetection) {
    sf::FloatRect collidingRect(100, 100, 32, 32);  // Sample rectangle that's expected to collide
    EXPECT_TRUE(gameWorld->checkCollision(collidingRect));

    sf::FloatRect nonCollidingRect(0, 0, 32, 32);  // Sample rectangle that's expected not to collide
    EXPECT_FALSE(gameWorld->checkCollision(nonCollidingRect));
}

TEST_F(GameWorldTest, GroundedCheck) {
    sf::FloatRect groundedRect(100, 500, 32, 32);  // Sample rectangle that's expected to be grounded
    EXPECT_TRUE(gameWorld->isGrounded(groundedRect));

    sf::FloatRect nonGroundedRect(100, 100, 32, 32);  // Sample rectangle that's expected not to be grounded
    EXPECT_FALSE(gameWorld->isGrounded(nonGroundedRect));
}

