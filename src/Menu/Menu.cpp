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

static void ApplyDeepSpaceTheme() {
  ImGui::StyleColorsDark();
  ImGuiStyle &s  = ImGui::GetStyle();
  ImVec4     *c  = s.Colors;

  s.WindowRounding    = 6.0f;
  s.ChildRounding     = 6.0f;
  s.FrameRounding     = 5.0f;
  s.PopupRounding     = 5.0f;
  s.GrabRounding      = 4.0f;
  s.TabRounding       = 5.0f;
  s.ScrollbarRounding = 6.0f;
  s.WindowBorderSize  = 0.0f;
  s.ChildBorderSize   = 1.0f;
  s.FrameBorderSize   = 0.0f;
  s.ScrollbarSize     = 10.0f;
  s.GrabMinSize       = 8.0f;

  c[ImGuiCol_WindowBg]           = ImVec4(0.051f, 0.067f, 0.090f, 1.000f);
  c[ImGuiCol_ChildBg]            = ImVec4(0.039f, 0.055f, 0.078f, 1.000f);
  c[ImGuiCol_PopupBg]            = ImVec4(0.078f, 0.102f, 0.149f, 0.980f);
  c[ImGuiCol_MenuBarBg]          = ImVec4(0.039f, 0.055f, 0.078f, 1.000f);

  c[ImGuiCol_Border]             = ImVec4(0.149f, 0.204f, 0.314f, 0.700f);
  c[ImGuiCol_BorderShadow]       = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
  c[ImGuiCol_Separator]          = ImVec4(0.149f, 0.204f, 0.314f, 0.600f);

  c[ImGuiCol_Text]               = ImVec4(0.878f, 0.910f, 1.000f, 1.000f);
  c[ImGuiCol_TextDisabled]       = ImVec4(0.310f, 0.380f, 0.530f, 1.000f);

  c[ImGuiCol_FrameBg]            = ImVec4(0.067f, 0.094f, 0.157f, 1.000f);
  c[ImGuiCol_FrameBgHovered]     = ImVec4(0.102f, 0.157f, 0.251f, 1.000f);
  c[ImGuiCol_FrameBgActive]      = ImVec4(0.118f, 0.188f, 0.314f, 1.000f);

  c[ImGuiCol_TitleBg]            = ImVec4(0.039f, 0.055f, 0.086f, 1.000f);
  c[ImGuiCol_TitleBgActive]      = ImVec4(0.051f, 0.071f, 0.110f, 1.000f);
  c[ImGuiCol_TitleBgCollapsed]   = ImVec4(0.039f, 0.055f, 0.086f, 0.750f);

  c[ImGuiCol_ScrollbarBg]        = ImVec4(0.039f, 0.055f, 0.078f, 1.000f);
  c[ImGuiCol_ScrollbarGrab]      = ImVec4(0.102f, 0.165f, 0.275f, 1.000f);
  c[ImGuiCol_ScrollbarGrabHovered]= ImVec4(0.141f, 0.204f, 0.376f, 1.000f);
  c[ImGuiCol_ScrollbarGrabActive]= ImVec4(0.000f, 0.749f, 1.000f, 0.800f);

  c[ImGuiCol_CheckMark]          = ImVec4(0.000f, 0.749f, 1.000f, 1.000f);
  c[ImGuiCol_SliderGrab]         = ImVec4(0.000f, 0.659f, 0.910f, 1.000f);
  c[ImGuiCol_SliderGrabActive]   = ImVec4(0.000f, 0.749f, 1.000f, 1.000f);

  c[ImGuiCol_Button]             = ImVec4(0.102f, 0.141f, 0.251f, 1.000f);
  c[ImGuiCol_ButtonHovered]      = ImVec4(0.141f, 0.196f, 0.376f, 1.000f);
  c[ImGuiCol_ButtonActive]       = ImVec4(0.000f, 0.447f, 0.700f, 1.000f);

  c[ImGuiCol_Header]             = ImVec4(0.102f, 0.165f, 0.282f, 0.700f);
  c[ImGuiCol_HeaderHovered]      = ImVec4(0.141f, 0.204f, 0.376f, 0.800f);
  c[ImGuiCol_HeaderActive]       = ImVec4(0.000f, 0.447f, 0.700f, 0.900f);

  c[ImGuiCol_Tab]                = ImVec4(0.067f, 0.094f, 0.157f, 1.000f);
  c[ImGuiCol_TabHovered]         = ImVec4(0.000f, 0.549f, 0.800f, 0.800f);
  c[ImGuiCol_TabActive]          = ImVec4(0.000f, 0.447f, 0.700f, 1.000f);
  c[ImGuiCol_TabUnfocused]       = ImVec4(0.051f, 0.071f, 0.110f, 1.000f);
  c[ImGuiCol_TabUnfocusedActive] = ImVec4(0.067f, 0.110f, 0.200f, 1.000f);

  c[ImGuiCol_PlotHistogram]      = ImVec4(0.000f, 0.749f, 1.000f, 1.000f);
  c[ImGuiCol_PlotHistogramHovered]= ImVec4(0.310f, 0.859f, 1.000f, 1.000f);
  c[ImGuiCol_PlotLines]          = ImVec4(0.000f, 0.749f, 1.000f, 0.800f);
  c[ImGuiCol_PlotLinesHovered]   = ImVec4(0.310f, 0.859f, 1.000f, 1.000f);

  c[ImGuiCol_TableHeaderBg]      = ImVec4(0.059f, 0.082f, 0.125f, 1.000f);
  c[ImGuiCol_TableBorderStrong]  = ImVec4(0.149f, 0.204f, 0.314f, 1.000f);
  c[ImGuiCol_TableBorderLight]   = ImVec4(0.102f, 0.141f, 0.227f, 1.000f);
  c[ImGuiCol_TableRowBg]         = ImVec4(0.039f, 0.055f, 0.090f, 0.400f);
  c[ImGuiCol_TableRowBgAlt]      = ImVec4(0.059f, 0.082f, 0.137f, 0.400f);

  c[ImGuiCol_ResizeGrip]         = ImVec4(0.000f, 0.749f, 1.000f, 0.200f);
  c[ImGuiCol_ResizeGripHovered]  = ImVec4(0.000f, 0.749f, 1.000f, 0.600f);
  c[ImGuiCol_ResizeGripActive]   = ImVec4(0.000f, 0.749f, 1.000f, 0.900f);
  c[ImGuiCol_NavHighlight]       = ImVec4(0.000f, 0.749f, 1.000f, 1.000f);
}

