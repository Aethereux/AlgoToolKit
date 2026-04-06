#pragma once
#include "imgui.h"
#include <array>

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
    void RunRecursionSimulation();
    void RunGraphSimulation();

    Visualizer* m_Visualizer = nullptr;
    int m_SelectedSortingAlgorithm = 0;
    int m_SelectedRecursionSimulation = 0;
    int m_SelectedMode = 0;
    std::array<int, 3> m_RecursionNBySimulation = {5, 10, 4};
    bool m_HasRun = false;
    double m_LastExecutionTimeMs = 0.0;
    int m_LastOperationsCount = 0;

    // Graph tab state
    int m_SelectedGraphAlgorithm = 0; // 0 = Kruskal's
    int m_GraphVertices = 6;
    bool m_GraphHasRun = false;
};