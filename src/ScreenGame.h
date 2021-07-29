#pragma once

#include "Game/Building.h"
#include "Screen.h"

class ScreenGame final : public Screen {
  public:
    ScreenGame(ScreenManager* screens);

    void onEvent(const sf::Event& e) override;
    void onInput(const sf::RenderWindow& window) override;
    void onGUI() override;
    void onUpdate(const sf::Time& dt) override;
    void onRender(sf::RenderWindow* window) override;

  private:
    bool isBuildingSelected() const;
    bool canAfford(int buildingId);
    void tryPlaceBuilding(float x, float y);

    sf::RectangleShape m_backgroundRect;
    sf::Texture m_backgroundTexture;
    sf::Image m_backgroundImage;

    sf::Clock m_dailyTimer;

    sf::RectangleShape m_sprite;

    std::vector<Building> m_buildingBlueprints;
    std::vector<PlacedBuilding> m_buildings;

    sf::Texture m_texResCoin;
    sf::Texture m_texResMetal;
    sf::Texture m_texResPopulation;
    sf::Texture m_texResStone;
    sf::Texture m_texResUnemployed;
    sf::Texture m_texResWood;
    sf::Texture m_texResFood;

    float m_mouseX = 0;
    float m_mouseY = 0;
    int m_selectedBuilding = -1;

    bool m_isPasued = false;

    int m_coins = 100;
    int m_population = 0;
    int m_unemployed = 0;

    int m_food = 100;
    int m_wood = 100;
    int m_stone = 100;
    int m_metal = 100;

    int m_dailyCoins = 0;
    int m_dailyWood = 0;
    int m_dailyFood = 0;
    int m_dailyStone = 0;
    int m_dailyMetal = 0;
};
