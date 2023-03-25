#include "Solver.h"

int Solver::get_closest_node(int current_node, int *available_nodes, int** matrix) {
    int min = matrix[current_node][available_nodes[0]];
    int closest_node = 0;

    //find the closes node which is different from current node
    for (int i = 1; i < matrix[current_node][0]; i++) {
        if (matrix[current_node][available_nodes[i]] < min && available_nodes[i] != current_node) {
            min = matrix[current_node][available_nodes[i]];
            closest_node = i;
        }
    }
    return closest_node;
}
int* Solver::get_available_nodes(int current_node, int data_size){
    int index = 0;
    //available nodes are all nodes except current node
    int* available_nodes = new int[data_size -1];
    for (int i = 0; i < data_size; i++) {
        if (i == current_node) continue;
        available_nodes[index] = i;
        index++;
    }
    return available_nodes;
}

//int* Solver::greedy(Instance *instance, int start) {
//    int* solution = new int[instance->get_size()];
//    int** matrix = instance->get_matrix();
//
//    int* available_nodes = get_available_nodes(start, instance->get_size());
//
//    //initialize solution
//    current_node = start;
//
//
//    //find the closest node to start
////    int closest_node = get_closest_node(start, available_nodes, matrix);
//
////    //add the closest node to solution
////    solution[1] = available_nodes[closest_node];
////
////    //remove the closest node from available nodes
////    available_nodes[closest_node] = available_nodes[instance->get_size() - 1];
////
////    //find the closest node to the last added node
////    for (int i = 2; i < instance->get_size(); i++) {
////        closest_node = get_closest_node(solution[i - 1], available_nodes, matrix);
////        solution[i] = available_nodes[closest_node];
////        available_nodes[closest_node] = available_nodes[instance->get_size() - i];
////    }
//
//    return solution;
//}
