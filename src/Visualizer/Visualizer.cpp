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

float Visualizer::EaseOutElastic(float t) {
  if (t <= 0.0f)
    return 0.0f;
  if (t >= 1.0f)
    return 1.0f;
  return sinf(-13.0f * (float)M_PI * 0.5f * (t + 1.0f)) *
             powf(2.0f, -10.0f * t) +
         1.0f;
}

float Visualizer::EaseOutBounce(float t) {
  if (t < 1.0f / 2.75f) {
    return 7.5625f * t * t;
  } else if (t < 2.0f / 2.75f) {
    t -= 1.5f / 2.75f;
    return 7.5625f * t * t + 0.75f;
  } else if (t < 2.5f / 2.75f) {
    t -= 2.25f / 2.75f;
    return 7.5625f * t * t + 0.9375f;
  } else {
    t -= 2.625f / 2.75f;
    return 7.5625f * t * t + 0.984375f;
  }
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

  switch (m_Config.theme) {
  case ColorTheme::Cyberpunk:
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
  case ColorTheme::Ocean:
    switch (type) {
    case StepType::Compare:
      return c(255, 180, 50);
    case StepType::Swap:
    case StepType::Overwrite:
      return c(0, 200, 180);
    case StepType::Sorted:
      return c(33, 150, 243);
    case StepType::Pivot:
      return c(156, 39, 176);
    default:
      return c(0, 210, 240);
    }
  case ColorTheme::Sunset:
    switch (type) {
    case StepType::Compare:
      return c(255, 210, 60);
    case StepType::Swap:
    case StepType::Overwrite:
      return c(255, 87, 34);
    case StepType::Sorted:
      return c(255, 60, 120);
    case StepType::Pivot:
      return c(180, 50, 200);
    default:
      return c(255, 160, 40);
    }
  case ColorTheme::Matrix:
    switch (type) {
    case StepType::Compare:
      return c(0, 255, 0);
    case StepType::Swap:
    case StepType::Overwrite:
      return c(255, 255, 255);
    case StepType::Sorted:
      return c(0, 200, 0);
    case StepType::Pivot:
      return c(0, 255, 255);
    default:
      return c(0, 120, 0);
    }
  case ColorTheme::Pastel:
    switch (type) {
    case StepType::Compare:
      return c(255, 223, 186);
    case StepType::Swap:
    case StepType::Overwrite:
      return c(255, 179, 186);
    case StepType::Sorted:
      return c(186, 255, 201);
    case StepType::Pivot:
      return c(224, 187, 228);
    default:
      return c(186, 225, 255);
    }
  }
  return ImVec4(0.4f, 0.6f, 1.0f, 1.0f);
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
  if (!m_ShowTowerIllustration)
    return;

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

    // First time — snap to position
    if (e.currentHeight == 0.0f && m_AnimationProgress >= 1.0f)
      e.currentHeight = e.targetHeight;

    // Determine target color
    StepType elType = StepType::Default;
    if (m_Config.highlightOps &&
        (static_cast<int>(i) == idx1 || static_cast<int>(i) == idx2))
      elType = currentType;

    ImVec4 tc = GetThemeColorVec(elType);
    e.targetR = tc.x;
    e.targetG = tc.y;
    e.targetB = tc.z;
    e.targetA = tc.w;

    // Glow for active elements
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
    // Height — ease toward target
    float diff = e.targetHeight - e.currentHeight;
    e.currentHeight += diff * EaseOutCubic(lerpRate);

    // Color — smooth lerp
    LerpColor(e.currentR, e.currentG, e.currentB, e.currentA, e.targetR,
              e.targetG, e.targetB, e.targetA, lerpRate);

    // Scale
    e.scale += (e.targetScale - e.scale) * lerpRate;

    // Glow
    e.glowIntensity += (e.targetGlow - e.glowIntensity) * lerpRate * 0.8f;

    // Pulse phase (continuous)
    e.pulsePhase += dt * 4.0f;
    if (e.pulsePhase > 2.0f * (float)M_PI)
      e.pulsePhase -= 2.0f * (float)M_PI;
  }
}

