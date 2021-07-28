#include "Screen.h"
#include <imgui_sfml/imgui.h>

void ScreenManager::pushScreen(std::unique_ptr<Screen> screen)
{
    Action action;
    action.kind = Action::Kind::Push;
    action.screen = std::move(screen);
    m_actions.push_back(std::move(action));
}

void ScreenManager::popScreen()
{
    Action action;
    action.kind = Action::Kind::Pop;
    m_actions.push_back(std::move(action));
}

void ScreenManager::changeScreen(std::unique_ptr<Screen> screen)
{
    Action action;
    action.kind = Action::Kind::Change;
    action.screen = std::move(screen);
    m_actions.push_back(std::move(action));
}

void ScreenManager::update()
{
    for (Action& action : m_actions) {
        switch (action.kind) {
            case Action::Kind::Push:
                m_screens.push(std::move(action.screen));
                break;

            case Action::Kind::Pop:
                m_screens.pop();
                break;

            case Action::Kind::Change:
                while (!m_screens.empty()) {
                    m_screens.pop();
                }
                m_screens.push(std::move(action.screen));
                break;
        }
    }
    m_actions.clear();
}

Screen& ScreenManager::peekScreen() { return *m_screens.top(); }

bool ScreenManager::isEmpty() const { return m_screens.empty(); }

Screen::Screen(ScreenManager* screens)
    : m_pScreens(screens)
{
}
