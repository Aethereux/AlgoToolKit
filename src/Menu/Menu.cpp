#include "Menu.h"
#include "../Algorithms/SortingAlgorithm.h"
#include "../Resources/FontAwesome.h"
#include "../Visualizer/Visualizer.h"
#include <chrono>
#include "ImGuiHelper.h"

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

  // Determine what UI to show in the sidebar based on the top tabs
  switch (m_SelectedMode) {
    case 0: RenderSortingTab(sidebarWidth); break;
    case 1: RenderGraphTab(sidebarWidth); break;
    case 2: RenderRecursionTab(sidebarWidth); break;
  }

  // Spacer to push credits to bottom
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

  ImGui::BeginChild("main", ImVec2(0, 0), false);
  
  // Custom Horizontal Mode Tabs at top of the Graph area
  const char* modes[] = { "Sorting Algorithms", "MST Algorithms", "Recursion Simulation" };
  ImVec4 disabledColor = ImVec4(0.5f, 0.5f, 0.6f, 1.0f);
  ImGuiHelper::drawTabHorizontally("top_tabs", ImVec2(ImGui::GetContentRegionAvail().x, 45), modes, 3, m_SelectedMode, &disabledColor);

  if (m_Visualizer) {
    m_Visualizer->Update();
    m_Visualizer->Render();
  }
  ImGui::EndChild();

  ImGui::End();
}

void Menu::RenderSortingTab(float sidebarWidth) {
  // Algorithm Selection
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

    bool selected = (m_SelectedAlgorithm == i);
    if (ImGui::Selectable(label, selected, 0, ImVec2(sidebarWidth - 28, 28))) {
      m_SelectedAlgorithm = i;
      m_HasRun = false;
    }

    // Show hint below selected item
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
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.35f, 0.45f, 1.0f));
  if (ImGui::Button(ICON_FA_SYNC " Generate", ImVec2(halfBtn, 28))) {
      if (m_Visualizer) m_Visualizer->GenerateRandomArray(m_Visualizer->GetConfig().arraySize);
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_UNDO " Reset", ImVec2(halfBtn, 28))) {
      if (m_Visualizer) m_Visualizer->Reset();
  }
  ImGui::PopStyleColor(2);

  //  Run Button
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(14);

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.45f, 0.85f, 0.9f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.20f, 0.55f, 0.95f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                        ImVec4(0.10f, 0.35f, 0.75f, 1.0f));

  if (ImGui::Button(ICON_FA_PLAY_CIRCLE " Run Algorithm", ImVec2(sidebarWidth - 28, 36))) {
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
    ImGui::SetCursorPosX(16);
    ImGui::SliderInt("Speed##sp", &config.animationSpeed, 1, 200);
    ImGui::SetCursorPosX(16);

    const char *themes[] = {"Cyberpunk", "Ocean", "Sunset", "Matrix", "Pastel"};
    int themeIdx = static_cast<int>(config.theme);
    if (ImGui::Combo("Theme##th", &themeIdx, themes, 5))
      config.theme = static_cast<ColorTheme>(themeIdx);
    ImGui::SetCursorPosX(16);

    const char *vizTypes[] = {"Bar Graph",    "Line Graph",     "Circular",
                              "Scatter Plot", "Color Spectrum", "Starburst",
                              "Pyramid"};
    int vizIdx = static_cast<int>(config.vizType);
    if (ImGui::Combo("Type##ty", &vizIdx, vizTypes, 7))
      config.vizType = static_cast<VizType>(vizIdx);
    ImGui::SetCursorPosX(16);

    if (config.vizType == VizType::BarGraph) {
      const char *styles[] = {"Rounded", "Sharp", "Gradient"};
      int styleIdx = static_cast<int>(config.barStyle);
      if (ImGui::Combo("Style##st", &styleIdx, styles, 3))
        config.barStyle = static_cast<BarStyle>(styleIdx);
      ImGui::SetCursorPosX(16);
    }
    ImGui::PopItemWidth();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImGui::Checkbox("Smooth", &config.smoothAnimation);
    ImGui::SameLine();
    ImGui::Checkbox("Glow", &config.showGlow);
    ImGui::SameLine();
    ImGui::Checkbox("Values", &config.showValues);

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
    switch (m_SelectedAlgorithm) {
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
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.5f, 0.2f, 1.0f));
  ImGui::TextWrapped(ICON_FA_EXCLAMATION_TRIANGLE " MST Algorithms Not Yet Implemented");
  ImGui::PopStyleColor();
  ImGui::Dummy(ImVec2(0, 8));

  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_PROJECT_DIAGRAM "  ALGORITHMS");
  ImGui::PopStyleColor();
  
  ImGui::SetCursorPosX(24);
  ImGui::TextDisabled("Kruskal's Algorithm");
  ImGui::SetCursorPosX(24);
  ImGui::TextDisabled("Prim's Algorithm");

  ImGui::Dummy(ImVec2(0, 8));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 4));

  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_COG "  GRAPH INPUT");
  ImGui::PopStyleColor();
  
  ImGui::SetCursorPosX(16);
  static int numVertices = 5;
  ImGui::PushItemWidth(sidebarWidth - 80);
  ImGui::SliderInt("Vertices", &numVertices, 3, 20);
  ImGui::PopItemWidth();
  
  ImGui::SetCursorPosX(16);
  ImGui::TextDisabled("Edge Generation Setup");

  ImGui::Dummy(ImVec2(0, 16));
  ImGui::SetCursorPosX(12);
  ImGui::BeginDisabled(true);
  ImGui::Button("Run Graph Algorithm", ImVec2(sidebarWidth - 28, 36));
  ImGui::EndDisabled();
}

void Menu::RenderRecursionTab(float sidebarWidth) {
  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.5f, 0.2f, 1.0f));
  ImGui::TextWrapped(ICON_FA_EXCLAMATION_TRIANGLE " Recursion Sim Not Yet Implemented");
  ImGui::PopStyleColor();
  ImGui::Dummy(ImVec2(0, 8));

  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_REDO "  SIMULATIONS");
  ImGui::PopStyleColor();
  
  ImGui::SetCursorPosX(24);
  ImGui::TextDisabled("Factorial");
  ImGui::SetCursorPosX(24);
  ImGui::TextDisabled("Fibonacci");
  ImGui::SetCursorPosX(24);
  ImGui::TextDisabled("Tower of Hanoi");

  ImGui::Dummy(ImVec2(0, 8));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 4));

  ImGui::SetCursorPosX(16);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
  ImGui::Text(ICON_FA_COG "  VARIABLES");
  ImGui::PopStyleColor();
  
  ImGui::SetCursorPosX(16);
  static int nVal = 4;
  ImGui::PushItemWidth(sidebarWidth - 80);
  ImGui::SliderInt("N value", &nVal, 1, 10);
  ImGui::PopItemWidth();

  ImGui::Dummy(ImVec2(0, 16));
  ImGui::SetCursorPosX(12);
  ImGui::BeginDisabled(true);
  ImGui::Button("Run Simulation", ImVec2(sidebarWidth - 28, 36));
  ImGui::EndDisabled();
}

void Menu::RunSelectedAlgorithm() {
  if (!m_Visualizer)
    return;

  const std::vector<int> &array = m_Visualizer->GetOriginalArray();
  if (array.empty())
    return;

  m_Visualizer->Reset();

  std::vector<int> arrCopy = array;

  SortingAlgorithm *algorithm = nullptr;

  switch (m_SelectedAlgorithm) {
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

void Menu::Shutdown() {
  delete m_Visualizer;
  m_Visualizer = nullptr;
}
