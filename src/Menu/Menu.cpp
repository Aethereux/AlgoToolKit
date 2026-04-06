#include "Menu.h"
#include "../Algorithms/GraphAlgorithm.h"
#include "../Algorithms/RecursionAlgorithm.h"
#include "../Algorithms/SortingAlgorithm.h"
#include "../Resources/FontAwesome.h"
#include "../Visualizer/Visualizer.h"
#include "ImGuiHelper.h"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <random>

static int GetRecursionMaxForSimulation(int simulationIndex) {
  switch (simulationIndex) {
  case 0:
    return 12; // 13! overflows 32-bit int
  case 1:
    return 20; // naive recursive fibonacci gets expensive quickly
  case 2:
    return 10; // Tower of Hanoi grows as 2^n - 1 moves
  default:
    return 20;
  }
}

static int GetRecursionDefaultForSimulation(int simulationIndex) {
  switch (simulationIndex) {
  case 0:
    return 5;
  case 1:
    return 10;
  case 2:
    return 4;
  default:
    return 5;
  }
}

Menu::Menu() { m_Visualizer = new Visualizer(); }

void Menu::Render() {
  const ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                                  ImGuiWindowFlags_NoMove |
                                  ImGuiWindowFlags_NoSavedSettings;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::Begin("Algorithm Analysis Toolkit", nullptr, window_flags);
  ImGui::PopStyleVar();

  float sidebarWidth = 280.0f;

  // Sidebar
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.12f, 1.0f));
  ImGui::BeginChild("sidebar", ImVec2(sidebarWidth, 0), true);

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));

  // Header
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.7f, 1.0f, 1.0f));
  ImGui::SetCursorPosX(16);
  ImGui::Text(ICON_FA_FLASK "  AlgoToolKit");
  ImGui::PopStyleColor();

  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.6f, 0.6f));
  ImGui::Text("By Eux, Alex & Francine");
  ImGui::PopStyleColor();

  ImGui::Dummy(ImVec2(0, 4));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 4));

  // Determine which algorithm is selected
  switch (m_SelectedMode) {
  case 0:
    RenderSortingTab(sidebarWidth);
    break;
  case 1:
    RenderGraphTab(sidebarWidth);
    break;
  case 2:
    RenderRecursionTab(sidebarWidth);
    break;
  }

  // Spacer 
  float footerHeight = 50.0f;
  float available = ImGui::GetContentRegionAvail().y - footerHeight;
  if (available > 0)
    ImGui::Dummy(ImVec2(0, available));

  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35f, 0.35f, 0.4f, 0.7f));
  ImGui::Text(ICON_FA_TERMINAL "  AlgoToolKit v1.0");
  ImGui::PopStyleColor();

  ImGui::PopStyleVar(2);
  ImGui::EndChild();
  ImGui::PopStyleColor();

  ImGui::SameLine();

  ImGui::BeginChild("main", ImVec2(0, 0), false,
                    ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse);

  // Algorithm Selection
  const char *modes[] = {"Sorting Algorithms", "MST Algorithms",
                         "Recursion Simulation"};
  ImVec4 disabledColor = ImVec4(0.5f, 0.5f, 0.6f, 1.0f);
  ImGuiHelper::drawTabHorizontally("top_tabs",
                                   ImVec2(ImGui::GetContentRegionAvail().x, 45),
                                   modes, 3, m_SelectedMode, &disabledColor);
  if (m_SelectedMode == 1 && m_Visualizer) {
    m_Visualizer->UpdateKruskals(ImGui::GetIO().DeltaTime);
    m_Visualizer->RenderKruskals();
  } else if (m_SelectedMode == 2 && m_Visualizer) {
    if (m_SelectedRecursionSimulation == 2) {
      m_Visualizer->UpdateTowerOfHanoi(ImGui::GetIO().DeltaTime);
      m_Visualizer->RenderTowerOfHanoi();
    } else if (m_SelectedRecursionSimulation == 1) {
      m_Visualizer->RenderFibonacciGoldenRatio();
    } else {
      m_Visualizer->Update();
      m_Visualizer->Render();
    }
  } else if (m_Visualizer) {
    m_Visualizer->Update();
    m_Visualizer->Render();
  }
  ImGui::EndChild();

  ImGui::End();
}

