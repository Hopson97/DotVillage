#include "ScreenGame.h"
#include "GUI.h"
#include "ScreenMainMenu.h"
#include <cmath>
#include <imgui_sfml/imgui-SFML.h>
#include <imgui_sfml/imgui.h>
#include <iostream>

ScreenGame::ScreenGame(ScreenManager* stack)
    : Screen(stack)
{
    m_backgroundRect.setSize({WIN_WIDTH, WIN_HEIGHT});
    m_backgroundTexture.loadFromFile("Data/Textures/Area1.png");
    m_backgroundImage.loadFromFile("Data/Textures/Area1.png");

    m_backgroundRect.setTexture(&m_backgroundTexture);

    m_buildingBlueprints = loadBuildings();
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
        const Building* hoveredBuilding = nullptr;

        if (imguiGameMenuBegin("M E N U")) {
            ImGui::Columns(15, "game menu", false);
            ImGui::Text("     Coins: %d", m_coins);
            ImGui::Text("Population: %d", m_population);
            ImGui::Text("Unemployed: %d", m_unemployed);

            ImGui::NextColumn();
            ImGui::Text("      Food: %d", m_food);
            ImGui::Text("      Wood: %d", m_wood);
            ImGui::Text("     Stone: %d", m_stone);
            ImGui::Text("     Metal: %d", m_metal);
            ImGui::NextColumn();
            ImGui::NextColumn();

            for (const Building& building : m_buildingBlueprints) {
                ImGui::ImageButton(building.texture,
                                   sf::Vector2f{building.width, building.height});
                if (ImGui::IsItemHovered()) {
                    hoveredBuilding = &building;
                }
            }
        }
        ImGui::End();

        if (hoveredBuilding) {
            if (ImGui::Begin(hoveredBuilding->name.c_str())) {
                ImGui::ImageButton(
                    hoveredBuilding->texture,
                    sf::Vector2f{hoveredBuilding->width, hoveredBuilding->height});
                ImGui::Text(hoveredBuilding->description.c_str());
                ImGui::Separator();
                ImGui::Text("Cost");
                ImGui::Text("Coins: %d", hoveredBuilding->costCoins);
                ImGui::Text("Food: %d", hoveredBuilding->costFood);
                ImGui::Text("Wood: %d", hoveredBuilding->costWood);
                ImGui::Text("Stone: %d", hoveredBuilding->costStone);
                ImGui::Text("Metal: %d", hoveredBuilding->costMetal);

                ImGui::Separator();
                ImGui::Text("Daily Rate");
                ImGui::Text("Coins: %d", hoveredBuilding->rateCoins);
                ImGui::Text("Food: %d", hoveredBuilding->rateFood);
                ImGui::Text("Wood: %d", hoveredBuilding->rateMetal);
                ImGui::Text("Stone: %d", hoveredBuilding->rateStone);
                ImGui::Text("Metal: %d", hoveredBuilding->rateMetal);
            }
            ImGui::End();
        }
    }
}

void ScreenGame::onUpdate(const sf::Time& dt) {}
void ScreenGame::onRender(sf::RenderWindow* window) { window->draw(m_backgroundRect); }
