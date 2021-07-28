#include "ScreenGame.h"
#include "ScreenMainMenu.h"
#include <cmath>
#include <imgui_sfml/imgui.h>
#include <iostream>
#include "GUI.h"

ScreenGame::ScreenGame(ScreenManager* stack)
    : Screen(stack)
{
    m_backgroundRect.setSize({WIN_WIDTH, WIN_HEIGHT});
    m_backgroundTexture.loadFromFile("Data/area1.png");
    m_backgroundImage.loadFromFile("Data/area1.png");

    m_backgroundRect.setTexture(&m_backgroundTexture);
}

void ScreenGame::onEvent(const sf::Event& e)
{
    if (e.type == sf::Event::KeyReleased) {
        if (e.key.code == sf::Keyboard::Escape) {
            m_isPasued = !m_isPasued;
        }
    }
}

void ScreenGame::onInput(const sf::RenderWindow& window) {}

void ScreenGame::onGUI()
{
    if (m_isPasued) {
        if (imguiBeginMenu("P A U S E   M E N U")) {
            if (imguiButtonCustom("Resume Game")) {
                m_isPasued = false;
            }
            if (imguiButtonCustom("Exit Game")) {
                m_pScreens->popScreen();
            }
        }
        ImGui::End();
    }
    else {
        if (imguiGameMenuBegin("M E N U")) {
            ImGui::Text("Resources");
            ImGui::Text("Population: %d", m_population);
            ImGui::Text("     Coins: %d", m_coins);
            ImGui::Text("      Food: %d", m_food);
            ImGui::Text("      Wood: %d", m_wood);
            ImGui::Text("     Metal: %d", m_metal);
            ImGui::Separator();
        }
        ImGui::End();
    }
}

void ScreenGame::onUpdate(const sf::Time& dt) {}
void ScreenGame::onRender(sf::RenderWindow* window) { window->draw(m_backgroundRect); }
