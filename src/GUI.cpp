#include "GUI.h"

#include "Screen.h"
#include <imgui_sfml/imgui.h>

bool imguiBeginMenu(const char* name)
{
    ImVec2 windowSize(WIN_WIDTH / 4, WIN_HEIGHT / 2);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos({windowSize.x + windowSize.x * 4 / 8.0f, windowSize.y / 2},
                            ImGuiCond_Always);
    return ImGui::Begin(name, nullptr,
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize |
                            ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_AlwaysAutoResize);
}

bool imguiButtonCustom(const char* text)
{
    ImGui::SetCursorPos({ImGui::GetCursorPosX() + 100, ImGui::GetCursorPosY() + 20});
    return ImGui::Button(text, {100, 50});
}

bool imguiGameMenuBegin(const char* name)
{
    ImVec2 windowSize(WIN_WIDTH, 128);
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos({0, WIN_HEIGHT - 128}, ImGuiCond_Always);
    return ImGui::Begin(name, nullptr,
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);
}