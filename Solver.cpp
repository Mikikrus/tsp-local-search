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

int Solver::get_closest_node(int current_node,int available_nodes_size, int *available_nodes, int** matrix) {
    int min = matrix[current_node][available_nodes[0]];
    int closest_node = 0;

    //find the closes node which is different from current node
    for (int i = 1; i < available_nodes_size; i++) {
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

int* Solver::nearest_neighbour(Instance *instance, int start) {
    int* solution = new int[instance->get_size()];
    int** matrix = instance->get_matrix();

    int* available_nodes = get_available_nodes(start, instance->get_size());
    int available_nodes_size = instance->get_size() - 1;
    //initialize solution
    solution[0] = start;
    int current_node = start;

    //start from one because we already have the starting node
    for (int i = 1; i < instance->get_size(); i++) {
        current_node = get_closest_node(current_node,available_nodes_size, available_nodes, matrix);
        //add current_node to solution
        solution[i] = available_nodes[current_node];
        //decrease the available nodes size and swap the last element with the current node
        available_nodes_size--;
        std::swap(available_nodes[current_node], available_nodes[available_nodes_size]);
    }

    return solution;
}

//int* Solver::two_opt(int* solution, int** matrix, int data_size) {
//    int best_cost = Solver::cost(solution, matrix, data_size);
//    int new_cost;
//    int* new_solution = new int[data_size];
//    for (int i = 0; i < data_size; i++) {
//        for (int j = i+1; j < data_size; j++) {
//            std::copy(solution, solution + data_size, new_solution);
//            std::reverse(new_solution + i, new_solution + j + 1);
//            new_cost = Solver::cost(new_solution, matrix, data_size);
//            if (new_cost < best_cost) {
//                best_cost = new_cost;
//                std::copy(new_solution, new_solution + data_size, solution);
//            }
//        }
//    }
//    return solution;
//}

int* Solver::steepest(Instance *instance){
    int iterations = 0;
    //create a random solution using Solver::random
    std::random_device rd;
    std::mt19937 rng(rd());
    int n = instance->get_size()-1;
    int* solution = shuffle(instance->get_size(), rng);
    int** matrix = instance->get_matrix();
    int best_delta, best_i, best_j;
    bool edge;
    do {
        iterations++;
        best_delta = -1;
        edge = false;
        for (int i = 0; i < instance->get_size(); i++) {
            for (int j = i + 2; j < instance->get_size() - 1; j++) {
                // node exchange delta
                int delta_current = matrix[solution[i]][solution[(i + 1) % instance->get_size()]]
                                    + matrix[solution[i - 1 + instance->get_size()] % instance->get_size()][solution[i]]
                                    + matrix[solution[j]][solution[(j + 1) % instance->get_size()]]
                                    + matrix[solution[j - 1 + instance->get_size()] % instance->get_size()][solution[j]];

                int delta_new = matrix[solution[i]][solution[(j + 1) % instance->get_size()]]
                                + matrix[solution[(j - 1 + instance->get_size()) % instance->get_size()]][solution[i]]
                                + matrix[solution[j]][solution[(i + 1) % instance->get_size()]]
                                + matrix[solution[i - 1 + instance->get_size()] % instance->get_size()][solution[j]];
                int improvement = delta_current - delta_new;
                // edge exchange delta
                delta_current = matrix[solution[(i - 1 + instance->get_size()) % instance->get_size()]][solution[i]]
                                + matrix[solution[(j - 1 + instance->get_size()) % instance->get_size()]][solution[j]];
                delta_new = matrix[solution[i]][solution[j]]
                            + matrix[solution[(i - 1 + instance->get_size()) % instance->get_size()]][solution[
                        (j - 1 + instance->get_size()) % instance->get_size()]];
                if (delta_current - delta_new > improvement) {
                    improvement = delta_current - delta_new;
                    edge = true;
                }
                if (improvement > best_delta) {
                    best_delta = improvement;
                    best_i = i;
                    best_j = j;
                }


            }
        }
        // special case: neighbors
        // maybe i=0 outside?
        for (int i = 0; i < instance->get_size() - 1; i++) {
            int delta_current = matrix[solution[i - 1 + instance->get_size()] % instance->get_size()][solution[i]]
                                + matrix[solution[i + 1]][solution[i + 2]% instance->get_size()];
            int delta_new = matrix[solution[i + 2]% instance->get_size()][solution[i]]
                            + matrix[solution[i - 1] % instance->get_size()][solution[i + 1]];
            int improvement = delta_current - delta_new;
            if (improvement > best_delta) {
                best_delta = improvement;
                best_i = i;
                best_j = i + 1;
                edge = false;
            }
        }
        // special case: last node in list with every other except neighbors
        int j = instance->get_size() - 1;
        int j_with_neighbors_delta = matrix[solution[j]][solution[(j + 1) % instance->get_size()]]
                                     +
                                     matrix[solution[j - 1 + instance->get_size()] % instance->get_size()][solution[j]];
        for (int i = 0; i < instance->get_size() - 2; i++) {
            int delta_current = matrix[solution[i]][solution[(i + 1) % instance->get_size()]]
                                + matrix[solution[i - 1 + instance->get_size()] % instance->get_size()][solution[i]]
                                + j_with_neighbors_delta;

            int delta_new = matrix[solution[i]][solution[(j + 1) % instance->get_size()]]
                            + matrix[solution[(j - 1 + instance->get_size()) % instance->get_size()]][solution[i]]
                            + matrix[solution[j]][solution[(i + 1) % instance->get_size()]]
                            + matrix[solution[i - 1 + instance->get_size()] % instance->get_size()][solution[j]];
            int improvement = delta_current - delta_new;
            if (improvement > best_delta) {
                best_delta = improvement;
                best_i = i;
                best_j = 0;
                edge = false;
            }
        }
        //
        //TODO: special cases for edge exchange
        //

        // make moves
        if (best_delta > 0) {
            if (edge) {
                std::swap(solution[best_i], solution[best_j]);
            } else {
                std::reverse(solution + best_i, solution + best_j + 1);
            }
        }
    } while (best_delta > 0 && iterations < 100);
    return solution;
}
// kod pythonowy do tego:
//            a = order[move[1]-1]
//            b = order[move[2]-1]
//            old = distances[a][order[move[1]]] + distances[b][order[move[2]]]
//            new = distances[a][b] + distances[order[move[1]]][order[move[2]]]
//            if old - new > 0:
//            out.append([old - new] + move)


//            std::swap(solution[i], solution[j]);
//            std::reverse(solution + i, solution + j + 1);

// python do special case edge exchange:
//    if not best_move[0]:
//    order[best_move[1]] = best_move[2]
//    else:
//    ind = sorted([best_move[1], best_move[2]])
//    if ind[0] == 0:
//    order = order[ind[1]-1::-1] + order[ind[1]:]
//    else:
//    order = order[:ind[0]] + order[ind[1]-1:ind[0]-1:-1] + order[ind[1]:]