static int GetRecursionMaxForSimulation(int simulationIndex) {
  switch (simulationIndex) {
  case 0:
    return 12;
  case 1:
    return 20;
  case 2:
    return 10;
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

Menu::Menu() {
  m_Visualizer = new Visualizer();
  ApplyDeepSpaceTheme();
}

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

  float totalWidth   = ImGuiHelper::getWidth();
  float sidebarWidth = std::min(280.0f, totalWidth * 0.30f);

  ImGui::BeginChild("sidebar", ImVec2(sidebarWidth, 0), true,
                    ImGuiWindowFlags_NoScrollbar);

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 6));

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

  float footerHeight = 36.0f;
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

  ImGui::SameLine();

  ImGui::BeginChild("main", ImVec2(0, 0), false,
                    ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoScrollWithMouse);

  const char *modes[] = {"Sorting Algorithms", "MST Algorithms",
                         "Recursion Simulation"};
  if (m_SelectedMode < 0 || m_SelectedMode > 2)
    m_SelectedMode = 0;
  ImVec4 disabledColor = ImVec4(0.5f, 0.5f, 0.6f, 1.0f);
  const int previousMode = m_SelectedMode;
  ImGuiHelper::drawTabHorizontally("top_tabs",
                                   ImVec2(ImGui::GetContentRegionAvail().x, 45),
                                   modes, 3, m_SelectedMode, &disabledColor);
  if (m_Visualizer && previousMode != m_SelectedMode) {
    if (m_SelectedMode == 2) {
      m_SelectedRecursionSimulation = 0;
      m_Visualizer->SetVisualizationMode(VisualizationMode::FactorialLadder);
      m_Visualizer->Pause();
      m_Visualizer->ClearSteps();
      m_Visualizer->ClearTowerOfHanoiSimulation();
      m_Visualizer->ClearTowerTrace();
      m_Visualizer->ClearFibonacciSimulation();
    } else {
      m_Visualizer->SetVisualizationMode(VisualizationMode::BarGraph);
      m_Visualizer->Pause();
      m_Visualizer->ClearSteps();
    }
  }

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
    if (m_ShowComparison && m_SelectedMode == 0) {
      RenderComparisonTable();
    } else {
      m_Visualizer->Update();
      m_Visualizer->Render();
    }
  }
  ImGui::EndChild();

  ImGui::End();
}

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

    ImGui::SetCursorPosX(36);
    ImGui::PushStyleColor(ImGuiCol_Text, selected
        ? ImVec4(0.5f, 0.5f, 0.6f, 0.8f)
        : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::TextUnformatted(algorithms[i].hint);
    ImGui::PopStyleColor();
  }

  ImGui::PopStyleColor(3);

  ImGui::Dummy(ImVec2(0, 8));
  ImGui::SetCursorPosX(14);

  float halfBtn = (sidebarWidth - 28 - ImGui::GetStyle().ItemSpacing.x) / 2.0f;

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.20f, 0.25f, 0.35f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.30f, 0.35f, 0.45f, 1.0f));
  if (ImGui::Button(ICON_FA_SYNC " Generate", ImVec2(halfBtn, 28))) {
    if (m_Visualizer) {
      m_Visualizer->GenerateRandomArray(m_Visualizer->GetConfig().arraySize);
      m_ShowComparison = false;
      m_CompareResults.clear();
    }
  }
  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_UNDO " Reset", ImVec2(halfBtn, 28))) {
    if (m_Visualizer)
      m_Visualizer->Reset();
  }
  ImGui::PopStyleColor(2);

  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(14);

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.45f, 0.85f, 0.9f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4(0.20f, 0.55f, 0.95f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                        ImVec4(0.10f, 0.35f, 0.75f, 1.0f));

  if (ImGui::Button(ICON_FA_PLAY_CIRCLE " Run Algorithm",
                    ImVec2(sidebarWidth - 28, 36))) {
    m_ShowComparison = false;
    RunSelectedAlgorithm();
  }

  ImGui::PopStyleColor(3);

  ImGui::Dummy(ImVec2(0, 4));
  ImGui::SetCursorPosX(14);
  ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.18f, 0.22f, 0.32f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.30f, 0.45f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.12f, 0.16f, 0.26f, 1.0f));
  if (ImGui::Button(ICON_FA_CHART_BAR " Compare All",
                    ImVec2(sidebarWidth - 28, 28))) {
    RunComparisonBenchmark();
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

    ImGui::SetCursorPosX(16);
    ImGui::PushItemWidth(sidebarWidth - 80);

    ImGui::SliderInt("Size##sz", &config.arraySize, 10, 200);
    ImGui::Dummy(ImVec2(0, 4));
    ImGui::SetCursorPosX(16);
    ImGui::SliderInt("Speed##sp", &config.animationSpeed, 1, 200);

    ImGui::PopItemWidth();

    ImGui::Dummy(ImVec2(0, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 6));
    ImGui::SetCursorPosX(16);
    ImGui::Columns(3, nullptr, false);
    ImGui::Checkbox("Smooth", &config.smoothAnimation);
    ImGui::Checkbox("Grid", &config.showGrid);
    ImGui::NextColumn();
    ImGui::Checkbox("Glow", &config.showGlow);
    ImGui::Checkbox("Highlight", &config.highlightOps);
    ImGui::NextColumn();
    ImGui::Checkbox("Values", &config.showValues);
    ImGui::PopStyleVar();
    ImGui::Columns(1);
  }

  if (m_HasRun) {
    ImGui::Dummy(ImVec2(0, 8));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 4));

    ImGui::SetCursorPosX(16);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.7f, 1.0f));
    ImGui::Text(ICON_FA_CHART_PIE "  PROFILER");
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

  ImGui::Dummy(ImVec2(0, 8));
  ImGui::SetCursorPosX(14);

  float halfBtn = (sidebarWidth - 28 - ImGui::GetStyle().ItemSpacing.x) / 2.0f;
  ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.20f, 0.25f, 0.35f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.35f, 0.45f, 1.0f));
  if (ImGui::Button(ICON_FA_SYNC " Generate", ImVec2(halfBtn, 28))) {
    GenerateGraphSimulation();
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
  const char *simulationHints[] = {
      "Multiply n down to one",
      "Sum of two previous numbers",
      "Stack smaller disks on larger"};
  for (int i = 0; i < 3; i++) {
    ImGui::SetCursorPosX(12);
    bool selected = (m_SelectedRecursionSimulation == i);
    if (ImGui::Selectable(simulations[i], selected, 0,
                          ImVec2(sidebarWidth - 28, 28))) {
      m_SelectedRecursionSimulation = i;
      m_RecursionNBySimulation[i] =
          std::clamp(m_RecursionNBySimulation[i], 1, GetRecursionMaxForSimulation(i));
      if (m_Visualizer) {
        switch (i) {
        case 0:
          m_Visualizer->SetVisualizationMode(VisualizationMode::FactorialLadder);
          break;
        case 1:
        case 2:
        default:
          m_Visualizer->SetVisualizationMode(VisualizationMode::BarGraph);
          break;
        }
      }
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
      ImGui::TextWrapped("%s", simulationHints[i]);
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
    m_RecursionNBySimulation[m_SelectedRecursionSimulation] =
        GetRecursionDefaultForSimulation(m_SelectedRecursionSimulation);
    if (m_Visualizer) {
      if (m_SelectedRecursionSimulation == 2) {
        m_Visualizer->ClearTowerOfHanoiSimulation();
        m_Visualizer->ClearTowerTrace();
      } else if (m_SelectedRecursionSimulation == 1) {
        m_Visualizer->ClearFibonacciSimulation();
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
    int &currentRecursionN = m_RecursionNBySimulation[m_SelectedRecursionSimulation];
    currentRecursionN = std::clamp(currentRecursionN, 1, recursionMax);

    ImGui::SetCursorPosX(16);
    ImGui::PushItemWidth(sidebarWidth - 80);

    ImGui::SliderInt("N value", &currentRecursionN, 1, recursionMax);
    ImGui::Dummy(ImVec2(0, 4));
    ImGui::SetCursorPosX(16);
    ImGui::SliderInt("Speed##rec_sp", &config.animationSpeed, 1, 200);

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

void Menu::RunComparisonBenchmark() {
  if (!m_Visualizer)
    return;

  const std::vector<int> &original = m_Visualizer->GetOriginalArray();
  if (original.empty())
    return;

  m_CompareResults.clear();

  SortingAlgorithm *algos[3] = {
      new BubbleSort(m_Visualizer),
      new SelectionSort(m_Visualizer),
      new InsertionSort(m_Visualizer),
  };
  const char *names[3] = {"Bubble Sort", "Selection Sort", "Insertion Sort"};

  for (int i = 0; i < 3; ++i) {
    m_Visualizer->Reset();
    m_Visualizer->ClearSteps();

    std::vector<int> arrCopy = original;

    auto t0 = std::chrono::high_resolution_clock::now();
    algos[i]->Sort(arrCopy);
    auto t1 = std::chrono::high_resolution_clock::now();

    SortCompareResult r;
    r.name        = names[i];
    r.timeMs      = std::chrono::duration<double, std::milli>(t1 - t0).count();
    r.totalOps    = m_Visualizer->GetTotalSteps();
    r.comparisons = m_Visualizer->GetStepCountOfType(StepType::Compare);
    r.writes      = m_Visualizer->GetStepCountOfType(StepType::Swap)
                  + m_Visualizer->GetStepCountOfType(StepType::Overwrite);
    m_CompareResults.push_back(r);

    delete algos[i];
  }

  m_Visualizer->Reset();
  m_Visualizer->ClearSteps();
  m_ShowComparison = true;
}

void Menu::RenderComparisonTable() {
  if (m_CompareResults.empty())
    return;

  ImVec2 origin = ImGui::GetCursorScreenPos();
  ImVec2 avail  = ImGui::GetContentRegionAvail();
  ImDrawList *dl = ImGui::GetWindowDrawList();

  dl->AddRectFilled(origin, ImVec2(origin.x + avail.x, origin.y + avail.y),
                    IM_COL32(16, 18, 24, 230));

  const float pad = 24.0f;

  ImGui::Dummy(ImVec2(0, 18));
  ImGui::SetCursorPosX(pad);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.65f, 0.78f, 1.0f, 1.0f));
  ImGui::Text(ICON_FA_CHART_BAR "  SIDE-BY-SIDE COMPARISON");
  ImGui::PopStyleColor();

  ImGui::SameLine(avail.x - 100.0f);
  ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.22f, 0.25f, 0.35f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.32f, 0.36f, 0.50f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(0.14f, 0.16f, 0.24f, 1.0f));
  if (ImGui::Button(ICON_FA_TIMES "  Close", ImVec2(84.0f, 26.0f)))
    m_ShowComparison = false;
  ImGui::PopStyleColor(3);

  ImGui::SetCursorPosX(pad);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.45f, 0.48f, 0.58f, 1.0f));
  ImGui::Text("Array size: %d  —  same dataset for all three algorithms",
              static_cast<int>(m_Visualizer->GetOriginalArray().size()));
  ImGui::PopStyleColor();

  ImGui::Dummy(ImVec2(0, 14));
  ImGui::SetCursorPosX(pad);

  int   maxCmp = 1, maxWrt = 1, maxOps = 1;
  double maxTime = 1e-6;
  for (const auto &r : m_CompareResults) {
    maxCmp  = std::max(maxCmp,  r.comparisons);
    maxWrt  = std::max(maxWrt,  r.writes);
    maxOps  = std::max(maxOps,  r.totalOps);
    maxTime = std::max(maxTime, r.timeMs);
  }

  int bestCmp = 0, bestWrt = 0, bestOps = 0, bestTime = 0;
  for (int i = 1; i < static_cast<int>(m_CompareResults.size()); ++i) {
    if (m_CompareResults[i].comparisons < m_CompareResults[bestCmp].comparisons)  bestCmp  = i;
    if (m_CompareResults[i].writes      < m_CompareResults[bestWrt].writes)       bestWrt  = i;
    if (m_CompareResults[i].totalOps    < m_CompareResults[bestOps].totalOps)     bestOps  = i;
    if (m_CompareResults[i].timeMs      < m_CompareResults[bestTime].timeMs)      bestTime = i;
  }

  const float tableW = avail.x - pad * 2.0f;
  const float rowH   = 56.0f;

  ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ImVec4(0.22f, 0.28f, 0.42f, 0.9f));
  ImGui::PushStyleColor(ImGuiCol_TableBorderLight,  ImVec4(0.16f, 0.20f, 0.32f, 0.6f));
  ImGui::PushStyleColor(ImGuiCol_TableHeaderBg,     ImVec4(0.10f, 0.13f, 0.22f, 1.0f));
  ImGui::PushStyleColor(ImGuiCol_TableRowBg,        ImVec4(0.09f, 0.11f, 0.18f, 0.5f));
  ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt,     ImVec4(0.12f, 0.15f, 0.24f, 0.5f));

  if (ImGui::BeginTable("##cmp_table", 5,
                        ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
                        ImGuiTableFlags_SizingFixedFit,
                        ImVec2(tableW, 0.0f))) {

    ImGui::TableSetupColumn("Algorithm",   ImGuiTableColumnFlags_WidthFixed,   160.0f);
    ImGui::TableSetupColumn("Comparisons", ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableSetupColumn("Writes",      ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableSetupColumn("Total Ops",   ImGuiTableColumnFlags_WidthStretch, 1.0f);
    ImGui::TableSetupColumn("Time (ms)",   ImGuiTableColumnFlags_WidthStretch, 0.8f);
    ImGui::TableHeadersRow();

    auto MetricCell = [&](int col, int value, int maxVal, bool isBest) {
      ImGui::TableSetColumnIndex(col);
      ImVec2 cellMin = ImGui::GetCursorScreenPos();
      float  cellW   = ImGui::GetContentRegionAvail().x;

      float barW = (maxVal > 0) ? ((float)value / (float)maxVal) * (cellW - 10.0f) : 0.0f;
      ImU32 barCol = isBest ? IM_COL32(35, 160, 85, 110) : IM_COL32(50, 90, 160, 80);
      dl->AddRectFilled(ImVec2(cellMin.x + 5.0f, cellMin.y + 8.0f),
                        ImVec2(cellMin.x + 5.0f + barW, cellMin.y + rowH - 8.0f),
                        barCol, 4.0f);

      ImGui::Dummy(ImVec2(0.0f, (rowH - ImGui::GetTextLineHeight()) * 0.5f - 2.0f));
      if (isBest)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35f, 1.0f, 0.55f, 1.0f));
      else
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.90f, 1.0f));
      ImGui::Text("  %d", value);
      ImGui::PopStyleColor();
    };

    for (int i = 0; i < static_cast<int>(m_CompareResults.size()); ++i) {
      const auto &r = m_CompareResults[i];
      ImGui::TableNextRow(0, rowH);

      ImGui::TableSetColumnIndex(0);
      ImGui::Dummy(ImVec2(0.0f, (rowH - ImGui::GetTextLineHeight()) * 0.5f - 2.0f));
      ImGui::Text("  %s", r.name.c_str());

      MetricCell(1, r.comparisons, maxCmp,  i == bestCmp);
      MetricCell(2, r.writes,      maxWrt,  i == bestWrt);
      MetricCell(3, r.totalOps,    maxOps,  i == bestOps);

      ImGui::TableSetColumnIndex(4);
      ImVec2 cellMin = ImGui::GetCursorScreenPos();
      float  cellW   = ImGui::GetContentRegionAvail().x;
      float  barW    = (float)(r.timeMs / maxTime) * (cellW - 10.0f);
      bool   tBest   = (i == bestTime);
      ImU32  barCol  = tBest ? IM_COL32(35, 160, 85, 110) : IM_COL32(50, 90, 160, 80);
      dl->AddRectFilled(ImVec2(cellMin.x + 5.0f, cellMin.y + 8.0f),
                        ImVec2(cellMin.x + 5.0f + barW, cellMin.y + rowH - 8.0f),
                        barCol, 4.0f);
      ImGui::Dummy(ImVec2(0.0f, (rowH - ImGui::GetTextLineHeight()) * 0.5f - 2.0f));
      if (tBest)
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35f, 1.0f, 0.55f, 1.0f));
      else
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.80f, 0.83f, 0.90f, 1.0f));
      ImGui::Text("  %.4f", r.timeMs);
      ImGui::PopStyleColor();
    }

    ImGui::EndTable();
  }

  ImGui::PopStyleColor(5);

  ImGui::Dummy(ImVec2(0, 12));
  ImGui::SetCursorPosX(pad);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35f, 1.0f, 0.55f, 0.85f));
  ImGui::Text(ICON_FA_CHECK_CIRCLE "  Green = best in that column");
  ImGui::PopStyleColor();
  ImGui::SetCursorPosX(pad);
  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.42f, 0.46f, 0.58f, 0.8f));
  ImGui::Text("Writes = swaps + overwrites (insertion sort shifts instead of swapping)");
  ImGui::PopStyleColor();
}