// Sorting Algorithms Menu
void Menu::RenderSortingTab(float sidebarWidth) {
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_CODE "  ALGORITHMS");
  ImGui::PopStyleColor();
  ImGui::Dummy(ImVec2(0, 2));

  struct AlgoEntry {
    const char *icon;
    const char *name;
    const char *hint;
  };

  AlgoEntry algorithms[] = {
      {ICON_FA_SORT_AMOUNT_DOWN, "Bubble Sort", "Compare & swap adjacent"},
      {ICON_FA_CHART_BAR, "Selection Sort", "Find minimum each pass"},
      {ICON_FA_LAYER_GROUP, "Insertion Sort", "Insert into sorted half"}};

  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.20f, 0.25f, 0.40f, 0.6f));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered,
                        ImVec4(0.25f, 0.30f, 0.50f, 0.7f));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive,
                        ImVec4(0.30f, 0.35f, 0.55f, 0.9f));

  for (int i = 0; i < 3; i++) {
    ImGui::SetCursorPosX(12);

    char label[128];
    snprintf(label, sizeof(label), "%s  %s", algorithms[i].icon,
             algorithms[i].name);

    bool selected = (m_SelectedSortingAlgorithm == i);
    if (ImGui::Selectable(label, selected, 0, ImVec2(sidebarWidth - 28, 28))) {
      m_SelectedSortingAlgorithm = i;
      m_HasRun = false;
    }

    // Description of algorithm
    if (selected) {
      ImGui::SetCursorPosX(36);
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.6f, 0.8f));
      ImGui::TextWrapped("%s", algorithms[i].hint);
      ImGui::PopStyleColor();
    }
  }

  ImGui::PopStyleColor(3);

  // Generate & Reset Arrays
  ImGui::Dummy(ImVec2(0, 8));
  ImGui::SetCursorPosX(14);

  float halfBtn = (sidebarWidth - 28 - ImGui::GetStyle().ItemSpacing.x) / 2.0f;

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.25f, 0.35f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.30f, 0.35f, 0.45f, 1.0f));
  if (ImGui::Button(ICON_FA_SYNC " Generate", ImVec2(halfBtn, 28))) {
    if (m_Visualizer)
      m_Visualizer->GenerateRandomArray(m_Visualizer->GetConfig().arraySize);
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_UNDO " Reset", ImVec2(halfBtn, 28))) {
    if (m_Visualizer)
      m_Visualizer->Reset();
  }
  ImGui::PopStyleColor(2);

  // Run Button
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(14);

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.45f, 0.85f, 0.9f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.20f, 0.55f, 0.95f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                        ImVec4(0.10f, 0.35f, 0.75f, 1.0f));

  if (ImGui::Button(ICON_FA_PLAY_CIRCLE " Run Algorithm",
                    ImVec2(sidebarWidth - 28, 36))) {
    RunSelectedAlgorithm();
  }

  ImGui::PopStyleColor(3);

  //  Settings
  ImGui::Dummy(ImVec2(0, 8));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 4));

  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_COG "  SETTINGS");
  ImGui::PopStyleColor();
  ImGui::Dummy(ImVec2(0, 4));

  if (m_Visualizer) {
    auto &config = m_Visualizer->GetConfig();

    ImGui::SetCursorPosX(16);
    ImGui::PushItemWidth(sidebarWidth - 80);

    ImGui::SliderInt("Size##sz", &config.arraySize, 10, 200);
    ImGui::Dummy(ImVec2(0, 4));
    ImGui::SetCursorPosX(16);
    ImGui::SliderInt("Speed##sp", &config.animationSpeed, 1, 200);
    ImGui::Dummy(ImVec2(0, 4));
    ImGui::SetCursorPosX(16);

    const char *themes[] = {"Cyberpunk", "Ocean", "Sunset", "Matrix", "Pastel"};
    int themeIdx = static_cast<int>(config.theme);
    if (ImGui::Combo("Theme##th", &themeIdx, themes, 5))
      config.theme = static_cast<ColorTheme>(themeIdx);

    ImGui::PopItemWidth();

    ImGui::Dummy(ImVec2(0, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 6));
    ImGui::SetCursorPosX(16);
    ImGui::Checkbox("Smooth", &config.smoothAnimation);
    ImGui::SameLine();
    ImGui::Checkbox("Glow", &config.showGlow);
    ImGui::SameLine();
    ImGui::Checkbox("Values", &config.showValues);

    ImGui::Dummy(ImVec2(0, 4));
    ImGui::SetCursorPosX(16);
    ImGui::Checkbox("Grid", &config.showGrid);
    ImGui::SameLine();
    ImGui::Checkbox("Highlight", &config.highlightOps);
    ImGui::PopStyleVar();
  }

  //  Complexity Info
  if (m_HasRun) {
  ImGui::Dummy(ImVec2(0, 8));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 4));

    ImGui::SetCursorPosX(16);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
    ImGui::Text(ICON_FA_INFO_CIRCLE "  COMPLEXITY");
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 4));

    const char *name;
    const char *timeC;
    const char *spaceC;
    const char *bestC;
    switch (m_SelectedSortingAlgorithm) {
    case 0:
      name = "Bubble Sort";
      timeC = "O(n²)";
      spaceC = "O(1)";
      bestC = "O(n)";
      break;
    case 1:
      name = "Selection Sort";
      timeC = "O(n²)";
      spaceC = "O(1)";
      bestC = "O(n²)";
      break;
    case 2:
      name = "Insertion Sort";
      timeC = "O(n²)";
      spaceC = "O(1)";
      bestC = "O(n)";
      break;
    default:
      name = "";
      timeC = "";
      spaceC = "";
      bestC = "";
      break;
    }

    auto InfoRow = [](const char *icon, const char *label, const char *value) {
      ImGui::SetCursorPosX(20);
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.6f, 1.0f));
      ImGui::Text("%s %s:", icon, label);
      ImGui::PopStyleColor();
      ImGui::SameLine(160);
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.8f, 0.3f, 1.0f));
      ImGui::Text("%s", value);
      ImGui::PopStyleColor();
    };

    InfoRow(ICON_FA_CLOCK, "Worst", timeC);
    InfoRow(ICON_FA_BOLT, "Best", bestC);
    InfoRow(ICON_FA_MEMORY, "Space", spaceC);

    ImGui::Dummy(ImVec2(0, 8));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 4));

    ImGui::SetCursorPosX(16);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
    ImGui::Text(ICON_FA_CHART_PIE "  PROFILER");
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 4));

    char timeBuf[32];
    snprintf(timeBuf, sizeof(timeBuf), "%.3f ms", m_LastExecutionTimeMs);
    char opsBuf[32];
    snprintf(opsBuf, sizeof(opsBuf), "%d", m_LastOperationsCount);

    InfoRow(ICON_FA_STOPWATCH, "Time", timeBuf);
    InfoRow(ICON_FA_SLIDERS_H, "Ops", opsBuf);
  }
}

