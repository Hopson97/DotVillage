#include "GUI.h"

#include <imgui_sfml/imgui.h>


bool imguiBeginMenu(const char* name)
{
    ImVec2 windowSize(1280 / 4, 720 / 2);
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
