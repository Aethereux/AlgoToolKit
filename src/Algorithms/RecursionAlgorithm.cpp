#include "RecursionAlgorithm.h"
#include <iostream>

// Update the class scope to FactorialAlgorithm
int FactorialAlgorithm::Factorial(int n) {
    if (!m_Visualizer)
        return 1;

    const bool isRootCall = m_Stack.empty();
    if (isRootCall) {
        m_Stack.clear();
    }

    m_Stack.push_back(n);

    AlgorithmStep step;
    step.array = m_Stack;
    step.type = StepType::Pivot;
    step.index1 = n;
    step.index2 = -1;
    step.description = "Call factorial(" + std::to_string(n) + ")";
    m_Visualizer->AddStep(step);

    if (n <= 1) {
        AlgorithmStep baseStep;
        baseStep.array = m_Stack;
        baseStep.type = StepType::Sorted;
        baseStep.index1 = n;
        baseStep.index2 = 1;
        baseStep.description = "Base case return 1";
        m_Visualizer->AddStep(baseStep);

        m_Stack.pop_back();
        return 1;
    }

    const int child = Factorial(n - 1);
    const int result = n * child;

    AlgorithmStep returnStep;
    returnStep.array = m_Stack;
    returnStep.type = StepType::Merge;
    returnStep.index1 = n;
    returnStep.index2 = result;
    returnStep.description =
            "Return factorial(" + std::to_string(n) + ") = " + std::to_string(result);
    m_Visualizer->AddStep(returnStep);

    m_Stack.pop_back();
    return result;
}

int Recursion::Fibonacci(int n){
    if(n <= 1){
        return n;
    }

    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

void Recursion::TowerOfHanoi(int numDisks, char source, char auxiliary, char destination){
    m_TowerMoves.clear();
    TowerOfHanoiTracked(numDisks, source, auxiliary, destination);
}

void Recursion::TowerOfHanoiTracked(int numDisks, char source, char auxiliary, char destination) {
    if(numDisks <= 0){
        return;
    }

    TowerOfHanoiTracked(numDisks - 1, source, destination, auxiliary);
    m_TowerMoves.push_back({source, destination});
   
    TowerOfHanoiTracked(numDisks - 1, auxiliary, source, destination);
}

const std::vector<std::pair<char, char>>& Recursion::GetTowerMoves() const {
    return m_TowerMoves;
}