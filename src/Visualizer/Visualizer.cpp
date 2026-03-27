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
  m_CurrentStepIndex = -1;
  m_IsPlaying = false;
  m_AnimationProgress = 1.0f;
  SyncElementStates(m_OriginalArray);
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
  const std::vector<int> &currentArray = GetCurrentArray();
  if (!currentArray.empty()) {
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    canvasSize.y -= 28.0f; // reserve space for step info
    if (canvasSize.y < 50.0f)
      canvasSize.y = 50.0f;

    ImVec2 canvasPos = ImGui::GetCursorScreenPos();

    switch (m_Config.vizType) {
    case VizType::BarGraph:
      RenderBarGraph(currentArray, canvasPos, canvasSize);
      break;
    case VizType::LineGraph:
      RenderLineGraph(currentArray, canvasPos, canvasSize);
      break;
    case VizType::Circular:
      RenderCircular(currentArray, canvasPos, canvasSize);
      break;
    case VizType::ScatterPlot:
      RenderScatterPlot(currentArray, canvasPos, canvasSize);
      break;
    case VizType::ColorSpectrum:
      RenderColorSpectrum(currentArray, canvasPos, canvasSize);
      break;
    case VizType::Starburst:
      RenderStarburst(currentArray, canvasPos, canvasSize);
      break;
    case VizType::Pyramid:
      RenderPyramid(currentArray, canvasPos, canvasSize);
      break;
    }

    ImGui::Dummy(canvasSize);
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

void Visualizer::RenderLineGraph(const std::vector<int> &arr,
                                 const ImVec2 &origin, const ImVec2 &size) {
  ImDrawList *dl = ImGui::GetWindowDrawList();
  const int n = static_cast<int>(arr.size());
  if (n < 2)
    return;

  SyncElementStates(arr);
  if (m_Config.showGrid) {
    constexpr int gridLines = 5;
    for (int i = 0; i <= gridLines; ++i) {
      float y = origin.y + (size.y / gridLines) * i;
      dl->AddLine(ImVec2(origin.x, y), ImVec2(origin.x + size.x, y),
                  IM_COL32(255, 255, 255, 18), 1.0f);
    }
  }

  float maxH = size.y - 24.0f;
  float xStep = size.x / (n - 1);

  // Build point positions
  std::vector<ImVec2> pts(n);
  for (int i = 0; i < n; ++i) {
    float h = m_Elements[i].currentHeight * maxH;
    pts[i] = ImVec2(origin.x + i * xStep, origin.y + size.y - h);
  }

  // Fill area under curve
  ImU32 defaultCol = GetThemeColor(StepType::Default);
  ImVec4 dc = ImGui::ColorConvertU32ToFloat4(defaultCol);
  ImU32 fillCol =
      IM_COL32((int)(dc.x * 255), (int)(dc.y * 255), (int)(dc.z * 255), 25);

  for (int i = 0; i < n - 1; ++i) {
    ImVec2 bl(pts[i].x, origin.y + size.y);
    ImVec2 br(pts[i + 1].x, origin.y + size.y);
    dl->AddQuadFilled(pts[i], pts[i + 1], br, bl, fillCol);
  }

  // Line segments
  for (int i = 0; i < n - 1; ++i) {
    ImU32 c1 = GetElementColor(i);
    ImU32 c2 = GetElementColor(i + 1);
    // Use average color for the segment
    ImVec4 v1 = ImGui::ColorConvertU32ToFloat4(c1);
    ImVec4 v2 = ImGui::ColorConvertU32ToFloat4(c2);
    ImU32 segCol = IM_COL32((int)((v1.x + v2.x) * 0.5f * 255),
                            (int)((v1.y + v2.y) * 0.5f * 255),
                            (int)((v1.z + v2.z) * 0.5f * 255), 255);
    dl->AddLine(pts[i], pts[i + 1], segCol, 2.5f);
  }

  // Data points
  for (int i = 0; i < n; ++i) {
    ImU32 col = GetElementColor(i);
    const auto &e = m_Elements[i];
    float r = 4.0f * e.scale;

    // Outer ring
    if (e.glowIntensity > 0.01f) {
      float pulse = 0.7f + 0.3f * sinf(e.pulsePhase);
      int alpha = static_cast<int>(80.0f * e.glowIntensity * pulse);
      dl->AddCircleFilled(pts[i], r + 4.0f,
                          (col & 0x00FFFFFF) |
                              (static_cast<ImU32>(alpha) << 24));
    }

    dl->AddCircleFilled(pts[i], r, col);
    dl->AddCircle(pts[i], r + 1.5f, IM_COL32(255, 255, 255, 60), 0, 1.0f);

    // Value labels
    if (m_Config.showValues && xStep > 18.0f) {
      std::string val = std::to_string(arr[i]);
      ImVec2 tsz = ImGui::CalcTextSize(val.c_str());
      float tx = pts[i].x - tsz.x * 0.5f;
      float ty = pts[i].y - tsz.y - 8.0f;
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

//  CIRCULAR

void Visualizer::RenderCircular(const std::vector<int> &arr,
                                const ImVec2 &origin, const ImVec2 &size) {
  ImDrawList *dl = ImGui::GetWindowDrawList();
  const int n = static_cast<int>(arr.size());
  if (n == 0)
    return;

  SyncElementStates(arr);

  float cx = origin.x + size.x * 0.5f;
  float cy = origin.y + size.y * 0.5f;
  float maxRadius = std::min(size.x, size.y) * 0.5f - 30.0f;
  float innerR = maxRadius * 0.28f;

  dl->AddCircle(ImVec2(cx, cy), innerR, IM_COL32(255, 255, 255, 20), 64, 1.0f);
  dl->AddCircle(ImVec2(cx, cy), maxRadius, IM_COL32(255, 255, 255, 20), 64,
                1.0f);

  float angleStep = 2.0f * (float)M_PI / n;
  float startAngle = -(float)M_PI * 0.5f;

  for (int i = 0; i < n; ++i) {
    const auto &e = m_Elements[i];
    float angle = startAngle + i * angleStep;
    float outerR = innerR + e.currentHeight * (maxRadius - innerR) * e.scale;

    float a0 = angle + angleStep * 0.05f; // slight gap between wedges
    float a1 = angle + angleStep * 0.95f;

    // Build wedge as 4-point polygon
    ImVec2 p0(cx + innerR * cosf(a0), cy + innerR * sinf(a0));
    ImVec2 p1(cx + outerR * cosf(a0), cy + outerR * sinf(a0));
    ImVec2 p2(cx + outerR * cosf(a1), cy + outerR * sinf(a1));
    ImVec2 p3(cx + innerR * cosf(a1), cy + innerR * sinf(a1));

    ImU32 col = GetElementColor(i);

    // Glow
    if (m_Config.showGlow && e.glowIntensity > 0.01f) {
      float pulse = 0.7f + 0.3f * sinf(e.pulsePhase);
      int alpha = static_cast<int>(35.0f * e.glowIntensity * pulse);
      ImU32 glowCol = (col & 0x00FFFFFF) | (static_cast<ImU32>(alpha) << 24);
      float glowExtra = 4.0f * e.glowIntensity;
      float gOuterR = outerR + glowExtra;
      ImVec2 gp0(cx + (innerR - 1) * cosf(a0), cy + (innerR - 1) * sinf(a0));
      ImVec2 gp1(cx + gOuterR * cosf(a0), cy + gOuterR * sinf(a0));
      ImVec2 gp2(cx + gOuterR * cosf(a1), cy + gOuterR * sinf(a1));
      ImVec2 gp3(cx + (innerR - 1) * cosf(a1), cy + (innerR - 1) * sinf(a1));
      ImVec2 gpts[] = {gp0, gp1, gp2, gp3};
      dl->AddConvexPolyFilled(gpts, 4, glowCol);
    }

    ImVec2 pts[] = {p0, p1, p2, p3};
    dl->AddConvexPolyFilled(pts, 4, col);

    // Value labels (only for small arrays)
    if (m_Config.showValues && n <= 40) {
      float midAngle = (a0 + a1) * 0.5f;
      float labelR = outerR + 14.0f;
      std::string val = std::to_string(arr[i]);
      ImVec2 tsz = ImGui::CalcTextSize(val.c_str());
      ImVec2 labelPos(cx + labelR * cosf(midAngle) - tsz.x * 0.5f,
                      cy + labelR * sinf(midAngle) - tsz.y * 0.5f);
      dl->AddText(labelPos, IM_COL32(200, 200, 210, 200), val.c_str());
    }
  }

  // Center label
  if (GetTotalSteps() > 0) {
    std::string label = std::to_string(m_CurrentStepIndex + 1) + "/" +
                        std::to_string(GetTotalSteps());
    ImVec2 tsz = ImGui::CalcTextSize(label.c_str());
    dl->AddText(ImVec2(cx - tsz.x * 0.5f, cy - tsz.y * 0.5f),
                IM_COL32(200, 200, 210, 180), label.c_str());
  }
}

void Visualizer::RenderScatterPlot(const std::vector<int> &arr,
                                   const ImVec2 &origin, const ImVec2 &size) {
  ImDrawList *dl = ImGui::GetWindowDrawList();
  const int n = static_cast<int>(arr.size());
  if (n == 0) return;

  SyncElementStates(arr);

  if (m_Config.showGrid) {
    constexpr int gridLines = 5;
    for (int i = 0; i <= gridLines; ++i) {
      float y = origin.y + (size.y / gridLines) * i;
      dl->AddLine(ImVec2(origin.x, y), ImVec2(origin.x + size.x, y),
                  IM_COL32(255, 255, 255, 18), 1.0f);
    }
  }

  float maxH = size.y - 24.0f;
  float xStep = size.x / n;
  float dotRadius = std::min(xStep * 0.4f, 8.0f);
  if (dotRadius < 1.5f) dotRadius = 1.5f;

  for (int i = 0; i < n; ++i) {
    const auto &e = m_Elements[i];
    float h = e.currentHeight * maxH;
    ImVec2 center(origin.x + i * xStep + xStep * 0.5f, origin.y + size.y - h);

    ImU32 col = GetElementColor(i);
    float r = dotRadius * e.scale;

    // Outer glow for active elements
    if (m_Config.showGlow && e.glowIntensity > 0.01f) {
      float pulse = 0.7f + 0.3f * sinf(e.pulsePhase);
      int alpha = static_cast<int>(120.0f * e.glowIntensity * pulse);
      dl->AddCircleFilled(center, r * 3.0f,
                          (col & 0x00FFFFFF) | (static_cast<ImU32>(alpha) << 24));
    }

    dl->AddCircleFilled(center, r, col);
    dl->AddCircle(center, r * 1.2f, IM_COL32(255, 255, 255, 40), 0, 1.0f);
  }
}

void Visualizer::RenderColorSpectrum(const std::vector<int> &arr,
                                     const ImVec2 &origin, const ImVec2 &size) {
  ImDrawList *dl = ImGui::GetWindowDrawList();
  const int n = static_cast<int>(arr.size());
  if (n == 0) return;

  SyncElementStates(arr);

  float barWidth = size.x / n;
  
  for (int i = 0; i < n; ++i) {
    const auto &e = m_Elements[i];
    
    // Map height (0.0 to 1.0) to a hue (0.0 to 0.8 to avoid red wrapping)
    float hue = e.currentHeight * 0.8f;
    float r, g, b;
    ImGui::ColorConvertHSVtoRGB(hue, 0.85f, 0.9f, r, g, b);
    
    // If element is actively being compared/swapped, mix with white
    if (e.glowIntensity > 0.01f) {
      r += (1.0f - r) * e.glowIntensity * 0.6f;
      g += (1.0f - g) * e.glowIntensity * 0.6f;
      b += (1.0f - b) * e.glowIntensity * 0.6f;
    }
    
    ImU32 col = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 255);

    float x = origin.x + i * barWidth;
    ImVec2 barMin(x, origin.y);
    ImVec2 barMax(x + barWidth + 1.0f, origin.y + size.y); // +1.0f to overlap gaps

    dl->AddRectFilled(barMin, barMax, col);

    if (m_Config.showGlow && e.glowIntensity > 0.01f) {
      float pulse = 0.7f + 0.3f * sinf(e.pulsePhase);
      int alpha = static_cast<int>(60.0f * e.glowIntensity * pulse);
      ImU32 glowCol = IM_COL32(255, 255, 255, alpha);
      dl->AddRectFilled(ImVec2(barMin.x - 2.0f, barMin.y), 
                        ImVec2(barMax.x + 2.0f, barMax.y), glowCol);
    }
  }
}

void Visualizer::RenderStarburst(const std::vector<int> &arr,
                                 const ImVec2 &origin, const ImVec2 &size) {
  ImDrawList *dl = ImGui::GetWindowDrawList();
  const int n = static_cast<int>(arr.size());
  if (n == 0) return;

  SyncElementStates(arr);

  float cx = origin.x + size.x * 0.5f;
  float cy = origin.y + size.y * 0.5f;
  float maxRadius = std::min(size.x, size.y) * 0.5f - 20.0f;
  float innerR = maxRadius * 0.15f;

  dl->AddCircle(ImVec2(cx, cy), innerR, IM_COL32(255, 255, 255, 20), 64, 1.0f);

  float angleStep = 2.0f * (float)M_PI / n;
  float startAngle = -(float)M_PI * 0.5f;

  for (int i = 0; i < n; ++i) {
    const auto &e = m_Elements[i];
    float angle = startAngle + i * angleStep;
    float outerR = innerR + e.currentHeight * (maxRadius - innerR) * e.scale;

    ImVec2 p0(cx + innerR * cosf(angle), cy + innerR * sinf(angle));
    ImVec2 p1(cx + outerR * cosf(angle), cy + outerR * sinf(angle));

    ImU32 col = GetElementColor(i);
    float thickness = std::max(1.0f, (2.0f * (float)M_PI * outerR / n) * 0.6f);

    if (m_Config.showGlow && e.glowIntensity > 0.01f) {
      float pulse = 0.7f + 0.3f * sinf(e.pulsePhase);
      int alpha = static_cast<int>(80.0f * e.glowIntensity * pulse);
      ImU32 glowCol = (col & 0x00FFFFFF) | (static_cast<ImU32>(alpha) << 24);
      dl->AddLine(p0, p1, glowCol, thickness * 3.0f);
    }

    dl->AddLine(p0, p1, col, thickness);
  }

  if (GetTotalSteps() > 0) {
    std::string label = std::to_string(m_CurrentStepIndex + 1);
    ImVec2 tsz = ImGui::CalcTextSize(label.c_str());
    dl->AddText(ImVec2(cx - tsz.x * 0.5f, cy - tsz.y * 0.5f),
                IM_COL32(200, 200, 210, 180), label.c_str());
  }
}

void Visualizer::RenderPyramid(const std::vector<int> &arr,
                               const ImVec2 &origin, const ImVec2 &size) {
  ImDrawList *dl = ImGui::GetWindowDrawList();
  const int n = static_cast<int>(arr.size());
  if (n == 0) return;

  SyncElementStates(arr);

  float cy = origin.y + size.y * 0.5f;

  if (m_Config.showGrid) {
    dl->AddLine(ImVec2(origin.x, cy), ImVec2(origin.x + size.x, cy),
                IM_COL32(255, 255, 255, 40), 1.0f);
  }

  float barWidth = (size.x - 4.0f) / n;
  float spacing = std::max(1.0f, barWidth * 0.12f);
  float actualW = barWidth - spacing;
  if (actualW < 1.0f) actualW = 1.0f;
  
  float maxH = (size.y * 0.5f) - 10.0f;

  for (int i = 0; i < n; ++i) {
    const auto &e = m_Elements[i];

    float h = e.currentHeight * maxH * e.scale;
    float x = origin.x + i * barWidth + spacing * 0.5f;

    ImVec2 barMin(x, cy - h);
    ImVec2 barMax(x + actualW, cy + h);

    ImU32 col = GetElementColor(i);

    if (m_Config.showGlow && e.glowIntensity > 0.01f) {
      float pulse = 0.7f + 0.3f * sinf(e.pulsePhase);
      int glowAlpha = static_cast<int>(40.0f * e.glowIntensity * pulse);
      ImU32 glowCol = (col & 0x00FFFFFF) | (static_cast<ImU32>(glowAlpha) << 24);
      float expand = 4.0f * e.glowIntensity;
      dl->AddRectFilled(ImVec2(barMin.x - expand, barMin.y - expand),
                        ImVec2(barMax.x + expand, barMax.y + expand), glowCol, 4.0f);
    }

    dl->AddRectFilled(barMin, barMax, col, (m_Config.barStyle == BarStyle::Rounded) ? (actualW * 0.25f) : 0.0f);
  }
}
