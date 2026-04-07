#include "Visualizer.h"
#include "../Resources/FontAwesome.h"
#include <algorithm>
#include <cmath>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float Visualizer::EaseOutCubic(float t) {
  t = 1.0f - t;
  return 1.0f - t * t * t;
}

float Visualizer::EaseInOutQuad(float t) {
  return t < 0.5f ? 2.0f * t * t : 1.0f - powf(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

void Visualizer::LerpColor(float &r, float &g, float &b, float &a, float tr,
                           float tg, float tb, float ta, float t) {
  r += (tr - r) * t;
  g += (tg - g) * t;
  b += (tb - b) * t;
  a += (ta - a) * t;
}

ImVec4 Visualizer::GetThemeColorVec(StepType type) const {
  auto c = [](int r, int g, int b, int a = 255) -> ImVec4 {
    return ImVec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
  };

  switch (type) {
  case StepType::Compare:
    return c(255, 215, 0);
  case StepType::Swap:
  case StepType::Overwrite:
    return c(255, 60, 100);
  case StepType::Sorted:
    return c(0, 255, 170);
  case StepType::Pivot:
    return c(190, 80, 255);
  default:
    return c(80, 160, 255);
  }
}

ImU32 Visualizer::GetThemeColor(StepType type) const {
  ImVec4 v = GetThemeColorVec(type);
  return IM_COL32((int)(v.x * 255), (int)(v.y * 255), (int)(v.z * 255),
                  (int)(v.w * 255));
}

ImU32 Visualizer::GetElementColor(int idx) const {
  if (idx < 0 || idx >= static_cast<int>(m_Elements.size())) {
    return GetThemeColor(StepType::Default);
  }
  const auto &e = m_Elements[idx];
  return IM_COL32((int)(e.currentR * 255), (int)(e.currentG * 255),
                  (int)(e.currentB * 255), (int)(e.currentA * 255));
}

Visualizer::Visualizer() { GenerateRandomArray(m_Config.arraySize); }

void Visualizer::GenerateRandomArray(int size) {
  m_OriginalArray.clear();
  m_OriginalArray.reserve(size);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(5, 100);

  for (int i = 0; i < size; ++i)
    m_OriginalArray.push_back(dis(gen));

  m_Config.arraySize = size;
  ClearSteps();
  SyncElementStates(m_OriginalArray);
}

void Visualizer::SetArray(const std::vector<int> &arr) {
  m_OriginalArray = arr;
  ClearSteps();
  SyncElementStates(m_OriginalArray);
}

void Visualizer::AddStep(const AlgorithmStep &step) { m_Steps.push_back(step); }

void Visualizer::ClearSteps() {
  m_Steps.clear();
  m_CurrentStepIndex = -1;
  m_IsPlaying = false;
  m_AnimationProgress = 1.0f;
}

const std::vector<int> &Visualizer::GetCurrentArray() const {
  if (m_CurrentStepIndex >= 0 &&
      m_CurrentStepIndex < static_cast<int>(m_Steps.size()))
    return m_Steps[m_CurrentStepIndex].array;
  return m_OriginalArray;
}

const std::vector<int> &Visualizer::GetOriginalArray() const {
  return m_OriginalArray;
}

void Visualizer::Play() {
  if (!m_Steps.empty()) {
    m_IsPlaying = true;
    m_PlayTimer = 0.0f;
  }
}

void Visualizer::Pause() { m_IsPlaying = false; }

void Visualizer::StepForward() {
  if (m_CurrentStepIndex < static_cast<int>(m_Steps.size()) - 1) {
    ++m_CurrentStepIndex;
    m_AnimationProgress = 0.0f;
    SyncElementStates(m_Steps[m_CurrentStepIndex].array);
  } else {
    m_IsPlaying = false;
  }
}

void Visualizer::StepBackward() {
  if (m_CurrentStepIndex > 0) {
    --m_CurrentStepIndex;
    m_AnimationProgress = 0.0f;
    SyncElementStates(m_Steps[m_CurrentStepIndex].array);
  }
}

void Visualizer::Reset() {
  m_Steps.clear();
  m_CurrentStepIndex = -1;
  m_IsPlaying = false;
  m_AnimationProgress = 1.0f;
  SyncElementStates(m_OriginalArray);
}

void Visualizer::SetTowerOfHanoiSimulation(
    const std::vector<std::pair<char, char>> &moves, int diskCount) {
  m_TowerMoves = moves;
  m_TowerDiskCount = std::max(1, diskCount);
  m_TowerStep = 0;
  m_TowerPlayTimer = 0.0f;
  m_TowerPlaying = !m_TowerMoves.empty();
  m_ShowTowerIllustration = true;
}

void Visualizer::ClearTowerOfHanoiSimulation() {
  m_ShowTowerIllustration = false;
  m_TowerPlaying = false;
  m_TowerPlayTimer = 0.0f;
  m_TowerStep = 0;
  m_TowerDiskCount = 4;
  m_TowerMoves.clear();
}

void Visualizer::SetFibonacciSimulation(const std::vector<int> &sequence) {
  m_FibonacciSequence = sequence;
  m_ShowFibonacciIllustration = !m_FibonacciSequence.empty();
  m_FibonacciAnimTime = 0.0f;
  m_FibonacciPlaying = !m_FibonacciSequence.empty();
  m_FibonacciPlayTimer = 0.0f;
  m_FibonacciStepProgress = 0.0f;
  m_FibonacciStep = 0;
  m_FibonacciLastFollowedIndex = -1;
}

void Visualizer::ResetFibonacciSimulation() {
  m_FibonacciAnimTime = 0.0f;
  m_FibonacciPlaying = false;
  m_FibonacciPlayTimer = 0.0f;
  m_FibonacciStepProgress = 0.0f;
  m_FibonacciStep = 0;
  m_FibonacciLastFollowedIndex = -1;
  m_ShowFibonacciIllustration = !m_FibonacciSequence.empty();
}

void Visualizer::ClearFibonacciSimulation() {
  m_ShowFibonacciIllustration = false;
  m_FibonacciSequence.clear();
  m_FibonacciAnimTime = 0.0f;
  m_FibonacciPlaying = false;
  m_FibonacciPlayTimer = 0.0f;
  m_FibonacciStepProgress = 0.0f;
  m_FibonacciStep = 0;
  m_FibonacciLastFollowedIndex = -1;
}

void Visualizer::SetTowerTrace(const std::vector<std::string> &traceLines) {
  m_TowerTrace = traceLines;
  m_ShowTowerTrace = !m_TowerTrace.empty();
}

void Visualizer::ClearTowerTrace() {
  m_TowerTrace.clear();
  m_ShowTowerTrace = false;
}

void Visualizer::UpdateTowerOfHanoi(float dt) {
  if (!m_ShowTowerIllustration || !m_TowerPlaying)
    return;

  int maxStep = static_cast<int>(m_TowerMoves.size());
  if (m_TowerStep >= maxStep) {
    m_TowerPlaying = false;
    m_TowerMoveProgress = 0.0f;
    return;
  }

  float stepInterval = 1.75f;
  float speedScale = 1.0f - (static_cast<float>(m_Config.animationSpeed) * 0.0045f);
  if (speedScale < 0.18f)
    speedScale = 0.18f;
  if (speedScale > 1.0f)
    speedScale = 1.0f;
  stepInterval *= speedScale;

  m_TowerPlayTimer += dt;
  m_TowerMoveProgress = m_TowerPlayTimer / stepInterval;
  if (m_TowerMoveProgress >= 1.0f) {
    m_TowerPlayTimer = 0.0f;
    m_TowerMoveProgress = 0.0f;
    ++m_TowerStep;
    if (m_TowerStep >= maxStep) {
      m_TowerStep = maxStep;
      m_TowerPlaying = false;
    }
  }
}

void Visualizer::RenderTowerOfHanoi() {
  if (!m_ShowTowerIllustration) {
    
    ImGui::Dummy(ImVec2(0.0f, ImGui::GetFrameHeightWithSpacing() + 6.0f));

    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    canvasSize.y -= 28.0f;
    if (canvasSize.y < 80.0f)
      canvasSize.y = 80.0f;
    if (canvasSize.x < 100.0f)
      canvasSize.x = 100.0f;

    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList *drawList = ImGui::GetWindowDrawList();

    float lineH = ImGui::GetTextLineHeightWithSpacing();
    float boxW = canvasSize.x * 0.30f - 24.0f;
    float boxH = 14.0f + (lineH * 7.0f) + 10.0f;
    ImVec2 boxMin(origin.x + canvasSize.x - boxW - 12.0f, origin.y + 12.0f);
    ImVec2 boxMax(boxMin.x + boxW, boxMin.y + boxH);
    drawList->AddRectFilled(boxMin, boxMax, IM_COL32(18, 22, 30, 210), 8.0f);
    drawList->AddRect(boxMin, boxMax, IM_COL32(110, 125, 150, 170), 8.0f, 0,
                      1.5f);
    drawList->AddText(ImVec2(boxMin.x + 6.0f, boxMin.y - lineH - 4.0f),
                      IM_COL32(200, 215, 235, 230), "Calls and Returns");
    drawList->AddText(ImVec2(boxMin.x + 10.0f, boxMin.y + 10.0f),
                      IM_COL32(160, 170, 190, 210), "No trace yet.");

    const char *centerMsg =
      "Click \"Run Simulation\" to view the Tower of Hanoi illustration";
    ImVec2 centerMsgSize = ImGui::CalcTextSize(centerMsg);
    ImVec2 centerMsgPos(origin.x + (canvasSize.x - centerMsgSize.x) * 0.5f,
              origin.y + (canvasSize.y - centerMsgSize.y) * 0.5f);
    drawList->AddText(centerMsgPos, IM_COL32(170, 180, 200, 220), centerMsg);

    ImGui::Dummy(canvasSize);
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f), "Tower of Hanoi Recursion");
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 110);
    ImGui::ProgressBar(0.0f, ImVec2(100, 0), "0%");
    return;
  }

  int maxStep = static_cast<int>(m_TowerMoves.size());

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));

  if (m_TowerPlaying) {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 80, 60, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(220, 100, 80, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(180, 65, 50, 255));
  } else {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 180, 100, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(70, 200, 120, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(40, 160, 90, 255));
  }

  if (ImGui::Button(m_TowerPlaying ? ICON_FA_PAUSE "  Pause"
                                   : ICON_FA_PLAY "  Play")) {
    if (m_TowerStep >= maxStep)
      m_TowerStep = 0;
    m_TowerPlaying = !m_TowerPlaying;
    m_TowerPlayTimer = 0.0f;
    m_TowerMoveProgress = 0.0f;
  }
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_STEP_BACKWARD)) {
    m_TowerPlaying = false;
    m_TowerPlayTimer = 0.0f;
    m_TowerMoveProgress = 0.0f;
    if (m_TowerStep > 0)
      --m_TowerStep;
  }

  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_STEP_FORWARD)) {
    m_TowerPlaying = false;
    m_TowerPlayTimer = 0.0f;
    m_TowerMoveProgress = 0.0f;
    if (m_TowerStep < maxStep)
      ++m_TowerStep;
  }

  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f), "Step %d / %d",
                     m_TowerStep, maxStep);
  ImGui::PopStyleVar(3);

  auto pegIndex = [](char peg) {
    switch (peg) {
    case 'A':
      return 0;
    case 'B':
      return 1;
    default:
      return 2;
    }
  };

  std::vector<int> pegs[3];
  for (int disk = m_TowerDiskCount; disk >= 1; --disk)
    pegs[0].push_back(disk);

  int movingDisk = -1;
  int movingFromPeg = -1;
  int movingToPeg = -1;
  int lastMovedDisk = -1;
  char lastMovedFrom = 'A';
  char lastMovedTo = 'A';

  for (int i = 0; i < m_TowerStep && i < maxStep; ++i) {
    const auto &move = m_TowerMoves[static_cast<size_t>(i)];
    int from = pegIndex(move.first);
    int to = pegIndex(move.second);
    if (pegs[from].empty())
      continue;
    int disk = pegs[from].back();
    pegs[from].pop_back();
    pegs[to].push_back(disk);
    lastMovedDisk = disk;
    lastMovedFrom = move.first;
    lastMovedTo = move.second;
  }

  if (m_TowerStep < maxStep && (m_TowerPlaying || m_TowerMoveProgress > 0.0f)) {
    movingFromPeg = pegIndex(m_TowerMoves[static_cast<size_t>(m_TowerStep)].first);
    movingToPeg = pegIndex(m_TowerMoves[static_cast<size_t>(m_TowerStep)].second);
    if (!pegs[movingFromPeg].empty()) {
      movingDisk = pegs[movingFromPeg].back();
      pegs[movingFromPeg].pop_back();
    }
  }

  ImGui::Dummy(ImVec2(0, 6));

  ImVec2 canvasSize = ImGui::GetContentRegionAvail();
  canvasSize.y -= 28.0f;
  if (canvasSize.y < 80.0f)
    canvasSize.y = 80.0f;
  if (canvasSize.x < 100.0f)
    canvasSize.x = 100.0f;

  ImVec2 origin = ImGui::GetCursorScreenPos();
  ImDrawList *drawList = ImGui::GetWindowDrawList();

  float baseY = origin.y + canvasSize.y - 78.0f;
  float centerY = baseY - 108.0f;
  float pegHeight = 210.0f;
  float pegX[3] = {
      origin.x + canvasSize.x * 0.24f,
      origin.x + canvasSize.x * 0.50f,
      origin.x + canvasSize.x * 0.76f,
  };

  drawList->AddRectFilled(ImVec2(origin.x + 70.0f, baseY),
                          ImVec2(origin.x + canvasSize.x - 70.0f, baseY + 8.0f),
                          IM_COL32(120, 120, 130, 255), 4.0f);

  for (int peg = 0; peg < 3; ++peg) {
    drawList->AddRectFilled(ImVec2(pegX[peg] - 4.0f, centerY - pegHeight / 2),
                            ImVec2(pegX[peg] + 4.0f, centerY + pegHeight / 2),
                            IM_COL32(180, 180, 200, 255), 2.0f);
  }

  float minW = 26.0f;
  float maxW = 112.0f;
  float diskH = 14.0f;
  float denom = (m_TowerDiskCount > 1) ? static_cast<float>(m_TowerDiskCount - 1) : 1.0f;
  ImU32 standbyColor = GetThemeColor(StepType::Default);
  ImU32 chosenColor = GetThemeColor(StepType::Compare);

  for (int peg = 0; peg < 3; ++peg) {
    for (size_t level = 0; level < pegs[peg].size(); ++level) {
      int disk = pegs[peg][level];
      float t = static_cast<float>(disk - 1) / denom;
      float diskW = minW + t * (maxW - minW);
      float yBottom = baseY - 3.0f - static_cast<float>(level) * (diskH + 3.0f);
      float yTop = yBottom - diskH;
      ImU32 color = standbyColor;
      drawList->AddRectFilled(ImVec2(pegX[peg] - diskW * 0.5f, yTop),
                              ImVec2(pegX[peg] + diskW * 0.5f, yBottom),
                              color, 4.0f);
    }
  }

  if (movingDisk > 0 && movingFromPeg >= 0 && movingToPeg >= 0) {
    float diskT = static_cast<float>(movingDisk - 1) / denom;
    float diskW = minW + diskT * (maxW - minW);

    float sourceLevel = static_cast<float>(pegs[movingFromPeg].size());
    float destLevel = static_cast<float>(pegs[movingToPeg].size());

    float sourceBottom = baseY - 3.0f - sourceLevel * (diskH + 3.0f);
    float destBottom = baseY - 3.0f - destLevel * (diskH + 3.0f);
    float sourceTop = sourceBottom - diskH;
    float destTop = destBottom - diskH;

    float moveDuration = 1.75f;
    float speedScale = 1.0f - (static_cast<float>(m_Config.animationSpeed) * 0.0045f);
    if (speedScale < 0.18f)
      speedScale = 0.18f;
    if (speedScale > 1.0f)
      speedScale = 1.0f;
    moveDuration *= speedScale;

    float phase = std::min(1.0f, m_TowerMoveProgress);
    float liftEnd = 0.18f;
    float holdEnd = liftEnd + (0.25f / moveDuration);
    if (holdEnd > 0.85f)
      holdEnd = 0.85f;
    float slideEnd = holdEnd + 0.20f;
    if (slideEnd > 0.95f)
      slideEnd = 0.95f;
    float peakY = centerY - pegHeight * 0.55f - 20.0f;
    float x = pegX[movingFromPeg];
    float y = sourceTop;

    if (phase < liftEnd) {
      float t = EaseOutCubic(phase / liftEnd);
      y = sourceTop + (peakY - sourceTop) * t;
    } else if (phase < holdEnd) {
      y = peakY;
      x = pegX[movingFromPeg];
    } else if (phase < slideEnd) {
      float t = EaseInOutQuad((phase - holdEnd) / (slideEnd - holdEnd));
      x = pegX[movingFromPeg] + (pegX[movingToPeg] - pegX[movingFromPeg]) * t;
      y = peakY;
    } else {
      float t = EaseInOutQuad((phase - slideEnd) / (1.0f - slideEnd));
      x = pegX[movingToPeg];
      y = peakY + (destTop - peakY) * t;
    }

    drawList->AddRectFilled(ImVec2(x - diskW * 0.5f, y),
                            ImVec2(x + diskW * 0.5f, y + diskH),
                            chosenColor, 4.0f);
  }

  drawList->AddText(ImVec2(pegX[0] - 4.0f, baseY + 14.0f), IM_COL32_WHITE, "A");
  drawList->AddText(ImVec2(pegX[1] - 4.0f, baseY + 14.0f), IM_COL32_WHITE, "B");
  drawList->AddText(ImVec2(pegX[2] - 4.0f, baseY + 14.0f), IM_COL32_WHITE, "C");

  
  float lineH = ImGui::GetTextLineHeightWithSpacing();
  float boxW = canvasSize.x * 0.30f - 24.0f;
  float boxH = 14.0f + (lineH * 7.0f) + 10.0f;
  ImVec2 boxMin(origin.x + canvasSize.x - boxW - 12.0f, origin.y + 12.0f);
  ImVec2 boxMax(boxMin.x + boxW, boxMin.y + boxH);
  drawList->AddRectFilled(boxMin, boxMax, IM_COL32(18, 22, 30, 210), 8.0f);
  drawList->AddRect(boxMin, boxMax, IM_COL32(110, 125, 150, 170), 8.0f, 0, 1.5f);
  drawList->AddText(ImVec2(boxMin.x + 6.0f, boxMin.y - lineH - 4.0f),
                    IM_COL32(200, 215, 235, 230), "Calls and Returns");

  ImVec2 scrollMin(boxMin.x + 8.0f, boxMin.y + 4.0f);
  ImVec2 scrollSize(boxW - 16.0f, boxMax.y - scrollMin.y - 8.0f);
  ImGui::SetCursorScreenPos(scrollMin);
  ImGui::PushID("tower_trace_scroll");
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
  ImGui::BeginChild("content", scrollSize, false,
                    ImGuiWindowFlags_AlwaysVerticalScrollbar |
                        ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoNav);

  const float contentPadTopY = 1.5f;
  const float contentPadBottomY = 2.0f;
  ImGui::Dummy(ImVec2(0.0f, contentPadTopY));

  if (m_TowerTrace.empty()) {
    ImGui::TextColored(ImVec4(0.62f, 0.66f, 0.74f, 0.85f), "No trace yet.");
  } else {
    int revealedMoveCount = m_TowerStep;
    if (m_TowerPlaying && m_TowerMoveProgress > 0.0f)
      revealedMoveCount = std::min(maxStep, m_TowerStep + 1);

    size_t visibleTraceCount = 0;
    int moveSeen = 0;
    for (size_t i = 0; i < m_TowerTrace.size(); ++i) {
      const bool isMove = (m_TowerTrace[i].rfind("Move:", 0) == 0);
      if (isMove && moveSeen >= revealedMoveCount)
        break;
      visibleTraceCount = i + 1;
      if (isMove)
        ++moveSeen;
    }

    if (visibleTraceCount == 0)
      visibleTraceCount = std::min<size_t>(1, m_TowerTrace.size());

    auto getTraceKind = [](const std::string &line) {
      if (line.rfind("Call:", 0) == 0)
        return 0;
      if (line.rfind("Return:", 0) == 0)
        return 1;
      if (line.rfind("Base", 0) == 0)
        return 2;
      if (line.rfind("Move:", 0) == 0)
        return 3;
      return 4;
    };

    for (size_t i = 0; i < visibleTraceCount; ++i) {
      ImVec2 linePos = ImGui::GetCursorScreenPos();

      std::string displayText = m_TowerTrace[i];
      const float maxTextWidth = scrollSize.x - 24.0f;
      if (ImGui::CalcTextSize(displayText.c_str()).x > maxTextWidth) {
        const char *ellipsis = "...";
        while (!displayText.empty()) {
          std::string candidate = displayText + ellipsis;
          if (ImGui::CalcTextSize(candidate.c_str()).x <= maxTextWidth) {
            displayText = candidate;
            break;
          }
          displayText.pop_back();
        }
        if (displayText.empty())
          displayText = ellipsis;
      }

      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.72f, 0.76f, 0.84f, 0.95f));
      ImGui::SetCursorScreenPos(ImVec2(linePos.x + 4.0f, linePos.y));
      ImGui::TextUnformatted(displayText.c_str());
      ImGui::PopStyleColor();

      
      
      if (getTraceKind(m_TowerTrace[i]) == 1) {
        size_t nextIndex = i + 1;
        while (nextIndex < m_TowerTrace.size() && m_TowerTrace[nextIndex].empty())
          ++nextIndex;

        if (nextIndex < m_TowerTrace.size()) {
          int nextKind = getTraceKind(m_TowerTrace[nextIndex]);
          if (nextKind != 0 && nextKind != 1)
            ImGui::Dummy(ImVec2(0.0f, lineH * 0.45f));
        }
      }

      if (m_TowerPlaying && i + 1 == visibleTraceCount)
        ImGui::SetScrollHereY(1.0f);
    }
  }

  ImGui::Dummy(ImVec2(0.0f, contentPadBottomY));
  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::PopID();

  ImGui::SetCursorScreenPos(origin);

  ImGui::Dummy(canvasSize);

  char moveText[96];
  if (movingDisk > 0 && movingFromPeg >= 0 && movingToPeg >= 0) {
    snprintf(moveText, sizeof(moveText), "Move disk %d from %c to %c",
             movingDisk, m_TowerMoves[static_cast<size_t>(m_TowerStep)].first,
             m_TowerMoves[static_cast<size_t>(m_TowerStep)].second);
  } else if (lastMovedDisk > 0) {
    snprintf(moveText, sizeof(moveText), "Move disk %d from %c to %c",
             lastMovedDisk, lastMovedFrom, lastMovedTo);
  } else {
    snprintf(moveText, sizeof(moveText), "Ready - run an algorithm, then press Play");
  }

  ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.6f, 1.0f), "%s", moveText);
  ImGui::SameLine(ImGui::GetContentRegionAvail().x - 110);
  float progress =
      maxStep > 0 ? static_cast<float>(m_TowerStep) / static_cast<float>(maxStep)
                  : 0.0f;
  ImGui::ProgressBar(progress, ImVec2(100, 0));
}

