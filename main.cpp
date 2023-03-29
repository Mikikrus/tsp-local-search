#include <iostream>
#include "Instance.h"
#include "Solver.h"
#include <chrono>
#include <thread>
#include <algorithm>


int main() {
    std::cout << "Hello, World!" << std::endl;
//    void shuffle(int combinations[][3], int total_count, std::mt19937 rng) {
//        for (int i = total_count-1; i >=0; --i) {
//            std::uniform_int_distribution<> dis(0, i);
//            int j = dis(rng);
//            std::swap(combinations[i], combinations[j]);
//        }
//    }


//    mt19937 rng = get_rng();
//    int x=0;
//    while (x < 99) {
//        int* solution = shuffle(instance->get_size(), rng);
//        cout << Solver::cost(solution, instance->get_matrix(), instance->get_size()) << endl;
//        x++;
//    }
//    cout << "====" << endl;
    int a = 0;
    auto startTime = std::chrono::steady_clock::now();
    for (int i = 0; i <1; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds (10));
    }
    auto endTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
//
//    std::cout << "Function took " << elapsedTime << " microseconds to execute." << std::endl;
//    Instance *x = new Instance("/Users/maciej.filanowicz/tsp-local-search/data/att48.tsp",true);
    Instance *x = new Instance("/home/mikikrus/CLionProjects/tsp-local-search/data/rd100.tsp",true);
//    x->print_matrix();
    int* y = Solver::random_walk(x, elapsedTime);
//    int* y = Solver::not_random_search(x, elapsedTime);

//    int* y = Solver::greedy(x);
//    auto startTime = std::chrono::steady_clock::now();
//    for (int i = 0; i <10; i++) {
//        y = Solver::steepest(x);
//    }
//    auto endTime = std::chrono::steady_clock::now();
//    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds >(endTime - startTime).count() / 10;
//    cout << "Average Time: " << elapsedTime << " miliseconds" <<endl;

    for (int i = 0; i < x->get_size(); i++) {
        std::cout << y[i] << " ";
    }
    std::cout << std::endl << "Cost: " << Solver::cost(y, x->get_matrix(), x->get_size()) << std::endl;
    std::cout << "Optimal cost: " << x->optimal_tour_length << std::endl; //TODO: load/calculate optimal tour length
    return 0;
}
