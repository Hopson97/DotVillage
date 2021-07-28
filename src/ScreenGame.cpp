#include "ScreenGame.h"
#include "ScreenMainMenu.h"
#include <cmath>
#include <imgui_sfml/imgui.h>
#include <iostream>

ScreenGame::ScreenGame(ScreenManager* stack)
    : Screen(stack)
    , m_recruitWalkAnimation(40, 40)
{
    m_sprite.setSize({64, 64});
    m_recruitTexture.loadFromFile("Data/Recruit.png");
    // m_recruitWalkAnimation.addFrame(0, 0, sf::milliseconds(500));
    m_recruitWalkAnimation.addFrame(0, 1, sf::milliseconds(500));
    m_recruitWalkAnimation.addFrame(0, 2, sf::milliseconds(500));
    // m_recruitWalkAnimation.addFrame(0, 1, sf::milliseconds(500));
}

void ScreenGame::onEvent(const sf::Event& e)
{
    if (e.type == sf::Event::KeyReleased) {
        if (e.key.code == sf::Keyboard::Escape) {
            m_isPasued = !m_isPasued;
        }
    }
}

void ScreenGame::onGUI()
{
    if (m_isPasued) {
        if (imguiBeginMenu("P A U S E   M E N U")) {
            if (ImGui::Button("Resume Game")) {
                m_isPasued = false;
            }
            if (ImGui::Button("Exit Game")) {
                m_pScreens->popScreen();
            }
        }
        ImGui::End();
    }
}

void ScreenGame::onUpdate(const sf::Time& dt)
{
    static float y = 500;
    static float x = 500;
    y += std::sin(m_clock.getElapsedTime().asSeconds() * dt.asSeconds() * 100);
    x += std::cos(m_clock.getElapsedTime().asSeconds() * dt.asSeconds() * 100);
    m_sprite.setPosition({x, y});
}
void ScreenGame::onRender(sf::RenderWindow* window)
{
    m_sprite.setTexture(&m_recruitTexture);
    m_sprite.setTextureRect(m_recruitWalkAnimation.getFrame());
    window->draw(m_sprite);
}
