#include "RecursionAlgorithm.h"
#include <iostream>

int Recursion::Factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    int result = n * Factorial(n - 1);
    
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