void Menu::RenderGraphTab(float sidebarWidth) {
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_PROJECT_DIAGRAM "  ALGORITHMS");
  ImGui::PopStyleColor();
  ImGui::Dummy(ImVec2(0, 2));

  ImGui::PushStyleColor(ImGuiCol_Header,        ImVec4(0.20f, 0.25f, 0.40f, 0.6f));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.25f, 0.30f, 0.50f, 0.7f));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive,  ImVec4(0.30f, 0.35f, 0.55f, 0.9f));

  // Kruskal's — selectable
  ImGui::SetCursorPosX(12);
  if (ImGui::Selectable(ICON_FA_SITEMAP "  Kruskal's Algorithm",
                        m_SelectedGraphAlgorithm == 0, 0,
                        ImVec2(sidebarWidth - 28, 28))) {
    m_SelectedGraphAlgorithm = 0;
    m_GraphHasRun = false;
  }
  if (m_SelectedGraphAlgorithm == 0) {
    ImGui::SetCursorPosX(36);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.6f, 0.8f));
    ImGui::TextWrapped("Sort edges, add if no cycle");
    ImGui::PopStyleColor();
  }

  // Prim's
  ImGui::SetCursorPosX(12);
  if (ImGui::Selectable(ICON_FA_CODE_BRANCH "  Prim's Algorithm",
                        m_SelectedGraphAlgorithm == 1, 0,
                        ImVec2(sidebarWidth - 28, 28))) {
    m_SelectedGraphAlgorithm = 1;
    m_GraphHasRun = false;
  }
  if (m_SelectedGraphAlgorithm == 1) {
    ImGui::SetCursorPosX(36);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.6f, 0.8f));
    ImGui::TextWrapped("Grow MST from a starting vertex");
    ImGui::PopStyleColor();
  }

  ImGui::PopStyleColor(3);

  // Generate + Run button
  ImGui::Dummy(ImVec2(0, 8));
  ImGui::SetCursorPosX(14);

  float halfBtn = (sidebarWidth - 28 - ImGui::GetStyle().ItemSpacing.x) / 2.0f;
  ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.20f, 0.25f, 0.35f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.35f, 0.45f, 1.0f));
  if (ImGui::Button(ICON_FA_SYNC " Generate", ImVec2(halfBtn, 28))) {
    RunGraphSimulation();
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_UNDO " Reset", ImVec2(halfBtn, 28))) {
    if (m_Visualizer) m_Visualizer->ClearGraphSimulation();
    m_GraphHasRun = false;
  }
  ImGui::PopStyleColor(2);

  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(14);
  ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.15f, 0.45f, 0.85f, 0.9f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.20f, 0.55f, 0.95f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.10f, 0.35f, 0.75f, 1.0f));
  const char *runLabel = (m_SelectedGraphAlgorithm == 1)
                             ? ICON_FA_PLAY_CIRCLE " Run Prim's"
                             : ICON_FA_PLAY_CIRCLE " Run Kruskal's";
  if (ImGui::Button(runLabel, ImVec2(sidebarWidth - 28, 36))) {
    RunGraphSimulation();
  }
  ImGui::PopStyleColor(3);

  // Settings
  ImGui::Dummy(ImVec2(0, 8));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_COG "  SETTINGS");
  ImGui::PopStyleColor();
  ImGui::Dummy(ImVec2(0, 4));

  ImGui::SetCursorPosX(16);
  ImGui::PushItemWidth(sidebarWidth - 80);
  ImGui::SliderInt("Vertices##gv", &m_GraphVertices, 3, 10);
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(16);
  if (m_Visualizer)
    ImGui::SliderInt("Speed##gs", &m_Visualizer->GetConfig().animationSpeed, 1, 200);
  ImGui::PopItemWidth();

  // Complexity info
  if (m_GraphHasRun) {
    ImGui::Dummy(ImVec2(0, 8));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 4));
    ImGui::SetCursorPosX(16);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
    ImGui::Text(ICON_FA_INFO_CIRCLE "  COMPLEXITY");
    ImGui::PopStyleColor();
    ImGui::Dummy(ImVec2(0, 4));

    auto InfoRow = [](const char *icon, const char *label, const char *value) {
      ImGui::SetCursorPosX(20);
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.6f, 1.0f));
      ImGui::Text("%s %s:", icon, label);
      ImGui::PopStyleColor();
      ImGui::SameLine(160);
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.8f, 0.3f, 1.0f));
      ImGui::Text("%s", value);
      ImGui::PopStyleColor();
    };

    if (m_SelectedGraphAlgorithm == 1) {
      InfoRow(ICON_FA_CLOCK,  "Time",  "O(E log V)");
      InfoRow(ICON_FA_MEMORY, "Space", "O(V + E)");
    } else {
      InfoRow(ICON_FA_CLOCK,  "Time",  "O(E log E)");
      InfoRow(ICON_FA_MEMORY, "Space", "O(V + E)");
    }
  }
}

