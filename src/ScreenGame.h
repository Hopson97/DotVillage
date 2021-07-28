#pragma once

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

    bool m_isPasued = false;

    int m_population = 0;
    int m_coins = 250;
    int m_food = 250;
    int m_wood = 250;
    int m_metal = 250;
};
