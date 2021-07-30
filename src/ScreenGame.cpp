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

    m_dot.setRadius(5);
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

            int buildingPtr = 0;
            for (const Building& building : m_buildingBlueprints) {
                if (ImGui::GetCursorScreenPos().y + building.height > WIN_HEIGHT) {
                    ImGui::NextColumn();
                }
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

        auto drawCosts = [&](const Building* building) {
            if (ImGui::Begin(building->name.c_str(), nullptr,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::ImageButton(building->texture,
                                   sf::Vector2f{building->width, building->height});
                ImGui::Text("%s", building->description.c_str());
                ImGui::Separator();

                // clang-format off
                ImGui::Text("Cost");
                drawResourceLine(m_texResCoin, building->costCoins,"Coins");
                drawResourceLine(m_texResFood, building->costFood,"Food");
                drawResourceLine(m_texResWood,building->costWood, "Wood");
                drawResourceLine(m_texResStone, building->costStone, "Stone");
                drawResourceLine(m_texResMetal, building->costMetal, "Metal");
                drawResourceLine(m_texResUnemployed, building->costJobs, "Unemployed Needed");

                ImGui::Separator();
                ImGui::Text("Daily Rate");
                drawResourceLine(m_texResCoin, building->rateCoins, "Tax");
                drawResourceLine(m_texResFood, building->rateFood, "Daily Food Rate");
                drawResourceLine(m_texResWood, building->rateWood, "Daily Wood Rate");
                drawResourceLine(m_texResStone, building->rateStone, "Daily Stone Rate");
                drawResourceLine(m_texResMetal, building->rateMetal, "Daily Metal Rate");
                // clang-format on
                ImGui::End();
            }
        };

        if (hoveredBuilding) {
            drawCosts(hoveredBuilding);
        }
        else if (isBuildingSelected()) {
            drawCosts(&m_buildingBlueprints[m_selectedBuilding]);
        }
    }
}

void ScreenGame::onUpdate(const sf::Time& dt)
{
    if (m_dailyTimer.getElapsedTime() > sf::seconds(5)) {
        m_coins += m_dailyCoins;
        m_wood += m_dailyWood;
        m_food += m_dailyFood;
        m_stone += m_dailyStone;
        m_metal += m_dailyMetal;

        m_dailyTimer.restart();
    }

    for (auto& dot : m_dots) {
        const PlacedBuilding* home = &m_buildings[dot.house];
        const PlacedBuilding* work = &m_buildings[dot.occupaction];
        const Building* homebp = &m_buildingBlueprints[home->id];
        const Building* workbp = &m_buildingBlueprints[work->id];

        switch (dot.state) {
            case DotState::Home:
                if (dot.timer.getElapsedTime().asSeconds() > dot.nextAction && dot.occupaction > -1) {
                    dot.timer.restart();
                    dot.state = DotState::Walking;

                    dot.location.x = home->bounds.left + homebp->width / 2;
                    dot.location.y = home->bounds.top + homebp->height / 2;

                    dot.target.x = work->bounds.left + workbp->width / 2;
                    dot.target.y = work->bounds.top + workbp->height / 2;
                    dot.targetId = dot.occupaction;
                }
                break;

            case DotState::Work:
                if (dot.timer.getElapsedTime().asSeconds() > dot.nextAction) {
                    dot.timer.restart();
                    dot.state = DotState::Walking;

                    dot.target.x = home->bounds.left + homebp->width / 2;
                    dot.target.y = home->bounds.top + homebp->height / 2;

                    dot.location.x = work->bounds.left + workbp->width / 2;
                    dot.location.y = work->bounds.top + workbp->height / 2;
                    dot.targetId = dot.house;
                }
                break;

            case DotState::Walking: {
                sf::Vector2f delta = dot.location - dot.target;

                float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
                sf::Vector2f speed = {delta.x / length, delta.y / length};
                dot.location -= speed * dt.asSeconds() * dot.speed;

                if (dot.targetId == dot.house && length < homebp->width / 4) {
                    dot.state = DotState::Home;
                }
                else if (dot.targetId == dot.occupaction && length < workbp->width / 4) {
                    dot.state = DotState::Work;
                }
            }

            break;
        }
    }
}

void ScreenGame::onRender(sf::RenderWindow* window)
{
    window->draw(m_backgroundRect);

    for (const Dot& dot : m_dots) {
        if (dot.state == DotState::Walking) {
            m_dot.setPosition(dot.location);
            m_dot.setRadius(dot.size);
            window->draw(m_dot);
        }
    }

    for (const PlacedBuilding& placedBuilding : m_buildings) {
        const Building* building = &m_buildingBlueprints[placedBuilding.id];
        m_sprite.setSize({building->width, building->height});
        m_sprite.setTexture(&building->texture, true);
        m_sprite.setPosition(placedBuilding.bounds.left, placedBuilding.bounds.top);
        window->draw(m_sprite);
    }

    if (isBuildingSelected()) {
        const Building* building = &m_buildingBlueprints[m_selectedBuilding];
        m_sprite.setSize({building->width, building->height});
        m_sprite.setTexture(&building->texture, true);
        m_sprite.setPosition(m_mouseX, m_mouseY);
        if (!canAfford(m_selectedBuilding)) {
            m_sprite.setFillColor(sf::Color::Red);
        }
        window->draw(m_sprite);
        m_sprite.setFillColor(sf::Color::White);
    }
}

bool ScreenGame::canAfford(int buildingId)
{
    const Building* blueprint = &m_buildingBlueprints[buildingId];
    return m_coins >= blueprint->costCoins && m_unemployed >= blueprint->costJobs &&
           m_food >= blueprint->costFood && m_stone >= blueprint->costStone &&
           m_metal >= blueprint->costMetal && m_wood >= blueprint->costWood;
}

bool ScreenGame::isBuildingSelected() const { return m_selectedBuilding > -1; }

void ScreenGame::tryPlaceBuilding(float x, float y)
{
    if (canAfford(m_selectedBuilding) && y < WIN_HEIGHT - 200) {
        const Building* blueprint = &m_buildingBlueprints[m_selectedBuilding];

        PlacedBuilding building;
        building.id = m_selectedBuilding;
        building.bounds.width = blueprint->width;
        building.bounds.height = blueprint->height;
        building.bounds.left = x;
        building.bounds.top = y;

        m_coins -= blueprint->costCoins;
        m_food -= blueprint->costFood;
        m_wood -= blueprint->costWood;
        m_stone -= blueprint->costStone;
        m_metal -= blueprint->costMetal;
        m_unemployed -= blueprint->costJobs;

        m_population += blueprint->onBuildPop;
        m_unemployed += blueprint->onBuildPop;

        m_dailyCoins += blueprint->rateCoins;
        m_dailyFood += blueprint->rateFood;
        m_dailyWood += blueprint->rateWood;
        m_dailyStone += blueprint->rateStone;
        m_dailyMetal += blueprint->rateMetal;

        m_buildings.push_back(building);
        int id = m_buildings.size() - 1;

        if (blueprint->type == BuildingType::Dwelling) {
            auto newDots = makeDots(blueprint->onBuildPop, id);
            m_dots.insert(m_dots.end(), newDots.begin(), newDots.end());
        }
        else {
            int jobs = blueprint->costJobs;
            for (Dot& dot : m_dots) {
                if (dot.occupaction == -1 && jobs > 0) {
                    dot.occupaction = id;
                    jobs--;
                }
            }
        }
    }
}