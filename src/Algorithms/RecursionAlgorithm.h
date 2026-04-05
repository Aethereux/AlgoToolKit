#pragma once
#include <string>
#include <utility>
#include <vector>
#include "../Visualizer/Visualizer.h"

class RecursionAlgorithm {
public:
    RecursionAlgorithm(Visualizer* visualizer) : m_Visualizer(visualizer) {}
    virtual ~RecursionAlgorithm() = default;

    virtual int Factorial(int n) = 0;
    virtual int Fibonacci(int n) = 0;
    virtual void TowerOfHanoi(int numDisks, char source, char auxiliary, char destination) = 0;

protected:
    Visualizer* m_Visualizer;
};

class FactorialAlgorithm : public RecursionAlgorithm {
public:
    using RecursionAlgorithm::RecursionAlgorithm;

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

    const std::vector<std::pair<char, char>>& GetTowerMoves() const;
    const std::vector<std::string>& GetTowerTrace() const;

private:
    void TowerOfHanoiTracked(int numDisks, char source, char auxiliary, char destination);
    void AddTowerTraceLine(const std::string& line);
    std::vector<std::pair<char, char>> m_TowerMoves;
    std::vector<std::string> m_TowerTrace;
};
