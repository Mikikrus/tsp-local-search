#pragma once
#include "Instance.h"
#include <random>

class Solver
{
public:
    static int cost(int solution[], int** matrix, int data_size);
    static int* greedy(Instance* instance, int start);
    static int* random(Instance* instance, int running_time);
    static int* steepest(Instance* instance);
private:
    static int* node_exchange(int solution[], int** matrix, int data_size, bool is_greedy);
    static int* shuffle(int n, std::mt19937 rng);
    static void random_move(int solution[], std::mt19937 rng, std::uniform_int_distribution<int> dis1,
                                           std::uniform_int_distribution<int> dis2, int n);
    static int get_closest_node(int current_node,int available_nodes_size, int* available_nodes, int** matrix);
    static int* get_available_nodes(int current_node, int data_size);
};