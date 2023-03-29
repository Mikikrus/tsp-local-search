#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

#include "Instance.h"
#include "Solver.h"
#include "SolutionWriter.h"



int main() {
    std::cout << "Hello, World!" << std::endl;

    const char *instance_names[8] = {
            "eil51.tsp",
            "eil76.tsp",
            "kroA100.tsp",
            "kroC100.tsp",
            "kroD100.tsp",
            "pr76.tsp",
            "tsp225.tsp",
            "st70.tsp"
    };
    double running_times [8] = {21.8,
    80.45,
    232.55,
    226.6,
    233.35,
    95.2,
    3413.25,
    63.45};
    int number_of_runs = 10;
    string data_dir = "/Users/maciej.filanowicz/tsp-local-search/data/";
    string destination_dir =  "/Users/maciej.filanowicz/tsp-local-search/predictions/";

    for (int i = 0; i < 8; i++) {
        cout << "Instance: " << instance_names[i] << endl;
        auto *instance = new Instance(data_dir + std::string(instance_names[i]), true);
        auto *solution_writer = new SolutionWriter(instance,data_dir, destination_dir,"greedy", number_of_runs);
        for (int j =0; j < number_of_runs; j++) {
            int iterations;
            int* solution;
            auto startTime = std::chrono::steady_clock::now();
            std::tie(solution, iterations) = Solver::greedy(instance,solution_writer);
//            std::tie(solution, iterations) = Solver::greedy(instance,solution_writer);
//            std::tie(solution, iterations) = Solver::random_search(instance,running_times[i],solution_writer);
//            std::tie(solution, iterations) = Solver::random_walk(instance,running_times[i],solution_writer);
            auto endTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds >(endTime - startTime).count();
            solution_writer->append_solution(solution,elapsedTime,"final_order", false,iterations);
        }
        solution_writer->print_summary();
        solution_writer->write_solution();
    }
    return 0;
}
