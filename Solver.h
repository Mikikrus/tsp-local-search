#pragma once
#include "Instance.h"
#include <random>
//template<typename T, std::size_t N, std::size_t M>
class Solver {
public:
    static int cost(int solution[], int** matrix, size_t data_size);
    static int* random(Instance* instance, int running_time);
    static int* greedy(Instance* instance);
    static int* steepest(Instance* instance);
private:
    static int* node_exchange(int solution[], int** matrix, size_t data_size, bool is_greedy);
    static int* shuffle(size_t n, std::mt19937 rng);
    static void shuffle(int combinations[][3], int total_count, std::mt19937 rng);
    static void random_move(int solution[], std::mt19937 rng, std::uniform_int_distribution<int> dis1,
                                           std::uniform_int_distribution<int> dis2, size_t n);
    static void calculate_deltas_node(const int solution[], int** matrix, int current_best[], int i, int j, size_t n);
    static void calculate_special_deltas(const int solution[], int** matrix, int current_best[], int i, size_t n);
    static void calculate_deltas_edge(const int solution[], int** matrix, int current_best[], int i, int j, size_t n);
    static int get_closest_node(int current_node,int available_nodes_size, int* available_nodes, int** matrix);
    static int* get_available_nodes(int current_node, int data_size);
};