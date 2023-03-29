#include "Solver.h"
#include <numeric>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <thread>

//Solver::Solver() : rng(std::random_device()()) {}
mt19937 Solver::get_rng() {
    std::random_device rd;
    if (rd.entropy() != 0) {
        // initialize some generator like mt19937 with rd()
        std::mt19937 rng(rd());
        return rng;
    }
    else {
        // use another seed generator (for example, time in milliseconds)
        std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
        return rng;
    }
}

int Solver::cost(int solution[], int** matrix, size_t data_size) {
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

void update_current_best(int current_best[], int improvement, int i, int j, bool is_edge) {
    if (improvement > current_best[0]) {
        current_best[0] = improvement;
        current_best[1] = i;
        current_best[2] = j;
        current_best[3] = is_edge;
    }
}
//template<typename T, std::size_t N, std::size_t M>
void Solver::shuffle(int combinations[][3], int total_count, std::mt19937 rng) {
    std::mt19937 rng2(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = total_count-1; i >=0; --i) {
        std::uniform_int_distribution<> dis(0, i);
        int j = dis(rng2);
        std::swap(combinations[i], combinations[j]);
    }
}

void Solver::calculate_deltas_node(const int solution[], int** matrix, int current_best[], int i, int j, size_t n) {
    int improvement;
    int delta_current = matrix[solution[i]][solution[(i + 1) % n]]
                        + matrix[solution[(i - 1 + n) % n]][solution[i]]
                        + matrix[solution[j]][solution[(j + 1) % n]]
                        + matrix[solution[(j - 1 + n) % n]][solution[j]];

    int delta_new = matrix[solution[i]][solution[(j + 1) % n]]
                    + matrix[solution[(j - 1 + n) % n]][solution[i]]
                    + matrix[solution[j]][solution[(i + 1) % n]]
                    + matrix[solution[(i - 1 + n) % n]][solution[j]];
    improvement = delta_current - delta_new;
    update_current_best(current_best, improvement, i, j, false);
}

void Solver::calculate_special_deltas(const int solution[], int** matrix, int current_best[], int i, int j,size_t n) {
    if (i == 0 && j == n-1) std::swap(i, j);
    int delta_current = matrix[solution[(i - 1 + n) % n]][solution[i]]
                        + matrix[solution[j]][solution[(j + 1)% n]];
    int delta_new = matrix[solution[(j + 1)% n]][solution[i]]
                    + matrix[solution[(i - 1 + n) % n]][solution[j]];
    int improvement = delta_current - delta_new;
    update_current_best(current_best, improvement, i, j, false);
}

void Solver::calculate_deltas_edge(const int solution[], int** matrix, int current_best[], int i, int j, size_t n) {
    int improvement;
    int i_prev = solution[(i - 1 + n) % n];
    int j_prev = solution[(j - 1 + n) % n];
    int delta_current = matrix[i_prev][solution[i]] + matrix[j_prev][solution[j]];
    int delta_new = matrix[solution[i]][solution[j]] + matrix[i_prev][j_prev];
    improvement = delta_current - delta_new;
    update_current_best(current_best, improvement, i, j, true);
}

int Solver::calculate_deltas_node(const int solution[], int** matrix, int i, int j, size_t n) {
    int delta_current = matrix[solution[i]][solution[(i + 1) % n]]
                        + matrix[solution[(i - 1 + n) % n]][solution[i]]
                        + matrix[solution[j]][solution[(j + 1) % n]]
                        + matrix[solution[(j - 1 + n) % n]][solution[j]];

    int delta_new = matrix[solution[i]][solution[(j + 1) % n]]
                    + matrix[solution[(j - 1 + n) % n]][solution[i]]
                    + matrix[solution[j]][solution[(i + 1) % n]]
                    + matrix[solution[(i - 1 + n) % n]][solution[j]];
    return delta_current - delta_new;
}

int Solver::calculate_special_deltas_random(const int *solution, int **matrix, int i, int j, size_t n) {
    if (i == 0 && j == n-1) std::swap(i, j);
    int delta_current = matrix[solution[(i - 1 + n) % n]][solution[i]]
                        + matrix[solution[j]][solution[(j + 1)% n]];
    int delta_new = matrix[solution[(j + 1)% n]][solution[i]]
                    + matrix[solution[(i - 1 + n) % n]][solution[j]];
    return delta_current - delta_new;
}

int Solver::calculate_deltas_edge(const int solution[], int** matrix, int i, int j, size_t n) {
    int i_prev = solution[(i - 1 + n) % n];
    int j_prev = solution[(j - 1 + n) % n];
    int delta_current = matrix[i_prev][solution[i]] + matrix[j_prev][solution[j]];
    int delta_new = matrix[solution[i]][solution[j]] + matrix[i_prev][j_prev];
    return delta_current - delta_new;
}

//int Solver::random_move(int solution[], std::mt19937 rng, std::uniform_int_distribution<int> dis1,
//                                        std::uniform_int_distribution<int> dis2, size_t n) {
//    int i = dis1(rng); // 0 - n (n = instance.size() -1)
//    int j = dis2(rng); // 0 - n-1
//    if (i == j) i = n;
//    if (i+1 == j) {
//        break;
//    if (std::rand() % 2)
//        std::swap(solution[i], solution[j]); //can we use this or implement our own swap?
//    else {
//        std::reverse(solution + i, solution + j); //can we use this or implement our own reverse?
//    }
//}
//TODO: what the fuck?
int* Solver::random_search(Instance *instance, int running_time) {
    mt19937 rng = get_rng();
    int score, best_score = 2147483647; //INT_MAX
    int* best_solution;
    auto endTime = std::chrono::steady_clock::now() + std::chrono::microseconds(running_time);
//    cout << Solver::cost(shuffle(instance->get_size(), rng), instance->get_matrix(), instance->get_size()) << endl;
    while (std::chrono::steady_clock::now() < endTime) {
        int* solution = shuffle(instance->get_size(), rng);
        cout << Solver::cost(solution, instance->get_matrix(), instance->get_size()) << endl;
        if (score = Solver::cost(solution, instance->get_matrix(), instance->get_size()); score < best_score) {
            best_score = score;
            best_solution = solution;
        }
    }
    return best_solution;
}

int* Solver::not_random_search(Instance *instance, int running_time) {
    mt19937 rng = get_rng();
    int x=0;
    while (x < 99) {
        int* solution = shuffle(instance->get_size(), rng);
        cout << Solver::cost(solution, instance->get_matrix(), instance->get_size()) << endl;
        x++;
    }
    cout << "====" << endl;
    return shuffle(instance->get_size(), rng);
}

int* Solver::random_walk(Instance *instance, int running_time) {
    std::srand(std::time(nullptr)); //idk if its ok for a coin toss, ctime is simpler so faster??
//    mt19937 rng = get_rng();
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    int x=0;
//    std::this_thread::sleep_for(std::chrono::milliseconds (10));

//eliminate this shit
    while (x < 99) {

        int* solution = shuffle(instance->get_size(), rng);
        cout << Solver::cost(solution, instance->get_matrix(), instance->get_size()) << endl;
        x++;
    }
    cout << "====" << endl;


//    return shuffle(instance->get_size(), rng);
    int n = instance->get_size()-1;
    int* solution = shuffle(instance->get_size(), rng);
    int cur_cost = Solver::cost(solution, instance->get_matrix(), instance->get_size());
    cout << "Initial cost: " << cur_cost << endl;
    int* best_solution = new int[instance->get_size()];
    std::copy(solution, solution + instance->get_size(), best_solution);
    int delta = 0, best_cost = cur_cost, iter = 0;
    std::uniform_int_distribution<int> dis1(0, n);
    std::uniform_int_distribution<int> dis2(0, n-1);
    auto endTime = std::chrono::steady_clock::now() + std::chrono::microseconds(running_time);
    while (std::chrono::steady_clock::now() < endTime) {

        int* test = shuffle(instance->get_size(), rng);
//        cout << Solver::cost(test, instance->get_matrix(), instance->get_size()) << endl;
//        string move = "none";
        int j = dis1(rng); // 0 - n (n = instance.size() -1)
        int i = dis2(rng); // 0 - n-1
        if (i == j) j = n;
        if (j < i) std::swap(i, j);
        if ((i+1 == j) || (i == 0 && j == n)) {
            delta = calculate_special_deltas_random(solution, instance->get_matrix(), i, j,
                                             instance->get_size());
            std::swap(solution[i], solution[j]);
//            move = "special";
        }
        else if (std::rand() % 2) {
            delta = calculate_deltas_node(solution, instance->get_matrix(), i, j,
                                          instance->get_size());
            std::swap(solution[i], solution[j]); //can we use this or implement our own swap?
//            move = "node";
        }
        else {
            delta = calculate_deltas_edge(solution, instance->get_matrix(), i, j,
                                          instance->get_size());
            std::reverse(solution + i, solution + j); //can we use this or implement our own reverse?
//            move = "edge";
        }
        cur_cost -= delta;

//        if (cur_cost != cost(solution, instance->get_matrix(), instance->get_size())) {
//            cout << i << ' ' << j << ' ' << instance->get_size() << endl;
//            cout << cur_cost << ' ' << cost(solution, instance->get_matrix(), instance->get_size()) << endl;
//            cout << "move: " << move << endl;
//            break;
//        }
        if (cur_cost < best_cost) {
            best_cost = cur_cost;
            std::copy(solution, solution + instance->get_size(), best_solution);
        }
        iter++;
    }

    std::cout << "iterations: " << iter << std::endl;
    return best_solution;
}

int* Solver::steepest(Instance *instance) {
    mt19937 rng = get_rng();
    int* solution = shuffle(instance->get_size(), rng);
    int** matrix = instance->get_matrix();
    int current_best[4];
    do {
        current_best[0] = -1;
        for (int i = 0; i < instance->get_size(); i++) {
            for (int j = i + 2; j < instance->get_size() - 1; j++) {
                calculate_deltas_node(solution, matrix, current_best, i, j, instance->get_size());
                calculate_deltas_edge(solution, matrix, current_best, i, j, instance->get_size());
            }
        }
        // special case: neighbors
        // maybe i=0 outside?
        for (int i = 0; i < instance->get_size() - 1; i++) {
            calculate_special_deltas(solution, matrix, current_best, i, i+1, instance->get_size());
        }
        // case: last node in list with every other except neighbors
        int j = instance->get_size() - 1;
        for (int i = 1; i < instance->get_size() - 2; i++) {
            calculate_deltas_node(solution, matrix, current_best, i, j, instance->get_size());
            calculate_deltas_edge(solution, matrix, current_best, i, j, instance->get_size());
        }

        // make moves
//        int old_cost = cost(solution, matrix, instance->get_size());
        if (current_best[0] > 0) {
            if (!current_best[3]) {
                std::swap(solution[current_best[1]], solution[current_best[2]]);
            } else {
                std::reverse(solution + current_best[1], solution + current_best[2]); //TODO: write our own
            }
        }
//        for(auto i : current_best) cout << i << " ";
//        cout << cost(solution, matrix, instance->get_size()) - old_cost << "\t\t";
//        for(int x=0; x<instance->get_size(); x++) cout << solution[x] << "->";
//        cout << endl;
    } while (current_best[0] > 0);
    return solution;
}

int* Solver::greedy(Instance *instance) {
    mt19937 rng = get_rng();
    int* solution = shuffle(instance->get_size(), rng);
    int** matrix = instance->get_matrix();
    size_t total_count = (instance->get_size()*(instance->get_size()-1)
                              + instance->get_size()*(instance->get_size()-3)) / 2;
    int combinations[total_count][3];
    int current_best[4];
    int count;
    do {
        count = 0;
        current_best[0] = -1;
        for (int i = 0; i < instance->get_size(); i++) {
            for (int j = i + 2; j < instance->get_size() - 1; j++) {
                int row[3] = {i, j, 0};
                std::copy(row, row+3, combinations[count++]);
                row[2] = 1;
                std::copy(row, row+3, combinations[count++]);
            }
        }
        // special case: neighbors
        for (int i = 0; i < instance->get_size(); i++) {
            int row[3] = {i, (i + 1) % instance->get_size(), 0};
            std::copy(row, row+3, combinations[count++]);
        }
        // case: last node in list with every other except neighbors
        int j = instance->get_size() - 1;
        for (int i = 1; i < instance->get_size() - 2; i++) {
            int row[3] = {i, j, 0};
            std::copy(row, row+3, combinations[count++]);
            row[2] = 1;
            std::copy(row, row+3, combinations[count++]);
        }
//        cout << count << " " << total_count << endl;
        shuffle(combinations, total_count, rng);
        for (auto& neighbor : combinations) {
            if (neighbor[2])
                calculate_deltas_edge(solution, matrix, current_best, neighbor[0], neighbor[1], instance->get_size());
            else if ((neighbor[0] + 1) % instance->get_size() == neighbor[1])
                calculate_special_deltas(solution, matrix, current_best, neighbor[0], neighbor[1], instance->get_size());
            else
                calculate_deltas_node(solution, matrix, current_best, neighbor[0], neighbor[1], instance->get_size());
            if (current_best[0] > 0) {
                if (!current_best[3]) {
                    std::swap(solution[current_best[1]], solution[current_best[2]]);
                } else {
                    std::reverse(solution + current_best[1], solution + current_best[2]);
                }
                break;
            }
        }
    } while(current_best[0] > 0);
    return solution;
}