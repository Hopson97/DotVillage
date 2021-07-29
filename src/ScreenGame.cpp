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

    m_texResCoin.loadFromFile("Data/Textures/ResCoin.png");
    m_texResMetal.loadFromFile("Data/Textures/ResMetal.png");
    m_texResPopulation.loadFromFile("Data/Textures/ResPopulation.png");
    m_texResStone.loadFromFile("Data/Textures/ResStone.png");
    m_texResUnemployed.loadFromFile("Data/Textures/ResUnemployed.png");
    m_texResWood.loadFromFile("Data/Textures/ResWood.png");
    m_texResFood.loadFromFile("Data/Textures/ResFood.png");
}

void ScreenGame::onEvent(const sf::Event& e)
{
    if (e.type == sf::Event::KeyReleased) {
        if (e.key.code == sf::Keyboard::Escape) {
            m_isPasued = !m_isPasued;
        }
    }
    else if (e.type == sf::Event::MouseMoved) {
        m_mouseX = e.mouseMove.x;
        m_mouseY = e.mouseMove.y;
    }
    else if (e.type == sf::Event::MouseButtonPressed) {
        if (e.mouseButton.button == sf::Mouse::Left) {
            if (isBuildingSelected()) {
                tryPlaceBuilding(e.mouseButton.x, e.mouseButton.y);
            }
        }
        else {
            m_selectedBuilding = -1;
        }
    }
}

void ScreenGame::onInput(const sf::RenderWindow& window) {}

void drawResourceLine(const sf::Texture& texture, int value, const char* tooltip)
{
    ImGui::Image(texture, {16, 16});
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("%s", tooltip);
        ImGui::EndTooltip();
    }
    ImGui::SameLine();
    ImGui::Text("%d", value);
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::Text("%s", tooltip);
        ImGui::EndTooltip();
    }
}

void ScreenGame::onGUI()
{
    ImGui::ShowDemoWindow();
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
            ImGui::Columns(16, "game menu", false);

            ImGui::Text("Stats");
            drawResourceLine(m_texResCoin, m_coins, "Coins");
            drawResourceLine(m_texResPopulation, m_population, "Population");
            drawResourceLine(m_texResUnemployed, m_unemployed, "Unemployed Population");
            ImGui::NextColumn();

            ImGui::Text("Resources");
            drawResourceLine(m_texResFood, m_food, "Food");
            drawResourceLine(m_texResWood, m_wood, "Wood");
            drawResourceLine(m_texResStone, m_stone, "Stone");
            drawResourceLine(m_texResMetal, m_metal, "Metal");
            ImGui::NextColumn();

            ImGui::Text("Daily");
            drawResourceLine(m_texResCoin, m_dailyCoins, "Tax");
            drawResourceLine(m_texResFood, m_dailyFood, "Daily Food Rate");
            drawResourceLine(m_texResWood, m_dailyWood, "Daily Wood Rate");
            drawResourceLine(m_texResStone, m_stone, "Daily Stone Rate");
            drawResourceLine(m_texResMetal, m_metal, "Daily Metal Rate");
            ImGui::NextColumn();

            ImGui::Text("Buildings");
            int buildingPtr = 0;
            for (const Building& building : m_buildingBlueprints) {
                if (ImGui::ImageButton(building.texture,
                                       sf::Vector2f{building.width, building.height})) {
                    m_selectedBuilding = buildingPtr;
                }
                if (ImGui::IsItemHovered()) {
                    hoveredBuilding = &building;
                }
                buildingPtr++;
            }
        }
        ImGui::End();

        if (hoveredBuilding) {
            if (ImGui::Begin(hoveredBuilding->name.c_str(), nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::ImageButton(
                    hoveredBuilding->texture,
                    sf::Vector2f{hoveredBuilding->width, hoveredBuilding->height});
                ImGui::Text(hoveredBuilding->description.c_str());
                ImGui::Separator();

                // clang-format off
                ImGui::Text("Cost");
                drawResourceLine(m_texResCoin, hoveredBuilding->costCoins,"Coins");
                drawResourceLine(m_texResFood, hoveredBuilding->costFood,"Food");
                drawResourceLine(m_texResWood,hoveredBuilding->costWood, "Wood");
                drawResourceLine(m_texResStone, hoveredBuilding->costStone, "Stone");
                drawResourceLine(m_texResMetal, hoveredBuilding->costMetal, "Metal");
                drawResourceLine(m_texResUnemployed, hoveredBuilding->costJobs, "Unemployed Needed");

                ImGui::Separator();
                ImGui::Text("Daily Rate");
                drawResourceLine(m_texResCoin, hoveredBuilding->rateCoins, "Tax");
                drawResourceLine(m_texResFood, hoveredBuilding->rateFood, "Daily Food Rate");
                drawResourceLine(m_texResWood, hoveredBuilding->rateWood, "Daily Wood Rate");
                drawResourceLine(m_texResStone, hoveredBuilding->rateStone, "Daily Stone Rate");
                drawResourceLine(m_texResMetal, hoveredBuilding->rateMetal, "Daily Metal Rate");
                // clang-format on
                ImGui::End();
            }
        }
    }
}

