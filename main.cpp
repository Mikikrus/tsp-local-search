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
    int number_of_runs = 10;
    string data_dir = "/Users/maciej.filanowicz/tsp-local-search/data/";

    for (auto & instance_name : instance_names) {
        cout << "Instance: " << instance_name << endl;
        auto *instance = new Instance(data_dir + std::string(instance_name), true);
        auto *solution_writer = new SolutionWriter(instance, data_dir,"greedy", number_of_runs);
        for (int j =0; j < number_of_runs; j++) {
            auto startTime = std::chrono::steady_clock::now();
            int* solution = Solver::greedy(instance,solution_writer);
//            cout << "Max value: " << max_value << " Number of cities: " <<  instance->get_size()<<endl;
            auto endTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds >(endTime - startTime).count();
            solution_writer->append_solution(solution,elapsedTime,"final_order", false);
        }
        solution_writer->print_summary();
        solution_writer->write_solution();
    }
    return 0;
}
