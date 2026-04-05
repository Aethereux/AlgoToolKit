#pragma once
#include "imgui.h"

class Visualizer;
class SortingAlgorithm;
class RecursionAlgorithm; // Forward declaration

class Menu {
public:
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
    void RunSelectedAlgorithm();
    void RunRecursionSimulation(); // Function declaration
    
    Visualizer* m_Visualizer = nullptr;
    int m_SelectedSortingAlgorithm = 0;
    int m_SelectedRecursionSimulation = 0;
    int m_SelectedMode = 0;
    int m_RecursionN = 5; // ADDED THIS: Resolves C2065
    bool m_HasRun = false;
    double m_LastExecutionTimeMs = 0.0;
    int m_LastOperationsCount = 0;
};