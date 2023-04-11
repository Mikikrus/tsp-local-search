#pragma once
#include "Instance.h"
#include "SolutionWriter.h"
#include <random>
//template<typename T, std::size_t N, std::size_t M>
class Solver {
public:
    static int cost(int solution[], int** matrix, size_t data_size);
    static std::tuple<int*, int> random_search(Instance* instance, int running_time, SolutionWriter* solution_writer);
    static std::tuple<int*, int> random_walk(Instance* instance, int running_time, SolutionWriter* solution_writer);
    static std::tuple<int*, int> greedy(Instance* instance, SolutionWriter* solution_writer);
    static std::tuple<int*, int> deterministic_greedy(Instance* instance, SolutionWriter* solution_writer);
    static std::tuple<int*, int> deterministic_greedy_2(Instance* instance, SolutionWriter* solution_writer);
    static std::tuple<int*, int> steepest(Instance* instance, SolutionWriter* solution_writer);
    static std::tuple<int*, int> nearest_neighbour(Instance *instance, int start);
    static std::tuple<int*, int> simulated_annealing(Instance *instance, SolutionWriter *solution_writer);
private:
    static mt19937 get_rng();
    static int* shuffle(int n, std::mt19937& rng);
    static void shuffle(int combinations[][3], int total_count, std::mt19937 rng);
    static void calculate_deltas_node(const int solution[], int** matrix, int current_best[], int i, int j, size_t n);
    static void calculate_special_deltas(const int solution[], int** matrix, int current_best[], int i, int j, size_t n);
    static void calculate_deltas_edge(const int solution[], int** matrix, int current_best[], int i, int j, size_t n);
    static int calculate_deltas_node(const int solution[], int** matrix, int i, int j, size_t n);
    static int calculate_special_deltas_random(const int solution[], int** matrix, int i, int j, size_t n);
    static int calculate_deltas_edge(const int solution[], int** matrix, int i, int j, size_t n);
    static int get_closest_node(int current_node,int available_nodes_size, int* available_nodes, int** matrix);
    static int* get_available_nodes(int current_node, int data_size);
};