#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

enum class DotState {
    Home,
    Work,
    Walking,
};

struct Dot {
    int house = -1;
    int occupaction = -1;
    int targetId = -1;
    

    sf::Vector2f location;
    sf::Vector2f target;

    sf::Clock timer;
    float nextAction;
    float speed;
    float size;

    DotState state = DotState::Home;
};

inline std::vector<Dot> makeDots(int number, int home)
{
    std::vector<Dot> dots;
    for (int i = 0; i < number; i++) {
        Dot dot;
        dot.house = home;
        dot.nextAction = rand() % 10;
        dot.speed = 30 + rand() % 25;
        dot.size = rand() % 5;
        dots.push_back(dot);
    }
    return dots;
}