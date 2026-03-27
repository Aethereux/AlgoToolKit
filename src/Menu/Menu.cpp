#include "Menu.h"
#include <iostream>

void Menu::Render() {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | 
                                    ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoSavedSettings;

    ImGui::Begin("Menu", nullptr, window_flags);

    ImGui::Text("Hello World!");


    ImGui::End();
}


