#include "Building.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using directoryItr = std::filesystem::recursive_directory_iterator;

namespace {
    Building loadBuilding(const std::filesystem::path& path)
    {
        std::ifstream infile(path);
        Building building;
        std::string key;

        building.texture.loadFromFile("Data/Textures/" + path.filename().string() +
                                      ".png");

        while (infile >> key) {
            if (key == "cost") {
                infile >> key >> building.costCoins;
                infile >> key >> building.costWood;
                infile >> key >> building.costFood;
                infile >> key >> building.costStone;
                infile >> key >> building.costMetal;
                infile >> key >> building.costJobs;
            }
            else if (key == "rates") {
                infile >> key >> building.rateCoins;
                infile >> key >> building.rateWood;
                infile >> key >> building.rateFood;
                infile >> key >> building.rateStone;
                infile >> key >> building.rateMetal;
            }
            else if (key == "onbuild") {
                infile >> key >> building.onBuildPop;
            }
            else if (key == "meta") {
                infile >> key >> building.width >> building.height;
                infile >> key >> building.name;
                infile >> key;
                std::getline(infile, building.description);
            }
        }
        std::cout << building.costCoins << " " << building.costWood << " "
                  << building.costFood << " " << building.costStone << " "
                  << building.costMetal << " " << building.costJobs << "\n"

                  << building.rateCoins << " " << building.rateWood << " "
                  << building.rateFood << " " << building.rateStone << " "
                  << building.rateMetal << "\n"

                  << building.onBuildPop << "\n"

                  << building.width << " " << building.height << " " << building.name
                  << building.description << "\n\n";
        return building;
    }
} // namespace

std::vector<Building> loadBuildings()
{
    std::vector<Building> buildings;
    for (const auto& dirEntry : directoryItr("Data/Buildings")) {
        buildings.push_back(loadBuilding(dirEntry));
    }
    return buildings;
}