void Menu::RenderRecursionTab(float sidebarWidth) {
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.5f, 0.2f, 1.0f));
  ImGui::PopStyleColor();
  ImGui::Dummy(ImVec2(0, 8));

  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_REDO "  SIMULATIONS");
  ImGui::PopStyleColor();

  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.20f, 0.25f, 0.40f, 0.6f));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered,
                        ImVec4(0.25f, 0.30f, 0.50f, 0.7f));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive,
                        ImVec4(0.30f, 0.35f, 0.55f, 0.9f));

  const char *simulations[] = {"Factorial", "Fibonacci", "Tower of Hanoi"};
  for (int i = 0; i < 3; i++) {
    ImGui::SetCursorPosX(12);
    bool selected = (m_SelectedRecursionSimulation == i);
    if (ImGui::Selectable(simulations[i], selected, 0,
                          ImVec2(sidebarWidth - 28, 28))) {
      m_SelectedRecursionSimulation = i;
      m_RecursionN = std::clamp(m_RecursionN, 1, GetRecursionMaxForSimulation(i));
      if (m_SelectedRecursionSimulation != 2 && m_Visualizer)
        m_Visualizer->ClearTowerOfHanoiSimulation();
      if (m_SelectedRecursionSimulation != 2 && m_Visualizer)
        m_Visualizer->ClearTowerTrace();
      if (m_SelectedRecursionSimulation != 1 && m_Visualizer)
        m_Visualizer->ClearFibonacciSimulation();
    }

    if (selected) {
      ImGui::SetCursorPosX(36);
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.6f, 0.8f));
      ImGui::PopStyleColor();
    }
  }

  ImGui::PopStyleColor(3);

  ImGui::Dummy(ImVec2(0, 8));
  ImGui::SetCursorPosX(12);
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.25f, 0.35f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.30f, 0.35f, 0.45f, 1.0f));
  if (ImGui::Button(ICON_FA_UNDO " Reset", ImVec2(sidebarWidth - 28, 28))) {
    m_RecursionN = GetRecursionDefaultForSimulation(m_SelectedRecursionSimulation);
    if (m_Visualizer) {
      if (m_SelectedRecursionSimulation == 2) {
        m_Visualizer->SetTowerOfHanoiSimulation({}, m_RecursionN);
        m_Visualizer->ClearTowerTrace();
      } else if (m_SelectedRecursionSimulation == 1) {
        m_Visualizer->ResetFibonacciSimulation();
      } else {
        m_Visualizer->ClearTowerOfHanoiSimulation();
        m_Visualizer->ClearFibonacciSimulation();
      }
      m_Visualizer->Reset();
    }
  }
  ImGui::PopStyleColor(2);

  ImGui::Dummy(ImVec2(0, 12));
  ImGui::SetCursorPosX(12);
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.45f, 0.85f, 0.9f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.20f, 0.55f, 0.95f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                        ImVec4(0.10f, 0.35f, 0.75f, 1.0f));
  if (ImGui::Button(ICON_FA_PLAY_CIRCLE " Run Simulation",
                    ImVec2(sidebarWidth - 28, 36))) {
    RunRecursionSimulation();
  }
  ImGui::PopStyleColor(3);

    ImGui::Dummy(ImVec2(0, 8));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 4));

  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_COG "  SETTINGS");
  ImGui::PopStyleColor();
  ImGui::Dummy(ImVec2(0, 4));

  if (m_Visualizer) {
    auto &config = m_Visualizer->GetConfig();
    const int recursionMax = GetRecursionMaxForSimulation(m_SelectedRecursionSimulation);
    m_RecursionN = std::clamp(m_RecursionN, 1, recursionMax);

    ImGui::SetCursorPosX(16);
    ImGui::PushItemWidth(sidebarWidth - 80);

    ImGui::SliderInt("N value", &m_RecursionN, 1, recursionMax);
    ImGui::Dummy(ImVec2(0, 4));
    ImGui::SetCursorPosX(16);
    ImGui::SliderInt("Speed##rec_sp", &config.animationSpeed, 1, 200);
    ImGui::Dummy(ImVec2(0, 4));
    ImGui::SetCursorPosX(16);

    const char *themes[] = {"Cyberpunk", "Ocean", "Sunset", "Matrix", "Pastel"};
    int themeIdx = static_cast<int>(config.theme);
    if (ImGui::Combo("Theme##rec_th", &themeIdx, themes, 5))
      config.theme = static_cast<ColorTheme>(themeIdx);
  }
}

