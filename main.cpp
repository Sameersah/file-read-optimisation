#include <iostream>
#include <chrono>

void functionToBenchmark() {
    // Your function logic
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    functionToBenchmark(); // Call the function to measure

    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = stop - start;

    std::cout << "Execution time: " << elapsed.count() << " seconds" << std::endl;
    return 0;
}