void Visualizer::RenderFibonacciGoldenRatio() {
  if (!m_ShowFibonacciIllustration) {
    
    ImGui::Dummy(ImVec2(0.0f, ImGui::GetFrameHeightWithSpacing() + 6.0f));

    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    canvasSize.y -= 28.0f;
    if (canvasSize.y < 80.0f)
      canvasSize.y = 80.0f;
    if (canvasSize.x < 100.0f)
      canvasSize.x = 100.0f;

    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList *drawList = ImGui::GetWindowDrawList();

    if (m_Config.showGrid) {
      constexpr int gridLines = 5;
      for (int i = 0; i <= gridLines; ++i) {
        float y = origin.y + (canvasSize.y / gridLines) * i;
        drawList->AddLine(ImVec2(origin.x, y),
                          ImVec2(origin.x + canvasSize.x, y),
                          IM_COL32(255, 255, 255, 16), 1.0f);
      }
    }

    const char *centerMsg =
        "Click \"Run Simulation\" to view the Fibonacci illustration";
    ImVec2 centerMsgSize = ImGui::CalcTextSize(centerMsg);
    ImVec2 centerMsgPos(origin.x + (canvasSize.x - centerMsgSize.x) * 0.5f,
                        origin.y + (canvasSize.y - centerMsgSize.y) * 0.5f);
    drawList->AddText(centerMsgPos, IM_COL32(170, 180, 200, 220), centerMsg);

    float lineH = ImGui::GetTextLineHeightWithSpacing();
    float boxW = canvasSize.x * 0.30f - 24.0f;
    float boxH = 14.0f + (lineH * 7.0f) + 10.0f;
    ImVec2 boxMin(origin.x + canvasSize.x - boxW - 12.0f, origin.y + 12.0f);
    ImVec2 boxMax(boxMin.x + boxW, boxMin.y + boxH);
    drawList->AddRectFilled(boxMin, boxMax, IM_COL32(18, 22, 30, 210), 8.0f);
    drawList->AddRect(boxMin, boxMax, IM_COL32(110, 125, 150, 170), 8.0f, 0,
              1.5f);
    drawList->AddText(ImVec2(boxMin.x + 6.0f, boxMin.y - lineH - 4.0f),
              IM_COL32(200, 215, 235, 230), "Calls and Returns");
    drawList->AddText(ImVec2(boxMin.x + 10.0f, boxMin.y + 10.0f),
              IM_COL32(160, 170, 190, 210), "No trace yet.");

    ImGui::Dummy(canvasSize);
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f),
                       "Fibonacci Recursion");
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 110);
    ImGui::ProgressBar(0.0f, ImVec2(100, 0), "0%");
    return;
  }

  float dt = ImGui::GetIO().DeltaTime;
  m_FibonacciAnimTime += dt;

  const int n = static_cast<int>(m_FibonacciSequence.size());
  if (n < 2) {
    
    ImGui::Dummy(ImVec2(0.0f, ImGui::GetFrameHeightWithSpacing() + 6.0f));

    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    canvasSize.y -= 28.0f;
    if (canvasSize.y < 80.0f)
      canvasSize.y = 80.0f;
    if (canvasSize.x < 100.0f)
      canvasSize.x = 100.0f;

    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList *drawList = ImGui::GetWindowDrawList();
    if (m_Config.showGrid) {
      constexpr int gridLines = 5;
      for (int i = 0; i <= gridLines; ++i) {
        float y = origin.y + (canvasSize.y / gridLines) * i;
        drawList->AddLine(ImVec2(origin.x, y),
                          ImVec2(origin.x + canvasSize.x, y),
                          IM_COL32(255, 255, 255, 16), 1.0f);
      }
    }

    float lineH = ImGui::GetTextLineHeightWithSpacing();
    float boxW = canvasSize.x * 0.30f - 24.0f;
    float boxH = 14.0f + (lineH * 7.0f) + 10.0f;
    ImVec2 boxMin(origin.x + canvasSize.x - boxW - 12.0f, origin.y + 12.0f);
    ImVec2 boxMax(boxMin.x + boxW, boxMin.y + boxH);
    drawList->AddRectFilled(boxMin, boxMax, IM_COL32(18, 22, 30, 210), 8.0f);
    drawList->AddRect(boxMin, boxMax, IM_COL32(110, 125, 150, 170), 8.0f, 0,
                      1.5f);
    drawList->AddText(ImVec2(boxMin.x + 6.0f, boxMin.y - lineH - 4.0f),
                      IM_COL32(200, 215, 235, 230), "Calls and Returns");
    drawList->AddText(ImVec2(boxMin.x + 10.0f, boxMin.y + 10.0f),
                      IM_COL32(160, 170, 190, 210), "No trace yet.");

    ImGui::Dummy(canvasSize);
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.75f, 1.0f),
                       "Run Fibonacci simulation to view golden ratio illustration.");
    ImGui::SameLine(ImGui::GetContentRegionAvail().x - 110);
    ImGui::ProgressBar(0.0f, ImVec2(100, 0), "0%");
    return;
  }

  int maxStep = n - 1;
  if (maxStep < 1)
    maxStep = 1;

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));

  if (m_FibonacciPlaying) {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 80, 60, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(220, 100, 80, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(180, 65, 50, 255));
  } else {
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 180, 100, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(70, 200, 120, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(40, 160, 90, 255));
  }

  if (ImGui::Button(m_FibonacciPlaying ? ICON_FA_PAUSE "  Pause"
                                       : ICON_FA_PLAY "  Play")) {
    if (m_FibonacciStep >= maxStep)
      m_FibonacciStep = 0;
    m_FibonacciPlaying = !m_FibonacciPlaying;
    m_FibonacciPlayTimer = 0.0f;
    m_FibonacciStepProgress = 0.0f;
  }
  ImGui::PopStyleColor(3);

  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_STEP_BACKWARD)) {
    m_FibonacciPlaying = false;
    m_FibonacciPlayTimer = 0.0f;
    m_FibonacciStepProgress = 0.0f;
    if (m_FibonacciStep > 0)
      --m_FibonacciStep;
  }

  ImGui::SameLine();
  if (ImGui::Button(ICON_FA_STEP_FORWARD)) {
    m_FibonacciPlaying = false;
    m_FibonacciPlayTimer = 0.0f;
    m_FibonacciStepProgress = 0.0f;
    if (m_FibonacciStep < maxStep)
      ++m_FibonacciStep;
  }

  ImGui::SameLine();
  ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f), "Step %d / %d",
                     m_FibonacciStep, maxStep);
  ImGui::PopStyleVar(3);

  if (m_FibonacciPlaying) {
    float stepInterval = 1.20f;
    float speedScale = 1.0f - (static_cast<float>(m_Config.animationSpeed) * 0.0045f);
    if (speedScale < 0.18f)
      speedScale = 0.18f;
    if (speedScale > 1.0f)
      speedScale = 1.0f;
    stepInterval *= speedScale;

    m_FibonacciPlayTimer += dt;
    m_FibonacciStepProgress = m_FibonacciPlayTimer / stepInterval;
    if (m_FibonacciStepProgress >= 1.0f) {
      m_FibonacciPlayTimer = 0.0f;
      m_FibonacciStepProgress = 0.0f;
      ++m_FibonacciStep;
      if (m_FibonacciStep >= maxStep) {
        m_FibonacciStep = maxStep;
        m_FibonacciPlaying = false;
      }
    }
  }

  const float phi = 1.61803398875f;
  float effectiveStep = static_cast<float>(m_FibonacciStep);
  if (m_FibonacciPlaying)
    effectiveStep += std::min(1.0f, m_FibonacciStepProgress);
  if (effectiveStep > static_cast<float>(maxStep))
    effectiveStep = static_cast<float>(maxStep);

  int loStep = static_cast<int>(floorf(effectiveStep));

  float reveal = static_cast<float>(maxStep > 0 ? effectiveStep / maxStep : 0.0f);
  if (reveal < 0.0f)
    reveal = 0.0f;
  if (reveal > 1.0f)
    reveal = 1.0f;

  ImVec2 canvasSize = ImGui::GetContentRegionAvail();
  canvasSize.y -= 28.0f;
  if (canvasSize.y < 80.0f)
    canvasSize.y = 80.0f;
  if (canvasSize.x < 100.0f)
    canvasSize.x = 100.0f;

  ImVec2 origin = ImGui::GetCursorScreenPos();
  ImDrawList *drawList = ImGui::GetWindowDrawList();

  if (m_Config.showGrid) {
    constexpr int gridLines = 5;
    for (int i = 0; i <= gridLines; ++i) {
      float y = origin.y + (canvasSize.y / gridLines) * i;
      drawList->AddLine(ImVec2(origin.x, y),
                        ImVec2(origin.x + canvasSize.x, y),
                        IM_COL32(255, 255, 255, 16), 1.0f);
    }
  }

  const float sidePanelReserve = canvasSize.x * 0.32f;
  const float spiralAreaWidth = std::max(120.0f, canvasSize.x - sidePanelReserve);
  ImVec2 center(origin.x + spiralAreaWidth * 0.5f, origin.y + canvasSize.y * 0.52f);
  ImU32 spiralColor = GetThemeColor(StepType::Compare);

  float thetaMax = static_cast<float>(n + 2) * (float)M_PI * 0.5f;
  float rMax = powf(phi, thetaMax / ((float)M_PI * 0.5f));
  float a = (std::min(spiralAreaWidth, canvasSize.y) * 0.42f) /
            std::max(1.0f, rMax);

  float visibleTheta = thetaMax * reveal;
  float visibleR = a * powf(phi, visibleTheta / ((float)M_PI * 0.5f));
  float targetViewportRadius = std::min(spiralAreaWidth, canvasSize.y) * 0.40f;
  float minVisibleR = std::max(1.0f, a * 0.16f);
  float zoomScale = targetViewportRadius / std::max(minVisibleR, visibleR);
  if (zoomScale < 1.0f)
    zoomScale = 1.0f;
  if (zoomScale > 14.0f)
    zoomScale = 14.0f;

  auto SpiralPoint = [&](float theta) {
    float r = a * powf(phi, theta / ((float)M_PI * 0.5f));
    float localX = r * cosf(theta) * zoomScale;
    float localY = r * sinf(theta) * zoomScale;
    return ImVec2(center.x + localX, center.y + localY);
  };

  const int samples = 720;
  int visibleSamples = std::max(32, static_cast<int>(samples * reveal));
  ImVec2 prev = center;
  for (int i = 0; i <= visibleSamples; ++i) {
    float t = static_cast<float>(i) / static_cast<float>(samples);
    float theta = thetaMax * t;
    ImVec2 p = SpiralPoint(theta);
    if (i > 0)
      drawList->AddLine(prev, p, spiralColor, 2.0f);
    prev = p;
  }

  float tracerT = reveal;
  if (m_FibonacciPlaying && reveal >= 1.0f)
    tracerT = fmodf(m_FibonacciAnimTime * 0.12f, 1.0f);
  float tracerTheta = thetaMax * tracerT;
  ImVec2 tracer = SpiralPoint(tracerTheta);
  float pulse = 0.8f + 0.2f * sinf(m_FibonacciAnimTime * 5.0f);
  drawList->AddCircleFilled(tracer, 6.0f * pulse, GetThemeColor(StepType::Swap));
  drawList->AddCircle(tracer, 10.0f * pulse, GetThemeColor(StepType::Compare), 0,
                      2.0f);

  std::vector<std::string> exprLines;
  exprLines.reserve(static_cast<size_t>(n) * 2 + 2);

  auto fibValue = [&](int k) {
    if (k <= 0)
      return 0;
    if (k == 1)
      return 1;
    if (k - 1 < static_cast<int>(m_FibonacciSequence.size()))
      return m_FibonacciSequence[static_cast<size_t>(k - 1)];

    int a = 0;
    int b = 1;
    for (int i = 2; i <= k; ++i) {
      int c = a + b;
      a = b;
      b = c;
    }
    return b;
  };

  exprLines.push_back("Base Case: 0");
  exprLines.push_back("Base Case: 1");

  int currentNum = std::max(1, std::min(n, m_FibonacciStep + 1));
  for (int k = 2; k <= currentNum; ++k) {
    exprLines.push_back("Call: F(" + std::to_string(k) + ") = F(" +
                        std::to_string(k - 1) + ") + F(" +
                        std::to_string(k - 2) + ")");

    exprLines.push_back("Return: F(" + std::to_string(k) + ") = " +
                        std::to_string(fibValue(k)));

    if (k < currentNum)
      exprLines.push_back("");
  }

  int visibleExprCount = static_cast<int>(exprLines.size());
  int activeExprIndex = std::max(0, visibleExprCount - 1);
  for (int i = visibleExprCount - 1; i >= 0; --i) {
    if (!exprLines[static_cast<size_t>(i)].empty()) {
      activeExprIndex = i;
      break;
    }
  }
  bool shouldFollowLatest = (activeExprIndex != m_FibonacciLastFollowedIndex);

  float boxW = canvasSize.x * 0.30f - 24.0f;
  float lineH = ImGui::GetTextLineHeightWithSpacing();
  float boxH = 14.0f + (lineH * 7.0f) + 10.0f;
  ImVec2 boxMin(origin.x + canvasSize.x - boxW - 12.0f, origin.y + 12.0f);
  ImVec2 boxMax(boxMin.x + boxW, boxMin.y + boxH);
  drawList->AddRectFilled(boxMin, boxMax, IM_COL32(18, 22, 30, 210), 8.0f);
  drawList->AddRect(boxMin, boxMax, IM_COL32(110, 125, 150, 170), 8.0f, 0, 1.5f);
  drawList->AddText(ImVec2(boxMin.x + 6.0f, boxMin.y - lineH - 4.0f),
                    IM_COL32(200, 215, 235, 230), "Calls and Returns");

  ImVec2 scrollMin(boxMin.x + 8.0f, boxMin.y + 4.0f);
  ImVec2 scrollSize(boxW - 16.0f, boxMax.y - scrollMin.y - 8.0f);
  ImGui::SetCursorScreenPos(scrollMin);
  ImGui::PushID("fib_expr_scroll");
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
  ImGui::BeginChild("content", scrollSize, false,
                    ImGuiWindowFlags_AlwaysVerticalScrollbar |
                        ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoNav);

  const float contentPadTopY = 1.5f;
  const float contentPadBottomY = 2.0f;
  ImGui::Dummy(ImVec2(0.0f, contentPadTopY));

  for (int i = 0; i < visibleExprCount; ++i) {
    float lineReveal = 1.0f;

    ImVec2 linePos = ImGui::GetCursorScreenPos();
    if (i == activeExprIndex && lineReveal > 0.0f) {
      ImGui::GetWindowDrawList()->AddRectFilled(
          ImVec2(linePos.x + 1.0f, linePos.y - 1.0f),
          ImVec2(linePos.x + scrollSize.x - 16.0f, linePos.y + lineH - 2.0f),
          IM_COL32(95, 140, 220, static_cast<int>(70.0f * lineReveal)), 5.0f);
    }

    ImU32 textColor = i < 2 ? IM_COL32(235, 240, 245, 245)
                            : IM_COL32(185, 195, 210,
                                       static_cast<int>(225.0f * lineReveal));

    std::string displayText = exprLines[static_cast<size_t>(i)];
    const float maxTextWidth = scrollSize.x - 24.0f;
    if (ImGui::CalcTextSize(displayText.c_str()).x > maxTextWidth) {
      const char *ellipsis = "...";
      while (!displayText.empty()) {
        std::string candidate = displayText + ellipsis;
        if (ImGui::CalcTextSize(candidate.c_str()).x <= maxTextWidth) {
          displayText = candidate;
          break;
        }
        displayText.pop_back();
      }
      if (displayText.empty())
        displayText = ellipsis;
    }

    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::SetCursorScreenPos(ImVec2(linePos.x + 4.0f, linePos.y));
    ImGui::TextUnformatted(displayText.c_str());
    if (i == activeExprIndex && shouldFollowLatest)
      ImGui::SetScrollHereY(1.0f);
    ImGui::PopStyleColor();
  }

  m_FibonacciLastFollowedIndex = activeExprIndex;

  ImGui::Dummy(ImVec2(0.0f, contentPadBottomY));

  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::PopID();

  
  ImGui::SetCursorScreenPos(origin);
  ImGui::Dummy(canvasSize);
  int currentSeqNum = std::max(1, std::min(n, m_FibonacciStep + 1));
  std::string sequenceText = "Fibonacci: 0";
  for (int i = 1; i <= currentSeqNum; ++i)
    sequenceText += ", " + std::to_string(fibValue(i));

  ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.6f, 1.0f), "%s",
                     sequenceText.c_str());
  ImGui::SameLine(ImGui::GetContentRegionAvail().x - 110);
  float progress =
      maxStep > 0 ? static_cast<float>(m_FibonacciStep) / static_cast<float>(maxStep)
                  : 0.0f;
  ImGui::ProgressBar(progress, ImVec2(100, 0));
}

