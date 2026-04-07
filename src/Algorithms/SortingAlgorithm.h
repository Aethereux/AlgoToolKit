#pragma once
#include <vector>
#include <string>
#include "../Visualizer/Visualizer.h"

// Sorting simulation contracts:
// - Input: integer array from the visualizer.
// - Output: ordered AlgorithmStep snapshots for animation/replay.
// - Supported algorithms: Bubble, Selection, and Insertion.

class SortingAlgorithm {
public:
    SortingAlgorithm(Visualizer* visualizer) : m_Visualizer(visualizer) {}
    virtual ~SortingAlgorithm() = default;
    
    // Executes algorithm-specific sorting while emitting visualization steps.
    virtual void Sort(std::vector<int>& array) = 0;
    virtual std::string GetName() const = 0;

protected:
    // Step emitters used by concrete algorithms to drive the UI.
    void Compare(int idx1, int idx2, const std::vector<int>& array);
    void Swap(int idx1, int idx2, std::vector<int>& array);
    void Overwrite(int idx, int value, std::vector<int>& array);
    void MarkSorted(int idx, const std::vector<int>& array);
    
    Visualizer* m_Visualizer;
};

class BubbleSort : public SortingAlgorithm {
public:
    using SortingAlgorithm::SortingAlgorithm;
    
    void Sort(std::vector<int>& array) override;
    std::string GetName() const override { return "Bubble Sort"; }
};

class SelectionSort : public SortingAlgorithm {
public:
    using SortingAlgorithm::SortingAlgorithm;
    
    void Sort(std::vector<int>& array) override;
    std::string GetName() const override { return "Selection Sort"; }
};

class InsertionSort : public SortingAlgorithm {
public:
    using SortingAlgorithm::SortingAlgorithm;
    
    void Sort(std::vector<int>& array) override;
    std::string GetName() const override { return "Insertion Sort"; }
};
