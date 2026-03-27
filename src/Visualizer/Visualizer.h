#pragma once

#include "imgui.h"
#include <string>
#include <vector>

enum class StepType { Compare, Swap, Overwrite, Sorted, Pivot, Merge, Default };

enum class VizType {
  BarGraph,
  LineGraph,
  Circular,
  ScatterPlot,
  ColorSpectrum,
  Starburst,
  Pyramid
};

enum class ColorTheme { Cyberpunk, Ocean, Sunset, Matrix, Pastel };

enum class BarStyle { Rounded, Sharp, Gradient };

struct AlgorithmStep {
  std::vector<int> array;
  int index1 = -1;
  int index2 = -1;
  StepType type = StepType::Default;
  std::string description;
};

struct ElementState {
  float currentHeight = 0.0f;
  float targetHeight = 0.0f;

  float currentR = 0.4f, currentG = 0.6f, currentB = 1.0f, currentA = 1.0f;
  float targetR = 0.4f, targetG = 0.6f, targetB = 1.0f, targetA = 1.0f;

  float scale = 1.0f;
  float targetScale = 1.0f;

  float glowIntensity = 0.0f;
  float targetGlow = 0.0f;

  float pulsePhase = 0.0f;
};

struct VisualizerConfig {
  int arraySize = 30;
  int animationSpeed = 50;
  bool smoothAnimation = true;
  ColorTheme theme = ColorTheme::Cyberpunk;
  VizType vizType = VizType::BarGraph;
  BarStyle barStyle = BarStyle::Rounded;
  bool showValues = false;
  bool showGrid = true;
  bool highlightOps = true;
  bool showGlow = true;
  bool showConnections = true;
  float barSpacing = 2.0f;
};

class Visualizer {
public:
  Visualizer();
  ~Visualizer() = default;

  void Update();
  void Render();

  void AddStep(const AlgorithmStep &step);
  void ClearSteps();
  void GenerateRandomArray(int size = 30);
  void SetArray(const std::vector<int> &arr);

  void Play();
  void Pause();
  void StepForward();
  void StepBackward();
  void Reset();

  bool IsPlaying() const { return m_IsPlaying; }
  int GetCurrentStepIndex() const { return m_CurrentStepIndex; }
  int GetTotalSteps() const { return static_cast<int>(m_Steps.size()); }
  const std::vector<int> &GetOriginalArray() const;
  const std::vector<int> &GetCurrentArray() const;

  VisualizerConfig &GetConfig() { return m_Config; }
  const VisualizerConfig &GetConfig() const { return m_Config; }

private:
  // UI panels
  void RenderControlPanel();
  void RenderStepInfo();

  // Visualizations
  void RenderBarGraph(const std::vector<int> &arr, const ImVec2 &origin,
                      const ImVec2 &size);
  void RenderLineGraph(const std::vector<int> &arr, const ImVec2 &origin,
                       const ImVec2 &size);
  void RenderCircular(const std::vector<int> &arr, const ImVec2 &origin,
                      const ImVec2 &size);
  void RenderScatterPlot(const std::vector<int> &arr, const ImVec2 &origin,
                         const ImVec2 &size);
  void RenderColorSpectrum(const std::vector<int> &arr, const ImVec2 &origin,
                           const ImVec2 &size);
  void RenderStarburst(const std::vector<int> &arr, const ImVec2 &origin,
                       const ImVec2 &size);
  void RenderPyramid(const std::vector<int> &arr, const ImVec2 &origin,
                     const ImVec2 &size);

  // Animation helpers
  void UpdateElementStates(float dt);
  void SyncElementStates(const std::vector<int> &arr);

  // Color helpers
  ImU32 GetThemeColor(StepType type) const;
  ImU32 GetElementColor(int idx) const;
  ImVec4 GetThemeColorVec(StepType type) const;
  void LerpColor(float &r, float &g, float &b, float &a, float tr, float tg,
                 float tb, float ta, float t);

  // Easing
  static float EaseOutCubic(float t);
  static float EaseOutElastic(float t);
  static float EaseOutBounce(float t);
  static float EaseInOutQuad(float t);

  // State
  std::vector<int> m_OriginalArray;
  std::vector<AlgorithmStep> m_Steps;
  std::vector<ElementState> m_Elements;

  int m_CurrentStepIndex = -1;
  bool m_IsPlaying = false;
  float m_PlayTimer = 0.0f;
  float m_AnimationProgress = 1.0f;
  float m_GlobalTime = 0.0f;

  VisualizerConfig m_Config;
};