void Visualizer::SyncElementStates(const std::vector<int> &arr) {
  float maxVal =
      arr.empty()
          ? 1.0f
          : static_cast<float>(*std::max_element(arr.begin(), arr.end()));
  if (maxVal == 0)
    maxVal = 1.0f;

  m_Elements.resize(arr.size());

  StepType currentType = StepType::Default;
  int idx1 = -1, idx2 = -1;
  if (m_CurrentStepIndex >= 0 &&
      m_CurrentStepIndex < static_cast<int>(m_Steps.size())) {
    currentType = m_Steps[m_CurrentStepIndex].type;
    idx1 = m_Steps[m_CurrentStepIndex].index1;
    idx2 = m_Steps[m_CurrentStepIndex].index2;
  }

  for (size_t i = 0; i < arr.size(); ++i) {
    auto &e = m_Elements[i];
    e.targetHeight = static_cast<float>(arr[i]) / maxVal;

    
    if (e.currentHeight == 0.0f && m_AnimationProgress >= 1.0f)
      e.currentHeight = e.targetHeight;

    
    StepType elType = StepType::Default;
    if (m_Config.highlightOps &&
        (static_cast<int>(i) == idx1 || static_cast<int>(i) == idx2))
      elType = currentType;

    ImVec4 tc = GetThemeColorVec(elType);
    e.targetR = tc.x;
    e.targetG = tc.y;
    e.targetB = tc.z;
    e.targetA = tc.w;

    
    bool isActive =
        (static_cast<int>(i) == idx1 || static_cast<int>(i) == idx2);
    e.targetGlow = (m_Config.showGlow && isActive) ? 1.0f : 0.0f;
    e.targetScale = isActive ? 1.05f : 1.0f;
  }
}