void ScreenGame::onUpdate(const sf::Time& dt) {}

void ScreenGame::onRender(sf::RenderWindow* window)
{
    window->draw(m_backgroundRect);

    if (isBuildingSelected()) {
        const Building* building = &m_buildingBlueprints[m_selectedBuilding];
        m_sprite.setSize({building->width, building->height});
        m_sprite.setTexture(&building->texture);
        m_sprite.setPosition(m_mouseX, m_mouseY);
        window->draw(m_sprite);
    }

    for (const PlacedBuilding& placedBuilding : m_buildings) {
        const Building* building = &m_buildingBlueprints[placedBuilding.id];
        m_sprite.setSize({building->width, building->height});
        m_sprite.setTexture(&building->texture);
        m_sprite.setPosition(placedBuilding.bounds.left, placedBuilding.bounds.top);
        window->draw(m_sprite);
    }
}

bool ScreenGame::isBuildingSelected() const { return m_selectedBuilding > -1; }

void ScreenGame::tryPlaceBuilding(float x, float y)
{
    const Building* blueprint = &m_buildingBlueprints[m_selectedBuilding];

    PlacedBuilding building;
    building.id = m_selectedBuilding;
    building.bounds.width = blueprint->width;
    building.bounds.height = blueprint->height;
    building.bounds.left = x;
    building.bounds.top = y;

    std::cout << "blueprint->costCoins " << blueprint->costCoins << "\n"
              << "blueprint->costFood  " << blueprint->costFood << "\n"
              << "blueprint->costWood  " << blueprint->costWood << "\n"
              << "blueprint->costStone " << blueprint->costStone << "\n"
              << "blueprint->costMetal " << blueprint->costMetal << "\n"
              << "blueprint->onBuildPop" << blueprint->onBuildPop << "\n\n";

    std::cout << "m_coins" << m_coins << "\n"
              << "m_food" << m_food << "\n"
              << "m_wood" << m_wood << "\n"
              << "m_stone" << m_stone << "\n"
              << "m_metal" << m_metal << "\n"
              << "m_population" << m_population << "\n\n";

    m_coins -= blueprint->costCoins;
    m_food -= blueprint->costFood;
    m_wood -= blueprint->costWood;
    m_stone -= blueprint->costStone;
    m_metal -= blueprint->costMetal;

    m_population += blueprint->onBuildPop;
    m_unemployed += blueprint->onBuildPop;

    m_unemployed -= blueprint->costJobs;

    m_dailyCoins += blueprint->rateCoins;
    m_dailyFood += blueprint->rateFood;
    m_dailyWood += blueprint->rateWood;
    m_dailyStone += blueprint->rateStone;
    m_dailyMetal += blueprint->rateMetal;

    std::cout << "m_coins" << m_coins << "\n"
              << "m_food" << m_food << "\n"
              << "m_wood" << m_wood << "\n"
              << "m_stone" << m_stone << "\n"
              << "m_metal" << m_metal << "\n"
              << "m_population" << m_population << "\n\n";

    m_buildings.push_back(building);
}