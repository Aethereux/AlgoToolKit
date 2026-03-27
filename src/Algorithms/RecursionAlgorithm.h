#pragma once
#include <string>
#include <vector>

// TODO: GROUP MEMBER TASK - Phase 3 (Recursion Simulation)
// Requirements from PDF:
// - Demonstrate how recursion works by simulating recursive calls
// - Must display recursive calls, show base cases, and show return values

class RecursionAlgorithm {
public:
    virtual ~RecursionAlgorithm() = default;

    // TODO: Implement Factorial simulation tracking call stack and base cases
    // Example Output: factorial(4) -> 4 * factorial(3) ... -> Result = 24
    virtual int Factorial(int n) = 0;

    // TODO: Implement Fibonacci simulation tracking overlapping subproblems
    virtual int Fibonacci(int n) = 0;

    // TODO: Implement Tower of Hanoi tracking disk movements
    // Example Output: Move disk 1 from A to C
    virtual void TowerOfHanoi(int numDisks, char source, char auxiliary, char destination) = 0;
};
