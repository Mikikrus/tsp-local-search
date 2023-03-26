#include <iostream>
#include "Instance.h"
#include "Solver.h"
#include <chrono>
#include <thread>
#include <algorithm>


int main() {
    std::cout << "Hello, World!" << std::endl;
    auto startTime = std::chrono::steady_clock::now();
    int a = 0;
//    for (int i = 0; i <1; i++) {
//        std::this_thread::sleep_for(std::chrono::seconds(1));
//    }
//    auto endTime = std::chrono::steady_clock::now();
//    auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
//
//    std::cout << "Function took " << elapsedTime << " microseconds to execute." << std::endl;
//    Instance *x = new Instance("/Users/maciej.filanowicz/tsp-local-search/data/att48.tsp",true);
    Instance *x = new Instance("/home/mikikrus/CLionProjects/tsp-local-search/data/att48.tsp",true);
//    x->print_matrix();

//    int *y = Solver::random(x, elapsedTime);
    std::cout << "begin stepeest" << std::endl;
    int *y = Solver::steepest(x);
    for (int i = 0; i < x->get_size(); i++) {
        std::cout << y[i] << " ";
    }
    std::cout << std::endl << "Cost: " << Solver::cost(y, x->get_matrix(), x->get_size()) << std::endl;
    std::cout << "Optimal cost: " << x->optimal_tour_length << std::endl; //TODO: load/calculate optimal tour length
    return 0;
}