void Menu::GenerateGraphSimulation() {
  if (!m_Visualizer) return;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> weightDis(1, 20);
  std::uniform_int_distribution<> chanceDis(0, 99);

  int n = m_GraphVertices;

  int cols = (int)ceilf(sqrtf((float)n));

  std::vector<Edge> edges;

  auto addEdge = [&](int u, int v) {
    for (const auto &e : edges)
      if ((e.src == u && e.dest == v) || (e.src == v && e.dest == u))
        return;
    edges.push_back({u, v, weightDis(gen)});
  };

  for (int i = 0; i < n; i++) {
    int col = i % cols;
    if (col + 1 < cols && i + 1 < n)
      addEdge(i, i + 1);
  }

  for (int i = 0; i < n; i++) {
    if (i + cols < n)
      addEdge(i, i + cols);
  }

  for (int i = 0; i < n; i++) {
    int col = i % cols;

    if (col + 1 < cols && i + cols + 1 < n && chanceDis(gen) < 60)
      addEdge(i, i + cols + 1);

    if (col - 1 >= 0 && i + cols - 1 < n && chanceDis(gen) < 60)
      addEdge(i, i + cols - 1);
  }

  if (m_SelectedGraphAlgorithm == 1) {
    PrimGraph pg;
    pg.SetGraphInfo(n, edges);
    pg.SimulatePrims();
    m_Visualizer->SetGraphSimulation(n, edges, pg.GetSteps());
  } else {
    KruskalGraph kg;
    kg.SetGraphInfo(n, edges);
    kg.SimulateKruskals();
    m_Visualizer->SetGraphSimulation(n, edges, kg.GetSteps());
  }

  m_GraphHasRun = true;
}

