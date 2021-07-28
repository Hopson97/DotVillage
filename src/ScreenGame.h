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
    sf::RectangleShape m_backgroundRect;
    sf::Texture m_backgroundTexture;
    sf::Image m_backgroundImage;

    sf::RectangleShape m_sprite;

    std::vector<Building> m_buildingBlueprints;

    bool m_isPasued = false;

    int m_coins = 100;
    int m_population = 0;
    int m_unemployed = 0;

    int m_food = 100;
    int m_wood = 100;
    int m_stone = 100;
    int m_metal = 100;
};