void Visualizer::UpdateElementStates(float dt) {
  float speed = static_cast<float>(m_Config.animationSpeed) / 25.0f;
  float lerpRate = std::min(1.0f, dt * speed * 6.0f);

  for (auto &e : m_Elements) {
    
    float diff = e.targetHeight - e.currentHeight;
    e.currentHeight += diff * EaseOutCubic(lerpRate);

    
    LerpColor(e.currentR, e.currentG, e.currentB, e.currentA, e.targetR,
              e.targetG, e.targetB, e.targetA, lerpRate);

    
    e.scale += (e.targetScale - e.scale) * lerpRate;

    
    e.glowIntensity += (e.targetGlow - e.glowIntensity) * lerpRate * 0.8f;

    
    e.pulsePhase += dt * 4.0f;
    if (e.pulsePhase > 2.0f * (float)M_PI)
      e.pulsePhase -= 2.0f * (float)M_PI;
  }
}

void Visualizer::Update() {
  float dt = ImGui::GetIO().DeltaTime;
  m_GlobalTime += dt;

  
  if (m_IsPlaying && !m_Steps.empty() && m_AnimationProgress >= 0.9f) {
    float speed = static_cast<float>(m_Config.animationSpeed);
    float delay = (210.0f - speed) / 1000.0f;

    m_PlayTimer += dt;
    if (m_PlayTimer >= delay) {
      m_PlayTimer = 0.0f;
      StepForward();
    }
  }

  
  if (m_Config.smoothAnimation && m_AnimationProgress < 1.0f) {
    float animSpeed = static_cast<float>(m_Config.animationSpeed) / 30.0f;
    m_AnimationProgress += dt * animSpeed * 2.5f;
    if (m_AnimationProgress > 1.0f)
      m_AnimationProgress = 1.0f;
  } else if (!m_Config.smoothAnimation) {
    m_AnimationProgress = 1.0f;
  }

  
  UpdateElementStates(dt);
}

