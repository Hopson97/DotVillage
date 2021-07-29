#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>

struct Building {
    std::string name;
    std::string description;

    int costCoins = 0;
    int costWood = 0;
    int costFood = 0;
    int costStone = 0;
    int costMetal = 0;
    int costJobs = 0;

    int rateCoins = 0;
    int rateWood = 0;
    int rateFood = 0;
    int rateStone = 0;
    int rateMetal = 0;

    int onBuildPop = 0;

    float width = 0;
    float height = 0;

    sf::Texture texture;
};

struct PlacedBuilding {
    int id;
    sf::FloatRect bounds;
};

std::vector<Building> loadBuildings();
