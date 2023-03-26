#include "Solver.h"
#include <numeric>
#include <chrono>
#include <ctime>
#include <algorithm>

//Solver::Solver() : rng(std::random_device()()) {}

int Solver::cost(int solution[], int** matrix, int data_size) {
    int cost = 0;
    for (int i = 0; i < data_size-1; i++) {
        cost += matrix[solution[i]][solution[i+1]];
    }
    cost += matrix[solution[data_size-1]][solution[0]];
    return cost;
}

int* Solver::shuffle(int n, std::mt19937 rng) {
    int *nums = new int[n];
    std::iota(nums, nums + n, 0);
    for (int i = n-1; i >=0; --i) {
        std::uniform_int_distribution<> dis(0, i);
        int j = dis(rng);
        std::swap(nums[i], nums[j]);
    }
    return nums;
}

void Solver::random_move(int solution[], std::mt19937 rng, std::uniform_int_distribution<int> dis1,
                                        std::uniform_int_distribution<int> dis2, int n) {
    int i = dis1(rng);
    int j = dis2(rng);
    if (i == j) i = n;
    if (std::rand() % 2)
        std::swap(solution[i], solution[j]); //TODO: can we use this or implement our own swap?
    else {
        std::reverse(solution + i, solution + j + 1); //TODO: can we use this or implement our own reverse?
    }
}

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

int* Solver::random(Instance *instance, int running_time) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::srand(std::time(nullptr)); //idk if its ok for a coin toss, ctime is simpler so faster??
    int n = instance->get_size()-1;
    int* solution = shuffle(instance->get_size(), rng);
    int iter = 0;
    // is this the most efficient way to do it? (initialized once instead of every iteration)
    std::uniform_int_distribution<int> dis1(0, n);
    std::uniform_int_distribution<int> dis2(0, n-1);
    std::cout << "begin" << std::endl;
    auto endTime = std::chrono::steady_clock::now() + std::chrono::microseconds(running_time);
    while (std::chrono::steady_clock::now() < endTime) {
        random_move(solution, rng, dis1, dis2, n);
        iter++;
    }
    std::cout << "iterations: " << iter << std::endl;
    return solution;
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