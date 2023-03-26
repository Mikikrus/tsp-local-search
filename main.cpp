#include <iostream>
#include "Instance.h"
#include "Solver.h"
#include "SolutionWriter.h"
#include <chrono>
#include <thread>
#include <algorithm>


int main() {
    std::cout << "Hello, World!" << std::endl;
    int number_of_runs = 10;
    string data_dir = "/Users/maciej.filanowicz/tsp-local-search/data/";
    string dataset_name = "att48.tsp";
    auto startTime = std::chrono::steady_clock::now();
    for (int i = 0; i <2; i++) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    auto endTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    std::cout << "Function took " << elapsedTime << " microseconds to execute." << std::endl;
    Instance *instance = new Instance(data_dir+dataset_name,true);
    SolutionWriter *solution_writer = new SolutionWriter(instance, data_dir,"random", number_of_runs);

    for (int run_id=0;run_id<number_of_runs;run_id++){
        int *solution = Solver::random(instance, elapsedTime);
        solution_writer->append_solution(solution,elapsedTime);
    }
    solution_writer->print_summary();
    solution_writer->write_solution();
    return 0;
}
