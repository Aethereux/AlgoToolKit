#pragma once
#include <vector>
#include <string>
#include "../Visualizer/Visualizer.h"

class SortingAlgorithm {
public:
    SortingAlgorithm(Visualizer* visualizer) : m_Visualizer(visualizer) {}
    virtual ~SortingAlgorithm() = default;
    
    virtual void Sort(std::vector<int>& array) = 0;
    virtual std::string GetName() const = 0;
    virtual std::string GetTimeComplexity() const = 0;
    virtual std::string GetSpaceComplexity() const = 0;

protected:
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
    std::string GetTimeComplexity() const override { return "O(n^2)"; }
    std::string GetSpaceComplexity() const override { return "O(1)"; }
};

class SelectionSort : public SortingAlgorithm {
public:
    using SortingAlgorithm::SortingAlgorithm;
    
    void Sort(std::vector<int>& array) override;
    std::string GetName() const override { return "Selection Sort"; }
    std::string GetTimeComplexity() const override { return "O(n^2)"; }
    std::string GetSpaceComplexity() const override { return "O(1)"; }
};

class InsertionSort : public SortingAlgorithm {
public:
    using SortingAlgorithm::SortingAlgorithm;
    
    void Sort(std::vector<int>& array) override;
    std::string GetName() const override { return "Insertion Sort"; }
    std::string GetTimeComplexity() const override { return "O(n^2)"; }
    std::string GetSpaceComplexity() const override { return "O(1)"; }
};