void Visualizer::Render() {
  RenderControlPanel();

  ImVec2 canvasSize = ImGui::GetContentRegionAvail();
  canvasSize.y -= 28.0f;
  if (canvasSize.y < 50.0f)
    canvasSize.y = 50.0f;
  ImVec2 canvasPos = ImGui::GetCursorScreenPos();

  if (m_VisualizationMode == VisualizationMode::FactorialLadder) {
    static const std::vector<int> emptyArray;
    bool hasFactorialSteps = false;
    if (!m_Steps.empty()) {
      const std::string &firstDesc = m_Steps.front().description;
      hasFactorialSteps = (firstDesc.find("factorial") != std::string::npos) ||
                          (firstDesc.find("Base case") != std::string::npos);
    }

    const std::vector<int> &ladderArray =
        hasFactorialSteps
            ? ((m_CurrentStepIndex < 0 && !m_Steps.empty()) ? m_Steps.front().array
                                                             : GetCurrentArray())
            : emptyArray;
    RenderLadder(ladderArray, canvasPos, canvasSize);
    ImGui::Dummy(canvasSize);
  } else {
    const std::vector<int> &currentArray = GetCurrentArray();
    if (!currentArray.empty()) {
      RenderBarGraph(currentArray, canvasPos, canvasSize);
      ImGui::Dummy(canvasSize);
    }
  }

  RenderStepInfo();
}

void Visualizer::RenderControlPanel() {
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));

  if (GetTotalSteps() > 0) {

    
    if (m_IsPlaying) {
      ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(200, 80, 60, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            IM_COL32(220, 100, 80, 255));
      if (ImGui::Button(ICON_FA_PAUSE "  Pause"))
        Pause();
      ImGui::PopStyleColor(2);
    } else {
      ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(50, 180, 100, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                            IM_COL32(70, 200, 120, 255));
      if (ImGui::Button(ICON_FA_PLAY "  Play")) {
        if (m_CurrentStepIndex < 0)
          m_CurrentStepIndex = 0;
        Play();
      }
      ImGui::PopStyleColor(2);
    }

    ImGui::SameLine();

    
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(70, 70, 90, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(90, 90, 120, 255));
    if (ImGui::Button(ICON_FA_STEP_BACKWARD))
      StepBackward();
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_STEP_FORWARD))
      StepForward();
    ImGui::PopStyleColor(2);

    
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f), "Step %d / %d",
                       std::max(0, m_CurrentStepIndex + 1), GetTotalSteps());
  }

  ImGui::PopStyleVar(3);

  
  ImGui::Dummy(ImVec2(0, 16.0f));
}

void Visualizer::RenderStepInfo() {
  if (m_CurrentStepIndex >= 0 &&
      m_CurrentStepIndex < static_cast<int>(m_Steps.size())) {
    const AlgorithmStep &step = m_Steps[m_CurrentStepIndex];

    ImU32 typeColor = GetThemeColor(step.type);
    ImVec4 tc = ImGui::ColorConvertU32ToFloat4(typeColor);
    ImGui::TextColored(tc, "%s", step.description.c_str());
  } else {
    const char *idleText =
        (m_VisualizationMode == VisualizationMode::FactorialLadder)
            ? "Factorial Recursion"
            : "Ready — run an algorithm, then press Play";
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.6f, 1.0f), "%s", idleText);
  }

  ImGui::SameLine(ImGui::GetContentRegionAvail().x - 110);
  float progress =
      GetTotalSteps() > 0
          ? static_cast<float>(m_CurrentStepIndex + 1) / GetTotalSteps()
          : 0.0f;
  ImGui::ProgressBar(progress, ImVec2(100, 0));
}

void Visualizer::RenderBarGraph(const std::vector<int> &arr,
                                const ImVec2 &origin, const ImVec2 &size) {
  ImDrawList *dl = ImGui::GetWindowDrawList();
  const int n = static_cast<int>(arr.size());
  if (n == 0)
    return;

  if (m_Config.showGrid) {
    constexpr int gridLines = 5;
    for (int i = 0; i <= gridLines; ++i) {
      float y = origin.y + (size.y / gridLines) * i;
      dl->AddLine(ImVec2(origin.x, y), ImVec2(origin.x + size.x, y),
                  IM_COL32(255, 255, 255, 18), 1.0f);
    }
  }

  float barWidth = (size.x - 4.0f) / n;
  float spacing = std::max(1.0f, barWidth * 0.12f);
  float actualW = barWidth - spacing;
  if (actualW < 2.0f)
    actualW = 2.0f;
  float maxH = size.y - 24.0f;

  SyncElementStates(arr); 

  for (int i = 0; i < n; ++i) {
    const auto &e = m_Elements[i];

    float h = e.currentHeight * maxH * e.scale;
    float x = origin.x + i * barWidth + spacing * 0.5f;
    float y = origin.y + size.y - h;

    ImVec2 barMin(x, y);
    ImVec2 barMax(x + actualW, origin.y + size.y);

    ImU32 col = GetElementColor(i);

    
    if (m_Config.showGlow && e.glowIntensity > 0.01f) {
      float pulse = 0.7f + 0.3f * sinf(e.pulsePhase);
      int glowAlpha = static_cast<int>(40.0f * e.glowIntensity * pulse);
      ImU32 glowCol =
          (col & 0x00FFFFFF) | (static_cast<ImU32>(glowAlpha) << 24);
      float expand = 4.0f * e.glowIntensity;
      dl->AddRectFilled(ImVec2(barMin.x - expand, barMin.y - expand),
                        ImVec2(barMax.x + expand, barMax.y), glowCol, 6.0f);
    }

    
    switch (m_Config.barStyle) {
    case BarStyle::Rounded: {
      float r = std::min(5.0f, actualW * 0.25f);
      dl->AddRectFilled(barMin, barMax, col, r, ImDrawFlags_RoundCornersTop);
      break;
    }
    case BarStyle::Sharp: {
      dl->AddRectFilled(barMin, barMax, col);
      break;
    }
    case BarStyle::Gradient: {
      
      ImVec4 cv = ImGui::ColorConvertU32ToFloat4(col);
      ImU32 topCol = col;
      ImU32 botCol = IM_COL32((int)(cv.x * 100), (int)(cv.y * 100),
                              (int)(cv.z * 100), 255);
      dl->AddRectFilledMultiColor(barMin, barMax, topCol, topCol, botCol,
                                  botCol);
      break;
    }
    }

    if (m_Config.showValues && actualW > 12.0f) {
      std::string val = std::to_string(arr[i]);
      ImVec2 tsz = ImGui::CalcTextSize(val.c_str());
      float tx = x + (actualW - tsz.x) * 0.5f;
      float ty = y - tsz.y - 4.0f;
      if (ty < origin.y)
        ty = origin.y;

      
      float px = 3.0f, py = 1.0f;
      dl->AddRectFilled(ImVec2(tx - px, ty - py),
                        ImVec2(tx + tsz.x + px, ty + tsz.y + py),
                        IM_COL32(20, 20, 30, 180), 4.0f);

      dl->AddText(ImVec2(tx, ty), IM_COL32(220, 220, 230, 255), val.c_str());
    }
  }
}

