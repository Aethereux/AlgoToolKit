#pragma once
#include <string>
#include <vector>
#include "imgui.h"

namespace ImGuiHelper {
    float getWidth();
    float getHeight();
    ImVec2 getTextLength(std::string text);
    
    // Custom horizontal tab drawer (takes array of const char*)
    void drawTabHorizontally(const char* childName, ImVec2 childSize, const char* const* tabNames, int numTabs, int& selectedSubTab, const ImVec4* notSelectedColor = nullptr);
}
