#include "RecursionAlgorithm.h"

int Factorial::Factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    int result = n * Factorial(n - 1);
    
    return result;
}