void Visualizer::RenderLadder(const std::vector<int> &arr, const ImVec2 &origin,
                              const ImVec2 &size) {
  ImDrawList *dl = ImGui::GetWindowDrawList();

  std::vector<int> displayArr = arr;
  for (const auto &step : m_Steps) {
    if (step.array.size() > displayArr.size())
      displayArr = step.array;
  }

  ImVec2 panelMin = origin;
  ImVec2 panelMax(origin.x + size.x, origin.y + size.y);
  dl->AddRectFilled(panelMin, panelMax, IM_COL32(18, 18, 22, 215));

  const float leftPad = 16.0f;
  const float rightPad = 18.0f;
  const float topPad = 16.0f;
  const float split = 0.70f;

  float leftW = size.x * split;

  ImVec2 stackMin(panelMin.x + leftPad, panelMin.y + topPad);
  ImVec2 stackMax(panelMin.x + leftW - rightPad, panelMax.y - topPad);
  ImVec2 retMin(panelMin.x + leftW + 6.0f, panelMin.y + topPad);
  ImVec2 retMax(panelMax.x - rightPad, panelMax.y - topPad);

  dl->AddText(ImVec2(stackMin.x, stackMin.y - 2.0f), IM_COL32(214, 214, 222, 255),
              "Factorial Ladder Graph");
  dl->AddText(ImVec2(retMin.x, retMin.y - 2.0f), IM_COL32(214, 214, 222, 255),
              "Return Values");

  const float titleGap = 26.0f;
  stackMin.y += titleGap;
  retMin.y += titleGap;

  
  
  {
    float lineH = ImGui::GetTextLineHeightWithSpacing();
    float boxH  = 14.0f + (lineH * 7.0f) + 10.0f;
    retMax.y    = retMin.y + boxH;
    retMax.x    = panelMax.x - rightPad;
  }

  int current = (m_CurrentStepIndex < 0) ? -1 : m_CurrentStepIndex;
  const int activeDepth = static_cast<int>(arr.size());
  const float stepAnim =
      m_Config.smoothAnimation ? EaseOutCubic(std::min(1.0f, std::max(0.0f, m_AnimationProgress)))
                               : 1.0f;
  const bool animateRevealNow =
      (current >= 0 && current < static_cast<int>(m_Steps.size()) &&
       m_Steps[current].type == StepType::Pivot);

  int revealedDepth = 0;
  for (int i = 0; i <= current && i < static_cast<int>(m_Steps.size()); ++i) {
    const int d = static_cast<int>(m_Steps[i].array.size());
    if (d > revealedDepth)
      revealedDepth = d;
  }

  const int depth = static_cast<int>(displayArr.size());
  if (depth > 0) {
    const float graphBottom = stackMax.y - 34.0f;
    const float graphTop = stackMin.y + 24.0f;
    const float usableHeight = std::max(80.0f, graphBottom - graphTop);
    const float totalWidth = (stackMax.x - stackMin.x);
    const float gap = std::max(8.0f, totalWidth * 0.014f);
    const float barW = std::max(24.0f, (totalWidth - (depth - 1) * gap) / depth);
    const int maxV =
      std::max(1, *std::max_element(displayArr.begin(), displayArr.end()));

    if (m_Config.showGrid) {
      constexpr int gridLines = 4;
      for (int i = 0; i <= gridLines; ++i) {
        float y = graphTop + (usableHeight / gridLines) * i;
        dl->AddLine(ImVec2(stackMin.x, y), ImVec2(stackMax.x, y),
                    IM_COL32(255, 255, 255, 14), 1.0f);
      }
    }

    dl->AddLine(ImVec2(stackMin.x, graphBottom), ImVec2(stackMax.x, graphBottom),
                IM_COL32(120, 120, 130, 210), 1.8f);

    for (int i = 0; i < depth; ++i) {
      if (i >= revealedDepth)
        continue;

      const int n = displayArr[i];
      const float ratio = static_cast<float>(n) / static_cast<float>(maxV);
      const float h = std::max(34.0f, usableHeight * ratio);

      const float x1 = stackMin.x + i * (barW + gap);
      const float x2 = std::min(x1 + barW, stackMax.x);
      const float y2 = graphBottom;
      const bool isActiveTop = (activeDepth > 0) ? (i == activeDepth - 1)
                                                  : (i == depth - 1);
      const bool isNewestRevealed = (i == revealedDepth - 1);
        const float reveal =
          (animateRevealNow && isNewestRevealed) ? stepAnim : 1.0f;
      const float hAnimated = std::max(26.0f, h * reveal);
      const float y1 = y2 - hAnimated;

      ImVec4 barTheme = GetThemeColorVec(isActiveTop ? StepType::Pivot : StepType::Default);
      ImVec4 barThemeDark = barTheme;
      barThemeDark.x *= 0.55f;
      barThemeDark.y *= 0.55f;
      barThemeDark.z *= 0.55f;
      const ImU32 topCol = IM_COL32((int)(barTheme.x * 255), (int)(barTheme.y * 255),
                                    (int)(barTheme.z * 255), 255);
      const ImU32 botCol = IM_COL32((int)(barThemeDark.x * 255),
                                    (int)(barThemeDark.y * 255),
                                    (int)(barThemeDark.z * 255), 255);

      if (m_Config.showGlow && isActiveTop) {
        dl->AddRectFilled(ImVec2(x1 - 3.0f, y1 - 4.0f), ImVec2(x2 + 3.0f, y2 + 1.0f),
                          IM_COL32((int)(barTheme.x * 255), (int)(barTheme.y * 255),
                                    (int)(barTheme.z * 255), 48),
                          6.0f);
      }

      switch (m_Config.barStyle) {
      case BarStyle::Rounded: {
        float r = std::min(6.0f, (x2 - x1) * 0.25f);
        dl->AddRectFilledMultiColor(ImVec2(x1, y1), ImVec2(x2, y2), topCol,
                                    topCol, botCol, botCol);
        dl->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(170, 210, 255, 230),
                    r, 0, 1.2f);
        break;
      }
      case BarStyle::Sharp: {
        dl->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), topCol);
        dl->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(170, 210, 255, 230));
        break;
      }
      case BarStyle::Gradient: {
        dl->AddRectFilledMultiColor(ImVec2(x1, y1), ImVec2(x2, y2), topCol,
                                    topCol, botCol, botCol);
        break;
      }
      }

      int runningValue = displayArr[0];
      for (int j = 1; j <= i; ++j)
        runningValue *= displayArr[j];
      const int nextValue = (i + 1 < depth) ? displayArr[i + 1] : 1;
      const int operationResult = runningValue * nextValue;
      std::string topLabel = std::to_string(runningValue) + " x " +
                             std::to_string(nextValue) + " = " +
                             std::to_string(operationResult);

      ImVec2 topSize = ImGui::CalcTextSize(topLabel.c_str());
      float tx = x1 + (x2 - x1 - topSize.x) * 0.5f;
      float ty = y1 - topSize.y - 7.0f;
      dl->AddRectFilled(ImVec2(tx - 5.0f, ty - 2.0f),
            ImVec2(tx + topSize.x + 5.0f, ty + topSize.y + 2.0f),
            IM_COL32(30, 30, 34, 220), 4.0f);
      dl->AddText(ImVec2(tx, ty), IM_COL32(238, 238, 242, 255), topLabel.c_str());

      std::string callLabel = "f(" + std::to_string(n) + ")";
      ImVec2 callSize = ImGui::CalcTextSize(callLabel.c_str());
      float cx = x1 + (x2 - x1 - callSize.x) * 0.5f;
      dl->AddText(ImVec2(cx, y2 + 6.0f), IM_COL32(208, 208, 216, 235),
          callLabel.c_str());

      if (m_Config.showValues) {
        std::string valueLabel = std::to_string(n);
        ImVec2 valueSize = ImGui::CalcTextSize(valueLabel.c_str());
        float vx = x1 + (x2 - x1 - valueSize.x) * 0.5f;
        float vy = y2 - valueSize.y - 6.0f;
        dl->AddText(ImVec2(vx, vy), IM_COL32(240, 240, 245, 255), valueLabel.c_str());
      }
    }
  } else {
    dl->AddText(ImVec2(stackMin.x, stackMin.y + 8.0f), IM_COL32(150, 150, 170, 255),
                "Run factorial to build the recursion ladder.");
  }

  dl->AddRectFilled(retMin, retMax, IM_COL32(18, 22, 30, 210), 8.0f);
  dl->AddRect(retMin, retMax, IM_COL32(110, 125, 150, 170), 8.0f, 0, 1.5f);

  
  const float pad = 6.0f;
  ImGui::SetCursorScreenPos(ImVec2(retMin.x + pad, retMin.y + pad));
  ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 2.0f));
  float childW = retMax.x - retMin.x - pad * 2.0f;
  float childH = retMax.y - retMin.y - pad * 2.0f;

  bool anyContent = false;
  if (ImGui::BeginChild("##ret_scroll", ImVec2(childW, childH), false,
                        ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
    for (int i = 0; i <= current && i < static_cast<int>(m_Steps.size()); ++i) {
      const AlgorithmStep &s = m_Steps[i];
      if (s.type != StepType::Merge && s.type != StepType::Sorted)
        continue;

      anyContent = true;
      std::string label;
      if (s.type == StepType::Sorted) {
        label = "base: factorial(" + std::to_string(s.index1) + ") = " +
                std::to_string(s.index2);
      } else {
        label = "return: factorial(" + std::to_string(s.index1) + ") = " +
                std::to_string(s.index2);
      }

      const bool isCurrentStep = (i == current);
      float alpha = isCurrentStep ? 0.92f : 1.0f;
      if (s.type == StepType::Merge) {
        ImGui::PushStyleColor(ImGuiCol_Text,
                              ImVec4(230/255.f, 235/255.f, 244/255.f, alpha));
      } else {
        ImGui::PushStyleColor(ImGuiCol_Text,
                              ImVec4(180/255.f, 220/255.f, 180/255.f, alpha));
      }
      ImGui::TextUnformatted(label.c_str());
      ImGui::PopStyleColor();
    }

    if (!anyContent) {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(150/255.f, 150/255.f, 170/255.f, 1.0f));
      ImGui::TextUnformatted("Return values will appear here");
      ImGui::PopStyleColor();
    }

    
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 1.0f)
      ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
  }
  ImGui::PopStyleVar();
  ImGui::PopStyleColor();
}