void Menu::RunSelectedAlgorithm() {
  if (!m_Visualizer)
    return;

  if (m_Visualizer->IsPlaying())
    return;

  const std::vector<int> &array = m_Visualizer->GetOriginalArray();
  if (array.empty())
    return;

  m_Visualizer->SetVisualizationMode(VisualizationMode::BarGraph);
  m_Visualizer->Reset();

  std::vector<int> arrCopy = array;

  SortingAlgorithm *algorithm = nullptr;

  switch (m_SelectedSortingAlgorithm) {
  case 0:
    algorithm = new BubbleSort(m_Visualizer);
    break;
  case 1:
    algorithm = new SelectionSort(m_Visualizer);
    break;
  case 2:
    algorithm = new InsertionSort(m_Visualizer);
    break;
  }

  if (algorithm) {
    auto start = std::chrono::high_resolution_clock::now();
    algorithm->Sort(arrCopy);
    auto end = std::chrono::high_resolution_clock::now();

    m_LastExecutionTimeMs =
        std::chrono::duration<double, std::milli>(end - start).count();
    m_LastOperationsCount = m_Visualizer->GetTotalSteps();

    delete algorithm;
  }

  m_HasRun = true;
  m_Visualizer->Play();
}

void Menu::RunGraphSimulation() {
  if (!m_Visualizer) return;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> weightDis(1, 20);
  std::uniform_int_distribution<> chanceDis(0, 99);

  int n = m_GraphVertices;

  // Match the same grid layout used by the renderer
  int cols = (int)ceilf(sqrtf((float)n));

  std::vector<Edge> edges;

  auto addEdge = [&](int u, int v) {
    // Skip duplicates
    for (const auto &e : edges)
      if ((e.src == u && e.dest == v) || (e.src == v && e.dest == u))
        return;
    edges.push_back({u, v, weightDis(gen)});
  };

  // Horizontal neighbours (same row, adjacent columns) — always added
  for (int i = 0; i < n; i++) {
    int col = i % cols;
    if (col + 1 < cols && i + 1 < n)
      addEdge(i, i + 1);
  }

  // Vertical neighbours (same column, next row) — always added
  for (int i = 0; i < n; i++) {
    if (i + cols < n)
      addEdge(i, i + cols);
  }

  // Diagonal neighbours — added ~60% of the time to create cycles without
  // introducing long crossing edges
  for (int i = 0; i < n; i++) {
    int col = i % cols;
    // Down-right diagonal
    if (col + 1 < cols && i + cols + 1 < n && chanceDis(gen) < 60)
      addEdge(i, i + cols + 1);
    // Down-left diagonal
    if (col - 1 >= 0 && i + cols - 1 < n && chanceDis(gen) < 60)
      addEdge(i, i + cols - 1);
  }

  if (m_SelectedGraphAlgorithm == 1) {
    // Prim's
    PrimGraph pg;
    pg.SetGraphInfo(n, edges);
    pg.SimulatePrims();
    m_Visualizer->SetGraphSimulation(n, edges, pg.GetSteps());
  } else {
    // Kruskal's
    KruskalGraph kg;
    kg.SetGraphInfo(n, edges);
    kg.SimulateKruskals();
    m_Visualizer->SetGraphSimulation(n, edges, kg.GetSteps());
  }

  m_GraphHasRun = true;
}

