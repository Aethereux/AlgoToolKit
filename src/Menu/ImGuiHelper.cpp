#include "ImGuiHelper.h"
#include <algorithm>
#include <cstring>

float ImGuiHelper::getWidth() { 
    return ImGui::GetContentRegionAvail().x; 
}

float ImGuiHelper::getHeight() { 
    return ImGui::GetContentRegionAvail().y; 
}

ImVec2 ImGuiHelper::getTextLength(std::string text) { 
    return ImGui::CalcTextSize(text.c_str()); 
}

void ImGuiHelper::drawTabHorizontally(const char* childName, ImVec2 childSize, const char* const* tabNames, int numTabs, int& selectedSubTab, const ImVec4* notSelectedColor) {
    if (numTabs <= 0) return;
    
    size_t length = strlen(tabNames[0]);
    int strIndex = 0;
    for (int i = 1; i < numTabs; i++) {
        size_t len = strlen(tabNames[i]);
        if (length > len) {
            length = len;
            strIndex = i;
        }
    }

    ImGui::BeginChild(childName, childSize, false, ImGuiWindowFlags_HorizontalScrollbar);

    int minWidth = (int)ImGui::CalcTextSize(tabNames[strIndex]).x;
    int maxWidth = 200;

    int btnWidth = (int)((ImGuiHelper::getWidth() - ImGui::GetStyle().ItemSpacing.x * numTabs) / numTabs);
    int btnHeight = std::clamp((int)ImGuiHelper::getHeight(), 20, 60);
    btnWidth = std::max(minWidth, std::min(btnWidth, maxWidth));

    // center buttons
    ImGui::SetCursorPosX((childSize.x - btnWidth * numTabs - ImGui::GetStyle().ItemSpacing.x) / 2);

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f); 
    for (int i = 0; i < numTabs; i++) {
        const char* it = tabNames[i];
        
        ImGui::PushStyleColor(ImGuiCol_Button, selectedSubTab == i ? ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] : ImGui::GetStyle().Colors[ImGuiCol_Button]);
        
        ImVec4 textColor = ImGui::GetStyle().Colors[ImGuiCol_Text];
        if (selectedSubTab != i && notSelectedColor) {
            textColor = *notSelectedColor;
        }
        ImGui::PushStyleColor(ImGuiCol_Text, textColor);

        if (ImGui::Button(it, ImVec2((float)btnWidth, (float)btnHeight))) selectedSubTab = i;
        
        ImGui::SameLine();
        ImGui::PopStyleColor(2);
    }
    ImGui::PopStyleVar();

    ImGui::EndChild();
}
