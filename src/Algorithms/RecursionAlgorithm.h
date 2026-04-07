#pragma once
#include <string>
#include <utility>
#include <vector>
#include "../Visualizer/Visualizer.h"

// Recursion simulation contracts used by the visualizer tabs.
class RecursionAlgorithm {
public:
    RecursionAlgorithm(Visualizer* visualizer) : m_Visualizer(visualizer) {}
    virtual ~RecursionAlgorithm() = default;

    // Factorial recursion entry point.
    virtual int Factorial(int n) = 0;
    // Fibonacci recursion entry point.
    virtual int Fibonacci(int n) = 0;
    // Tower of Hanoi recursion entry point.
    virtual void TowerOfHanoi(int numDisks, char source, char auxiliary, char destination) = 0;

protected:
    Visualizer* m_Visualizer;
};

class FactorialAlgorithm : public RecursionAlgorithm {
public:
    using RecursionAlgorithm::RecursionAlgorithm;

    // Emits visual steps while computing factorial recursively.
    int Factorial(int n) override;
    int Fibonacci(int n) override { return 0; }
    void TowerOfHanoi(int numDisks, char source, char auxiliary, char destination) override {}

private:
    std::vector<int> m_Stack;
};

class Recursion : public RecursionAlgorithm {
public:
    Recursion() : RecursionAlgorithm(nullptr) {}

    int Factorial(int n) override;
    int Fibonacci(int n) override;
    void TowerOfHanoi(int numDisks, char source, char auxiliary, char destination) override;

    // Exposes the captured Tower of Hanoi move list for animation.
    const std::vector<std::pair<char, char>>& GetTowerMoves() const;
    // Exposes a call/return trace for the side panel.
    const std::vector<std::string>& GetTowerTrace() const;

private:
    void TowerOfHanoiTracked(int numDisks, char source, char auxiliary, char destination);
    void AddTowerTraceLine(const std::string& line);
    std::vector<std::pair<char, char>> m_TowerMoves;
    std::vector<std::string> m_TowerTrace;
};
