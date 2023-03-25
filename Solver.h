#pragma once
#include "Instance.h"

class Solver
{
public:
    static int* greedy(Instance* instance, int start);
private:
    static int get_closest_node(int current_node, int* available_nodes, int** matrix);
    static int* get_available_nodes(int current_node, int data_size);
};