void Visualizer::Update() {
  float dt = ImGui::GetIO().DeltaTime;
  m_GlobalTime += dt;

  // Auto-play timer
  if (m_IsPlaying && !m_Steps.empty() && m_AnimationProgress >= 0.9f) {
    float speed = static_cast<float>(m_Config.animationSpeed);
    float delay = (210.0f - speed) / 1000.0f;

    m_PlayTimer += dt;
    if (m_PlayTimer >= delay) {
      m_PlayTimer = 0.0f;
      StepForward();
    }
  }

  // Advance animation progress
  if (m_Config.smoothAnimation && m_AnimationProgress < 1.0f) {
    float animSpeed = static_cast<float>(m_Config.animationSpeed) / 30.0f;
    m_AnimationProgress += dt * animSpeed * 2.5f;
    if (m_AnimationProgress > 1.0f)
      m_AnimationProgress = 1.0f;
  } else if (!m_Config.smoothAnimation) {
    m_AnimationProgress = 1.0f;
  }

  // Animate individual elements
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

    // Play / Pause
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

    // Step controls
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(70, 70, 90, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(90, 90, 120, 255));
    if (ImGui::Button(ICON_FA_STEP_BACKWARD))
      StepBackward();
    ImGui::SameLine();
    if (ImGui::Button(ICON_FA_STEP_FORWARD))
      StepForward();
    ImGui::PopStyleColor(2);

    // Step counter
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.7f, 1.0f), "Step %d / %d",
                       std::max(0, m_CurrentStepIndex + 1), GetTotalSteps());
  }

  ImGui::PopStyleVar(3);

  // Add vertical padding so graph doesn't overlap the buttons
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
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.6f, 1.0f),
                       "Ready — run an algorithm, then press Play");
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

  SyncElementStates(arr); // ensure we have targets

  for (int i = 0; i < n; ++i) {
    const auto &e = m_Elements[i];

    float h = e.currentHeight * maxH * e.scale;
    float x = origin.x + i * barWidth + spacing * 0.5f;
    float y = origin.y + size.y - h;

    ImVec2 barMin(x, y);
    ImVec2 barMax(x + actualW, origin.y + size.y);

    ImU32 col = GetElementColor(i);

    // Glow (drawn behind)
    if (m_Config.showGlow && e.glowIntensity > 0.01f) {
      float pulse = 0.7f + 0.3f * sinf(e.pulsePhase);
      int glowAlpha = static_cast<int>(40.0f * e.glowIntensity * pulse);
      ImU32 glowCol =
          (col & 0x00FFFFFF) | (static_cast<ImU32>(glowAlpha) << 24);
      float expand = 4.0f * e.glowIntensity;
      dl->AddRectFilled(ImVec2(barMin.x - expand, barMin.y - expand),
                        ImVec2(barMax.x + expand, barMax.y), glowCol, 6.0f);
    }

    // Bar body
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
      // Top color = original, bottom = darker
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

      // Pill background
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

      const int nextValue = (i + 1 < depth) ? displayArr[i + 1] : 1;
      std::string topLabel = std::to_string(n) + " x " + std::to_string(nextValue) +
                             " = " + std::to_string(n * nextValue);

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

  dl->AddRectFilled(retMin, retMax, IM_COL32(24, 24, 28, 220), 6.0f);
  dl->AddRect(retMin, retMax, IM_COL32(122, 122, 134, 220), 6.0f);

  float lineY = retMin.y + 10.0f;
  int returnCount = 0;

  for (int i = 0; i <= current && i < static_cast<int>(m_Steps.size()); ++i) {
    const AlgorithmStep &s = m_Steps[i];
    if (s.type != StepType::Merge && s.type != StepType::Sorted)
      continue;

    std::string label;
    if (s.type == StepType::Sorted) {
      label = "base: factorial(" + std::to_string(s.index1) + ") = " +
              std::to_string(s.index2);
    } else {
      label = "return: factorial(" + std::to_string(s.index1) + ") = " +
              std::to_string(s.index2);
    }

    if (lineY + 18.0f > retMax.y)
      break;
    const bool isCurrentStep = (i == current);
    if (s.type == StepType::Merge) {
      int chipAlpha = isCurrentStep ? 230 : 200;
      ImVec2 chipMin(retMin.x + 6.0f, lineY - 1.0f);
      ImVec2 chipMax(retMax.x - 6.0f, lineY + 16.0f);
      dl->AddRectFilled(chipMin, chipMax, IM_COL32(32, 45, 66, chipAlpha), 4.0f);
    }
    const int textAlpha = isCurrentStep ? 235 : 255;
    dl->AddText(ImVec2(retMin.x + 10.0f, lineY), IM_COL32(230, 235, 244, textAlpha),
                label.c_str());
    lineY += 18.0f;
    ++returnCount;
  }

  if (returnCount == 0) {
    dl->AddText(ImVec2(retMin.x + 8.0f, retMin.y + 10.0f),
                IM_COL32(150, 150, 170, 255),
                "Return values will appear here");
  }
}