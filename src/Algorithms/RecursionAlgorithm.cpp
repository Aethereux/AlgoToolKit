#include "RecursionAlgorithm.h"
#include <iostream>

int Factorial::Factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    int result = n * Factorial(n - 1);
    
    return result;
}

int RecursionAlgorithm::Fibonacci(int n){
    if(n <= 1){
        return n;
    }

    return Fibonacci(n - 1) + Fibonacci(n - 2);
}

void RecursionAlgorithm::TowerOfHanoi(int numDisks, char source, char auxiliary, char destination){
    if(numDisks <= 0){
        return;
    }

    TowerOfHanoi(numDisks - 1, source, destination, auxiliary);
    std::cout << "Move disk " << numDisks << " from " << source << " to " << destination << std::endl;

    TowerOfHanoi(numDisks - 1, auxiliary, source, destination);
}