int Visualizer::GetStepCountOfType(StepType type) const {
  int count = 0;
  for (const auto &s : m_Steps)
    if (s.type == type)
      ++count;
  return count;
}

void Visualizer::SetGraphSimulation(int vertexCount,
                                    const std::vector<Edge> &edges,
                                    const std::vector<GraphStep> &steps) {
  // Loads a new simulation snapshot set and keeps playback paused.
  m_GraphVertexCount = vertexCount;
  m_GraphEdges = edges;
  m_GraphSteps = steps;
  m_GraphStep = 0;
  m_GraphPlayTimer = 0.0f;
  m_GraphPlaying = false;
  m_ShowGraphSimulation = true;
}

void Visualizer::PlayGraphSimulation() {
  if (!m_ShowGraphSimulation || m_GraphSteps.empty())
    return;

  // Restart from step 0 if a prior run already finished.
  if (m_GraphStep >= static_cast<int>(m_GraphSteps.size()) - 1)
    m_GraphStep = 0;

  m_GraphPlayTimer = 0.0f;
  m_GraphPlaying = true;
}

void Visualizer::ClearGraphSimulation() {
  m_ShowGraphSimulation = false;
  m_GraphPlaying = false;
  m_GraphPlayTimer = 0.0f;
  m_GraphStep = 0;
  m_GraphVertexCount = 0;
  m_GraphEdges.clear();
  m_GraphSteps.clear();
}

void Visualizer::UpdateKruskals(float dt) {
  // Advances graph replay based on animation speed settings.
  if (!m_ShowGraphSimulation || !m_GraphPlaying)
    return;

  int maxStep = static_cast<int>(m_GraphSteps.size());
  if (m_GraphStep >= maxStep) {
    m_GraphPlaying = false;
    return;
  }

  float stepInterval = 1.20f;
  float speedScale = 1.0f - (static_cast<float>(m_Config.animationSpeed) * 0.0045f);
  if (speedScale < 0.18f) speedScale = 0.18f;
  if (speedScale > 1.0f)  speedScale = 1.0f;
  stepInterval *= speedScale;

  m_GraphPlayTimer += dt;
  if (m_GraphPlayTimer >= stepInterval) {
    m_GraphPlayTimer = 0.0f;
    ++m_GraphStep;
    if (m_GraphStep >= maxStep) {
      m_GraphStep = maxStep - 1;
      m_GraphPlaying = false;
    }
  }
}

