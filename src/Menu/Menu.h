#pragma once
#include "imgui.h"

class Visualizer;
class SortingAlgorithm;

class Menu {
public:
    Menu(const Menu&) = delete;
    Menu& operator=(const Menu&) = delete;

    static Menu& GetInstance() {
        static Menu instance;
        return instance;
    }

    void Render();
    void Shutdown();

private:
    Menu();
    ~Menu() = default;
    
    void RenderSidebar();
    void RenderSortingTab(float sidebarWidth);
    void RenderGraphTab(float sidebarWidth);
    void RenderRecursionTab(float sidebarWidth);
    void RenderMainContent();
    void RunSelectedAlgorithm();
    
    Visualizer* m_Visualizer = nullptr;
    int m_SelectedAlgorithm = 0;
    int m_SelectedMode = 0;
    bool m_HasRun = false;
    double m_LastExecutionTimeMs = 0.0;
    int m_LastOperationsCount = 0;
};
