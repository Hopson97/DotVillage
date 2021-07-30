#include "Building.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using directoryItr = std::filesystem::recursive_directory_iterator;

namespace {
    Building loadBuilding(const std::filesystem::path& path)
    {
        std::ifstream inFile(path);
        Building building;
        std::string key;

        building.texture.loadFromFile("Data/Textures/" + path.filename().string() +
                                      ".png");

        // clang-format off
        while (inFile >> key) {
            if (key == "cost") {
                while(key != "end") {
                    std::cout << key << "\n";
                    inFile >> key;
                    if (key == "coins") inFile >> building.costCoins;
                    else if (key == "wood")  inFile >> building.costWood;
                    else if (key == "food")  inFile >> building.costFood;
                    else if (key == "stone") inFile >> building.costStone;
                    else if (key == "metal") inFile >> building.costMetal;
                    else if (key == "jobs") inFile >> building.costJobs;
                }
            }
            else if (key == "rates") {
                while(key != "end") {
                    inFile >> key;
                    if (key == "coins") inFile >> building.rateCoins;
                    else if (key == "wood")  inFile >> building.rateWood;
                    else if (key == "food")  inFile >> building.rateFood;
                    else if (key == "stone") inFile >> building.rateStone;
                    else if (key == "metal") inFile >> building.rateMetal;
                }
            }
            else if (key == "onbuild") {
                while(key != "end") {
                    inFile >> key;
                    if (key == "population") inFile >> building.onBuildPop;
                }
            }
            else if (key == "meta") {
                while(key != "end") {
                    inFile >> key;
                    if (key == "size") inFile >> building.width >> building.height;
                    if (key == "name") inFile >> building.name;
                    if (key == "desc") std::getline(inFile, building.description);

                    if (key == "type") {
                        inFile >> key;
                        if (key == "dwelling") building.type = BuildingType::Dwelling;
                        if (key == "food") building.type = BuildingType::Food;
                        if (key == "wood") building.type = BuildingType::Wood;
                        if (key == "stone") building.type = BuildingType::Stone;
                        if (key == "metal") building.type = BuildingType::Metal;
                    }
                }
            }
            // clang-format on
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
        auto building = loadBuilding(dirEntry);
        bool inserted = false;
        for (auto itr = buildings.begin(); itr != buildings.end();) {
            auto b = &*itr;
            if(b->type == building.type) {
                buildings.insert(itr, building);
                inserted = true;
                break;
            }
            else {
                itr++;
            }
        }
        if (!inserted) {
         buildings.push_back(loadBuilding(dirEntry));

        }
    }
    return buildings;
}