void Visualizer::RenderKruskals() {
  auto FormatEdgePair = [](int a, int b) {
    int lo = std::min(a, b) + 1;
    int hi = std::max(a, b) + 1;
    std::string label = "(" + std::to_string(lo) + "," + std::to_string(hi) + ")";
    return label;
  };

  auto shade = [](ImU32 color, float scale, int alpha) {
    ImVec4 v = ImGui::ColorConvertU32ToFloat4(color);
    int r = static_cast<int>(v.x * 255.0f * scale);
    int g = static_cast<int>(v.y * 255.0f * scale);
    int b = static_cast<int>(v.z * 255.0f * scale);
    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    if (alpha < 0) alpha = 0;
    if (alpha > 255) alpha = 255;
    return IM_COL32(r, g, b, alpha);
  };

  const ImU32 themeDefault = GetThemeColor(StepType::Default);
  const ImU32 themeCompare = GetThemeColor(StepType::Compare);
  const ImU32 themeSwap = GetThemeColor(StepType::Swap);
  const ImU32 themeSorted = GetThemeColor(StepType::Sorted);
  const ImU32 themePivot = GetThemeColor(StepType::Pivot);

  const ImU32 graphBg = shade(themeDefault, 0.18f, 255);
  const ImU32 graphBorder = shade(themePivot, 0.40f, 185);
  const ImU32 rowHighlight = shade(themeCompare, 0.45f, 120);
  const ImU32 panelTextMuted = shade(themeDefault, 1.70f, 255);
  const ImU32 nodeBorder = shade(themeDefault, 0.24f, 255);

  
  const ImU32 kCompColors[] = {
      shade(themeDefault, 1.00f, 255),
      shade(themeCompare, 1.00f, 255),
      shade(themeSorted, 1.00f, 255),
      shade(themePivot, 1.00f, 255),
      shade(themeSwap, 1.00f, 255),
      shade(themeDefault, 1.30f, 255),
      shade(themeCompare, 0.75f, 255),
      shade(themePivot, 0.75f, 255),
  };
  static const int kNumColors = 8;

  
  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 6));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));

  if (m_ShowGraphSimulation) {
    int maxStep = static_cast<int>(m_GraphSteps.size());

    
    if (m_GraphPlaying) {
      ImGui::PushStyleColor(ImGuiCol_Button,        IM_COL32(200,  80,  60, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(220, 100,  80, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive,  IM_COL32(180,  65,  50, 255));
    } else {
      ImGui::PushStyleColor(ImGuiCol_Button,        IM_COL32( 50, 180, 100, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32( 70, 200, 120, 255));
      ImGui::PushStyleColor(ImGuiCol_ButtonActive,  IM_COL32( 40, 160,  90, 255));
    }
    if (ImGui::Button(m_GraphPlaying ? ICON_FA_PAUSE "  Pause"
                                     : ICON_FA_PLAY  "  Play")) {
      if (m_GraphStep >= maxStep - 1) m_GraphStep = 0;
      m_GraphPlaying = !m_GraphPlaying;
      m_GraphPlayTimer = 0.0f;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button,        IM_COL32(70, 70, 90, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(90, 90,120, 255));
    if (ImGui::Button(ICON_FA_STEP_BACKWARD)) {
      m_GraphPlaying = false;
      m_GraphPlayTimer = 0.0f;
      if (m_GraphStep > 0) --m_GraphStep;
    }
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_STEP_FORWARD)) {
      m_GraphPlaying = false;
      m_GraphPlayTimer = 0.0f;
      if (m_GraphStep < maxStep - 1) ++m_GraphStep;
    }
    ImGui::PopStyleColor(2);

    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f), "Step %d / %d",
                       m_GraphStep + 1, maxStep);
  }

  ImGui::PopStyleVar(3);
  ImGui::Dummy(ImVec2(0, 6.0f));

  if (!m_ShowGraphSimulation) {
    ImVec2 sz = ImGui::GetContentRegionAvail();
    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList *dl = ImGui::GetWindowDrawList();
    const char *msg = "Click \"Run Graph Algorithm\" to visualize the algorithm";
    ImVec2 msz = ImGui::CalcTextSize(msg);
    dl->AddText(ImVec2(origin.x + (sz.x - msz.x) * 0.5f,
                       origin.y + (sz.y - msz.y) * 0.5f),
                IM_COL32(170, 180, 200, 200), msg);
    ImGui::Dummy(sz);
    return;
  }

  const GraphStep &cur = m_GraphSteps[m_GraphStep];
  const bool isPrim = (cur.algorithm == GraphAlgorithmType::Prim);
  int n = m_GraphVertexCount;

  ImVec2 avail = ImGui::GetContentRegionAvail();
  avail.y -= 28.0f; 
  if (avail.y < 80.0f) avail.y = 80.0f;

  float edgeListW = avail.x * 0.30f;
  float graphW    = avail.x - edgeListW - 8.0f;

  ImVec2 graphOrigin = ImGui::GetCursorScreenPos();
  ImDrawList *dl = ImGui::GetWindowDrawList();

  
  dl->PushClipRect(graphOrigin,
                   ImVec2(graphOrigin.x + graphW, graphOrigin.y + avail.y), true);

  
  dl->AddRectFilled(graphOrigin,
                    ImVec2(graphOrigin.x + graphW, graphOrigin.y + avail.y),
          graphBg, 8.0f);
  dl->AddRect(graphOrigin,
              ImVec2(graphOrigin.x + graphW, graphOrigin.y + avail.y),
        graphBorder, 8.0f, 0, 1.0f);

  int cols = (int)ceilf(sqrtf((float)n));
  int rows = (n + cols - 1) / cols;

  float marginX = graphW  * 0.12f;
  float marginY = avail.y * 0.12f;
  float cellW = (graphW  - 2.0f * marginX) / std::max(cols - 1, 1);
  float cellH = (avail.y - 2.0f * marginY) / std::max(rows - 1, 1);
  if (cols == 1) cellW = 0.0f;
  if (rows == 1) cellH = 0.0f;

  std::vector<ImVec2> nodePos(n);
  for (int i = 0; i < n; ++i) {
    int row = i / cols;
    int col = i % cols;
    
    int nodesInRow = (row == rows - 1 && n % cols != 0) ? n % cols : cols;
    float rowOffsetX = (cols - nodesInRow) * cellW * 0.5f;
    nodePos[i] = ImVec2(
        graphOrigin.x + marginX + rowOffsetX + col * cellW,
        graphOrigin.y + marginY + row * cellH);
  }

  
  for (const auto &e : m_GraphEdges) {
    ImVec2 p0 = nodePos[e.src];
    ImVec2 p1 = nodePos[e.dest];

    
    bool isCurEdge  = (cur.u == e.src && cur.v == e.dest) ||
                      (cur.u == e.dest && cur.v == e.src);
    bool isInMST    = false;
    bool isRejected = false;

    for (const auto &me : cur.mstEdges) {
      if ((me.first == e.src && me.second == e.dest) ||
          (me.first == e.dest && me.second == e.src)) {
        isInMST = true;
        break;
      }
    }

    
    if (!isInMST && !isCurEdge) {
      for (int s = 0; s <= m_GraphStep; ++s) {
        const GraphStep &gs = m_GraphSteps[s];
        if (gs.type == GraphStepType::Reject &&
            ((gs.u == e.src && gs.v == e.dest) ||
             (gs.u == e.dest && gs.v == e.src))) {
          isRejected = true;
          break;
        }
      }
    }

    ImU32  color;
    float  thickness;
    if (isCurEdge && cur.type == GraphStepType::Consider) {
      float pulse = 0.7f + 0.3f * sinf(m_GlobalTime * 6.0f);
      int alpha = (int)(pulse * 255);
      color = shade(themeCompare, 1.0f, alpha);
      thickness = 3.5f;
    } else if (isCurEdge && cur.type == GraphStepType::Accept) {
      color = shade(themeSorted, 1.0f, 255);
      thickness = 3.5f;
    } else if (isCurEdge && cur.type == GraphStepType::Reject) {
      color = shade(themeSwap, 1.0f, 255);
      thickness = 2.5f;
    } else if (isInMST) {
      color = shade(themeSorted, 1.0f, 220);
      thickness = 2.5f;
    } else if (isRejected) {
      color = shade(themeSwap, 0.75f, 140);
      thickness = 1.0f;
    } else {
      color = shade(themeDefault, 0.65f, 200);
      thickness = 1.2f;
    }

    dl->AddLine(p0, p1, color, thickness);

    
    ImVec2 mid((p0.x + p1.x) * 0.5f, (p0.y + p1.y) * 0.5f);
    char wbuf[8];
    snprintf(wbuf, sizeof(wbuf), "%d", e.weight);
    ImVec2 tsz = ImGui::CalcTextSize(wbuf);
    
    dl->AddRectFilled(ImVec2(mid.x - tsz.x * 0.5f - 3, mid.y - tsz.y * 0.5f - 1),
                      ImVec2(mid.x + tsz.x * 0.5f + 3, mid.y + tsz.y * 0.5f + 1),
              shade(themeDefault, 0.20f, 200), 3.0f);
    dl->AddText(ImVec2(mid.x - tsz.x * 0.5f, mid.y - tsz.y * 0.5f),
            isCurEdge ? shade(themeCompare, 1.0f, 255)
              : shade(themeDefault, 1.75f, 220),
                wbuf);
  }

  
  float nodeR = 11.0f;
  for (int i = 0; i < n; ++i) {
    int compIdx = (i < (int)cur.componentId.size()) ? cur.componentId[i] : i;
    ImU32 nodeColor = kCompColors[compIdx % kNumColors];

    dl->AddCircleFilled(nodePos[i], nodeR, nodeBorder);
    dl->AddCircleFilled(nodePos[i], nodeR - 2.0f, nodeColor);

    
    char nbuf[4];
    snprintf(nbuf, sizeof(nbuf), "%d", i + 1);
    ImVec2 nsz = ImGui::CalcTextSize(nbuf);
    dl->AddText(ImVec2(nodePos[i].x - nsz.x * 0.5f, nodePos[i].y - nsz.y * 0.5f),
          shade(themeDefault, 0.12f, 255), nbuf);
  }

  dl->PopClipRect();

  ImGui::SetCursorScreenPos(ImVec2(graphOrigin.x + graphW + 8.0f, graphOrigin.y));
  ImGui::BeginChild("##edgeList", ImVec2(edgeListW, avail.y), false,
                    ImGuiWindowFlags_NoScrollbar);

  ImGui::PushStyleColor(ImGuiCol_Text,
                        ImGui::ColorConvertU32ToFloat4(panelTextMuted));
  if (isPrim)
    ImGui::Text(ICON_FA_PROJECT_DIAGRAM "  Used Edges ");
  else
    ImGui::Text(ICON_FA_LIST "  Sorted Edges");
  ImGui::PopStyleColor();
  ImGui::Separator();

  auto GetEdgeWeight = [&](int u, int v) {
    for (const auto &e : m_GraphEdges) {
      if ((e.src == u && e.dest == v) || (e.src == v && e.dest == u))
        return e.weight;
    }
    return 0;
  };

  std::vector<Edge> panelEdges;
  if (isPrim) {
    for (const auto &me : cur.mstEdges) {
      Edge used;
      used.src = me.first;
      used.dest = me.second;
      used.weight = GetEdgeWeight(me.first, me.second);
      panelEdges.push_back(used);
    }
  } else {
    panelEdges = m_GraphEdges;
    std::sort(panelEdges.begin(), panelEdges.end(),
              [](const Edge &a, const Edge &b) { return a.weight < b.weight; });
  }

  ImGui::BeginChild("##edgeScroll", ImVec2(0, avail.y - 40.0f), false);
  if (isPrim) {
    for (int i = 0; i < static_cast<int>(panelEdges.size()); ++i) {
      const Edge &e = panelEdges[i];
      const bool isCurrentAccepted =
          (cur.type == GraphStepType::Accept) &&
          (((cur.u == e.src) && (cur.v == e.dest)) ||
           ((cur.u == e.dest) && (cur.v == e.src)));

      if (isCurrentAccepted) {
        ImVec2 rmin = ImGui::GetCursorScreenPos();
        ImVec2 rmax(rmin.x + edgeListW - 4,
                    rmin.y + ImGui::GetTextLineHeightWithSpacing());
        ImGui::GetWindowDrawList()->AddRectFilled(rmin, rmax,
                                                  rowHighlight, 3.0f);
      }

      std::string rowbuf =
          FormatEdgePair(e.src, e.dest) + "  w=" + std::to_string(e.weight);
      ImVec4 textColor = isCurrentAccepted
                 ? ImGui::ColorConvertU32ToFloat4(
                   shade(themeCompare, 1.0f, 255))
                 : ImGui::ColorConvertU32ToFloat4(
                   shade(themeDefault, 1.65f, 255));
      ImGui::TextColored(textColor, "%s", rowbuf.c_str());
    }
  } else {
    for (int ei = 0; ei < (int)panelEdges.size(); ++ei) {
      const Edge &e = panelEdges[ei];

      bool isCurEdge = (cur.u == e.src && cur.v == e.dest) ||
                       (cur.u == e.dest && cur.v == e.src);
      bool isInMST = false;
      bool isRejected = false;

      for (const auto &me : cur.mstEdges) {
        if ((me.first == e.src && me.second == e.dest) ||
            (me.first == e.dest && me.second == e.src)) {
          isInMST = true;
          break;
        }
      }

      if (!isInMST && !isCurEdge) {
        for (int s = 0; s <= m_GraphStep; ++s) {
          const GraphStep &gs = m_GraphSteps[s];
          if (gs.type == GraphStepType::Reject &&
              ((gs.u == e.src && gs.v == e.dest) ||
               (gs.u == e.dest && gs.v == e.src))) {
            isRejected = true;
            break;
          }
        }
      }

      
      if (isCurEdge) {
        ImVec2 rmin = ImGui::GetCursorScreenPos();
        ImVec2 rmax(rmin.x + edgeListW - 4,
                    rmin.y + ImGui::GetTextLineHeightWithSpacing());
        ImGui::GetWindowDrawList()->AddRectFilled(rmin, rmax,
                                                  rowHighlight, 3.0f);
      }

      
      const char *icon;
      ImVec4 iconColor;
      if (isInMST) {
        icon = ICON_FA_CHECK;
        iconColor = ImGui::ColorConvertU32ToFloat4(shade(themeSorted, 1.0f, 255));
      } else if (isRejected) {
        icon = ICON_FA_TIMES;
        iconColor = ImGui::ColorConvertU32ToFloat4(shade(themeSwap, 1.0f, 255));
      } else if (isCurEdge) {
        icon = ICON_FA_ARROW_RIGHT;
        iconColor = ImGui::ColorConvertU32ToFloat4(shade(themeCompare, 1.0f, 255));
      } else {
        icon = "  ";
        iconColor = ImGui::ColorConvertU32ToFloat4(shade(themeDefault, 1.45f, 255));
      }

      ImGui::TextColored(iconColor, "%s", icon);
      ImGui::SameLine();

      std::string rowbuf =
          FormatEdgePair(e.src, e.dest) + "  w=" + std::to_string(e.weight);

      ImVec4 textColor = isCurEdge
                 ? ImGui::ColorConvertU32ToFloat4(
                   shade(themeCompare, 1.0f, 255))
                 : isInMST
                   ? ImGui::ColorConvertU32ToFloat4(
                     shade(themeSorted, 1.0f, 255))
                   : isRejected
                     ? ImGui::ColorConvertU32ToFloat4(
                       shade(themeSwap, 0.75f, 200))
                     : ImGui::ColorConvertU32ToFloat4(
                       shade(themeDefault, 1.65f, 255));
      ImGui::TextColored(textColor, "%s", rowbuf.c_str());
    }
  }

  if (panelEdges.empty()) {
    ImGui::TextDisabled("%s", isPrim ? "No used edges yet." : "No edges.");
  }

  ImGui::Dummy(ImVec2(0, 6));
  ImGui::Separator();
  ImGui::Dummy(ImVec2(0, 2));
  ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(shade(themeCompare, 1.0f, 255)),
                     "Weighted Time: %d", cur.mstCost);

  ImGui::EndChild(); 
  ImGui::EndChild(); 

  ImGui::SetCursorScreenPos(ImVec2(graphOrigin.x, graphOrigin.y + avail.y + 4.0f));

    ImVec4 descColor = cur.type == GraphStepType::Accept
           ? ImGui::ColorConvertU32ToFloat4(
             shade(themeSorted, 1.0f, 255))
           : cur.type == GraphStepType::Reject
             ? ImGui::ColorConvertU32ToFloat4(
               shade(themeSwap, 1.0f, 255))
             : cur.type == GraphStepType::Done
               ? ImGui::ColorConvertU32ToFloat4(
                 shade(themeCompare, 1.0f, 255))
               : ImGui::ColorConvertU32ToFloat4(
                 shade(themeCompare, 1.0f, 255));
  ImGui::TextColored(descColor, "%s", cur.description.c_str());

  
  float progress = m_GraphSteps.empty() ? 0.0f
      : static_cast<float>(m_GraphStep + 1) / static_cast<float>(m_GraphSteps.size());
  ImGui::SameLine(ImGui::GetContentRegionAvail().x - 110.0f +
                  ImGui::GetCursorPosX());
  ImGui::ProgressBar(progress, ImVec2(100, 0));
}