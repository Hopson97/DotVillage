#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include <vector>

class GUI;

class Screen;

constexpr int WIN_WIDTH = 1600;
constexpr int WIN_HEIGHT = 900;

class ScreenManager final {
    struct Action {
        enum class Kind {
            Push,
            Pop,
            Change,
        };
        Kind kind;
        std::unique_ptr<Screen> screen;
    };

  public:
    void pushScreen(std::unique_ptr<Screen> screen);
    void popScreen();
    void changeScreen(std::unique_ptr<Screen> screen);

    void update();

    Screen& peekScreen();

    bool isEmpty() const;

  private:
    std::stack<std::unique_ptr<Screen>> m_screens;
    std::vector<Action> m_actions;
};

class Screen {
  public:
    Screen(ScreenManager* screens);
    virtual ~Screen() = default;

    virtual void onGUI() {}
    virtual void onEvent([[maybe_unused]] const sf::Event& e) {}
    virtual void onInput([[maybe_unused]] const sf::RenderWindow& window) {}
    virtual void onUpdate([[maybe_unused]] const sf::Time& dt) {}
    virtual void onFixedUpdate([[maybe_unused]] const sf::Time& dt) {}
    virtual void onRender(sf::RenderWindow* window) = 0;

  protected:
    ScreenManager* m_pScreens;
};
