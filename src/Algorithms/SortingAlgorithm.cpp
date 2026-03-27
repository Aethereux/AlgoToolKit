#include "SortingAlgorithm.h"

void SortingAlgorithm::Compare(int idx1, int idx2,
                               const std::vector<int> &array) {
  if (m_Visualizer) {
    AlgorithmStep step;
    step.array = array;
    step.index1 = idx1;
    step.index2 = idx2;
    step.type = StepType::Compare;
    step.description = "Comparing " + std::to_string(array[idx1]) + " and " +
                       std::to_string(array[idx2]);
    m_Visualizer->AddStep(step);
  }
}

void SortingAlgorithm::Swap(int idx1, int idx2, std::vector<int> &array) {
  if (m_Visualizer) {
    std::swap(array[idx1], array[idx2]);
    AlgorithmStep step;
    step.array = array;
    step.index1 = idx1;
    step.index2 = idx2;
    step.type = StepType::Swap;
    step.description = "Swapping " + std::to_string(array[idx2]) + " and " +
                       std::to_string(array[idx1]);
    m_Visualizer->AddStep(step);
  }
}

void SortingAlgorithm::Overwrite(int idx, int value, std::vector<int> &array) {
  if (m_Visualizer) {
    array[idx] = value;
    AlgorithmStep step;
    step.array = array;
    step.index1 = idx;
    step.type = StepType::Overwrite;
    step.description =
        "Writing " + std::to_string(value) + " to index " + std::to_string(idx);
    m_Visualizer->AddStep(step);
  }
}

void SortingAlgorithm::MarkSorted(int idx, const std::vector<int> &array) {
  if (m_Visualizer) {
    AlgorithmStep step;
    step.array = array;
    step.index1 = idx;
    step.type = StepType::Sorted;
    step.description =
        "Element at index " + std::to_string(idx) + " is now sorted";
    m_Visualizer->AddStep(step);
  }
}

void BubbleSort::Sort(std::vector<int> &array) {
  int n = array.size();
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      Compare(j, j + 1, array);
      if (array[j] > array[j + 1]) {
        Swap(j, j + 1, array);
      }
    }
    MarkSorted(n - i - 1, array);
  }
  MarkSorted(0, array);
}

void SelectionSort::Sort(std::vector<int> &array) {
  // Implement Selection Sort using Visualizer API
  // Use Compare(idx1, idx2, array), Swap(idx1, idx2, array), and
  // MarkSorted(idx, array)
}

void InsertionSort::Sort(std::vector<int> &array) {
  // Implement Insertion Sort using Visualizer API
  // Use Compare(idx1, idx2, array), Overwrite(idx, val, array), and
  // MarkSorted(idx, array)
}
