#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "Instance.h"

using namespace std;

class SolutionWriter
{
public:
    SolutionWriter(Instance* instance, const string& data_dir, const string& dest_dir, string algorithm_name, int number_of_runs);
    static int cost(int solution[], int** matrix);
    void write_solution();
    void append_solution(int* solution,  long long elapsed_time, string run_name, bool initial_solution, long long n_iterations, int visited_solutions);
    string destination_filename;
    string dest_dir;
    Instance* instance{};
    int* get_optimal_solution(string optimal_solution_path);
    void print_summary();
    int* optimal_solution;
    string* run_names;
    int** calculated_solutions;
    long long* time_table;
    long long* number_of_iterations;
    long long* visited_solutions;
    int number_of_runs;
    int number_of_appended_solutions = 0;
};