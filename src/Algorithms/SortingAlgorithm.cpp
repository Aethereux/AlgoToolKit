#include "SortingAlgorithm.h"

void SortingAlgorithm::Compare(int idx1, int idx2,
                               const std::vector<int> &array) {
  // Emits a comparison step without mutating the array.
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
  // Swaps two values and records the post-swap snapshot.
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
  // Writes a value at index and records that write operation.
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
  // Marks an index as finalized in the visual timeline.
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
  // Bubble Sort: repeatedly move the largest unsorted element to the end.
  int n = array.size();
  for (int i = 0; i < n - 1; i++) {
    bool swapped = false;
    for (int j = 0; j < n - i - 1; j++) {
      Compare(j, j + 1, array);
      if (array[j] > array[j + 1]) {
        Swap(j, j + 1, array);
        swapped = true;
      }
    }
    if (!swapped)
      break;
    MarkSorted(n - i - 1, array);
  }
  MarkSorted(0, array);
}

void SelectionSort::Sort(std::vector<int> &array) {
  // Selection Sort: select the minimum from the unsorted suffix each pass.
  int n = array.size();
  for (int i = 0; i < n; i++) {
    int minIndex = i;

    for (int j = i + 1; j < n; j++) {
      Compare(j, minIndex, array);
      if (array[j] < array[minIndex]) {
        minIndex = j;
      }
    }

    if (minIndex != i) {
      Swap(i, minIndex, array);
    }

    MarkSorted(i, array);
  }
}

void InsertionSort::Sort(std::vector<int> &array) {
  // Insertion Sort: shift larger values right and insert current element.
  int n = array.size();
  for (int i = 1; i < n; i++) {
    int element = array[i];
    int j = i;

    while (j > 0) {
      Compare(j - 1, i, array);

      if (array[j - 1] > element) {
        Overwrite(j, array[j - 1], array);
        j--;
      } else {
        break;
      }
    }
    Overwrite(j, element, array);
    MarkSorted(i, array);
  }
}