void Menu::RunGraphSimulation() {
  if (!m_Visualizer)
    return;

  if (!m_Visualizer->HasGraphSimulation())
    GenerateGraphSimulation();

  m_Visualizer->PlayGraphSimulation();
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

  int &currentRecursionN = m_RecursionNBySimulation[m_SelectedRecursionSimulation];
  currentRecursionN = std::clamp(currentRecursionN, 1, GetRecursionMaxForSimulation(m_SelectedRecursionSimulation));
  m_Visualizer->Reset();
  m_Visualizer->ClearSteps();

  RecursionAlgorithm *sim = nullptr;

  switch (m_SelectedRecursionSimulation) {
  case 0: {
    sim = new FactorialAlgorithm(m_Visualizer);
    m_Visualizer->SetVisualizationMode(VisualizationMode::FactorialLadder);
    sim->Factorial(currentRecursionN);
    m_Visualizer->ClearTowerOfHanoiSimulation();
    m_Visualizer->ClearFibonacciSimulation();
    break;
  }
  case 1: {
    Recursion recursion;
    std::vector<int> sequence;
    sequence.reserve(static_cast<size_t>(currentRecursionN));
    for (int i = 1; i <= currentRecursionN; ++i)
      sequence.push_back(recursion.Fibonacci(i));
    m_Visualizer->SetFibonacciSimulation(sequence);
    m_Visualizer->ClearTowerOfHanoiSimulation();
    m_Visualizer->SetVisualizationMode(VisualizationMode::BarGraph);
    break;
  }
  case 2: {
    Recursion recursion;
    recursion.TowerOfHanoi(currentRecursionN, 'A', 'B', 'C');
    m_Visualizer->SetTowerOfHanoiSimulation(recursion.GetTowerMoves(),
                                            currentRecursionN);
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