void Menu::Shutdown() {
  delete m_Visualizer;
  m_Visualizer = nullptr;
}

void Menu::RunRecursionSimulation() {
  if (!m_Visualizer)
    return;

  if (m_Visualizer->IsPlaying())
    return;

  m_RecursionN = std::clamp(m_RecursionN, 1, GetRecursionMaxForSimulation(m_SelectedRecursionSimulation));
  m_Visualizer->Reset();
  m_Visualizer->ClearSteps();

  RecursionAlgorithm *sim = nullptr;

  switch (m_SelectedRecursionSimulation) {
  case 0: {
    sim = new FactorialAlgorithm(m_Visualizer);
    m_Visualizer->SetVisualizationMode(VisualizationMode::FactorialLadder);
    sim->Factorial(m_RecursionN);
    m_Visualizer->ClearTowerOfHanoiSimulation();
    m_Visualizer->ClearFibonacciSimulation();
    break;
  }
  case 1: {
    Recursion recursion;
    std::vector<int> sequence;
    sequence.reserve(static_cast<size_t>(m_RecursionN));
    for (int i = 1; i <= m_RecursionN; ++i)
      sequence.push_back(recursion.Fibonacci(i));
    m_Visualizer->SetFibonacciSimulation(sequence);
    m_Visualizer->ClearTowerOfHanoiSimulation();
    m_Visualizer->SetVisualizationMode(VisualizationMode::BarGraph);
    break;
  }
  case 2: {
    Recursion recursion;
    recursion.TowerOfHanoi(m_RecursionN, 'A', 'B', 'C');
    m_Visualizer->SetTowerOfHanoiSimulation(recursion.GetTowerMoves(),
                                            m_RecursionN);
    m_Visualizer->SetTowerTrace(recursion.GetTowerTrace());
    m_Visualizer->SetVisualizationMode(VisualizationMode::BarGraph);
    m_Visualizer->ClearFibonacciSimulation();
    break;
  }
  default:
    m_Visualizer->ClearTowerOfHanoiSimulation();
    m_Visualizer->ClearFibonacciSimulation();
    m_Visualizer->SetVisualizationMode(VisualizationMode::BarGraph);
    break;
  }

  if (sim) {
    delete sim;
    m_HasRun = true;
    m_Visualizer->Play();
  } else if (m_SelectedRecursionSimulation == 1 ||
             m_SelectedRecursionSimulation == 2) {
    m_HasRun = true;
    m_Visualizer->Play();
  }
}