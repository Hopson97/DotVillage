#pragma once

#include "Animation.h"
#include "Screen.h"

class ScreenGame final : public Screen {
  public:
    ScreenGame(ScreenManager* screens);

    void onEvent(const sf::Event& e) override;
    void onGUI() override;
    void onUpdate(const sf::Time& dt) override;
    void onRender(sf::RenderWindow* window) override;

  private:
    sf::RectangleShape m_sprite;
    sf::Clock m_clock;
    sf::Texture m_recruitTexture;

    Animation m_recruitWalkAnimation;

    bool m_isPasued = false;
};
