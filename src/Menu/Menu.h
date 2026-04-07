#pragma once
#include "imgui.h"
#include <array>
#include <string>
#include <vector>

class Visualizer;
class SortingAlgorithm;
class RecursionAlgorithm; 

struct SortCompareResult {
    std::string name;
    double      timeMs      = 0.0;
    int         comparisons = 0;
    int         writes      = 0; 
    int         totalOps    = 0;
};

class Menu {
public:
    static Menu& GetInstance() {
        static Menu instance;
        return instance;
    }
    // Main UI entry point; routes rendering and updates by selected mode.
    void Render();
    void Shutdown();

private:
    Menu();
    ~Menu() = default;

    void RenderSidebar();
    void RenderSortingTab(float sidebarWidth);
    void RenderGraphTab(float sidebarWidth);
    void RenderRecursionTab(float sidebarWidth);
    void RenderComparisonTable();
    // Runs one selected sorting algorithm and starts playback.
    void RunSelectedAlgorithm();
    // Executes all sorting algorithms on the same dataset for metrics comparison.
    void RunComparisonBenchmark();
    // Prepares and runs the selected recursion simulation.
    void RunRecursionSimulation();
    // Builds an MST graph instance and simulation steps without auto-playing.
    void GenerateGraphSimulation();
    // Starts graph simulation playback (generates first if needed).
    void RunGraphSimulation();

    Visualizer* m_Visualizer = nullptr;
    int m_SelectedSortingAlgorithm = 0;
    int m_SelectedRecursionSimulation = 0;
    int m_SelectedMode = 0;
    std::array<int, 3> m_RecursionNBySimulation = {5, 10, 4};
    bool m_HasRun = false;
    double m_LastExecutionTimeMs = 0.0;
    int m_LastOperationsCount = 0;

    
    // 0 = Kruskal, 1 = Prim.
    int m_SelectedGraphAlgorithm = 0;
    int m_GraphVertices = 6;
    bool m_GraphHasRun = false;

    
    // Cached benchmark rows shown in the comparison table.
    std::vector<SortCompareResult> m_CompareResults;
    bool m_ShowComparison = false;

};