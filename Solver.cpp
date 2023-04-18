#include "Solver.h"
#include <numeric>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <climits>
#include <cstring>

//Solver::Solver() : rng(std::random_device()()) {}
mt19937 Solver::get_rng() {
    std::random_device rd;
    if (rd.entropy() != 0) {
        std::mt19937 rng(rd());
        return rng;
    }
    else {
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

int* Solver::shuffle(int n, std::mt19937& rng) {
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
    for (int i = total_count-1; i >=0; --i) {
        std::uniform_int_distribution<> dis(0, i);
        int j = dis(rng);
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

int Solver::calculate_special_deltas(const int *solution, int **matrix, int i, int j, size_t n) {
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

std::tuple<int*, int> Solver::random_search(Instance *instance, int running_time, SolutionWriter* solution_writer) {
    mt19937 rng = get_rng();
    int c=0, score, best_score = 2147483647; //INT_MAX
    int* best_solution;
    auto endTime = std::chrono::steady_clock::now() + std::chrono::microseconds(running_time);
//    cout << Solver::cost(shuffle(instance->get_size(), rng), instance->get_matrix(), instance->get_size()) << endl;
    while (std::chrono::steady_clock::now() < endTime) {
        c++;
        int* solution = shuffle(instance->get_size(), rng);
//        cout << Solver::cost(solution, instance->get_matrix(), instance->get_size()) << endl;
        if (score = Solver::cost(solution, instance->get_matrix(), instance->get_size()); score < best_score) {
            best_score = score;
            best_solution = solution;
        }
    }
    return std::make_tuple(best_solution, c);
}

std::tuple<int*, int> Solver::random_walk(Instance *instance, int running_time, SolutionWriter* solution_writer) {
    std::srand(std::time(nullptr)); //idk if its ok for a coin toss, ctime is simpler so faster??
    mt19937 rng = get_rng();
    int n = instance->get_size()-1;
    int* solution = shuffle(instance->get_size(), rng);
    int cur_cost = Solver::cost(solution, instance->get_matrix(), instance->get_size());
//    cout << "Initial cost: " << cur_cost << endl;
    int* best_solution = new int[instance->get_size()];
    std::copy(solution, solution + instance->get_size(), best_solution);
    int c = 0, delta = 0, best_cost = cur_cost;
    std::uniform_int_distribution<int> dis1(0, n);
    std::uniform_int_distribution<int> dis2(0, n-1);
    auto endTime = std::chrono::steady_clock::now() + std::chrono::microseconds(running_time);
    while (std::chrono::steady_clock::now() < endTime) {
        c++;
//        cout << Solver::cost(test, instance->get_matrix(), instance->get_size()) << endl;
//        string move = "none";
        int j = dis1(rng); // 0 - n (n = instance.size() -1)
        int i = dis2(rng); // 0 - n-1
        if (i == j) j = n;
        if (j < i) std::swap(i, j);
        if ((i+1 == j) || (i == 0 && j == n)) {
            delta = calculate_special_deltas(solution, instance->get_matrix(), i, j,
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

        if (cur_cost < best_cost) {
            best_cost = cur_cost;
            std::copy(solution, solution + instance->get_size(), best_solution);
        }
    }
    return std::make_tuple(best_solution, c);
}
std::tuple<int*, int*> Solver::simulated_annealing(Instance *instance, SolutionWriter* solution_writer) {
    std::srand(std::time(nullptr)); //idk if its ok for a coin toss, ctime is simpler so faster??
    mt19937 rng = get_rng();
    int n = instance->get_size()-1;
    int* solution = shuffle(instance->get_size(), rng);
    int cur_cost = Solver::cost(solution, instance->get_matrix(), instance->get_size());
    int* best_solution = new int[instance->get_size()];
    std::copy(solution, solution + instance->get_size(), best_solution);
    int c = 0, delta = 0, best_cost = cur_cost;
    std::uniform_int_distribution<int> dis1(0, n);
    std::uniform_int_distribution<int> dis2(0, n-1);
    std::uniform_real_distribution<double> probability(0, 1);

    double initial_acceptance = 0.96;
    double alpha = 0.90;
    int L = 10 * n; // Markov chain length
    int P = 10;
    double T = -1.0 / log(initial_acceptance); // Starting temperature
//    cout << "temperature :" <<  T << endl;
    double min_probability = 0.01;
    double min_temperature = -1.0 / log(min_probability);
    int no_improvement_counter = 0;
    int iteration = 0;
    double prob;


    while (T > min_temperature && no_improvement_counter < P * L) {
        for (int k = 0; k < L; ++k) {
            c++;
            int j = dis1(rng); // 0 - n (n = instance.size() -1)
            int i = dis2(rng); // 0 - n-1
            if (i == j) j = n;
            prob = probability(rng);
            if (j < i) std::swap(i, j);
            if ((i+1 == j) || (i == 0 && j == n)) {
                delta = calculate_special_deltas(solution, instance->get_matrix(), i, j,
                                                        instance->get_size());
                if (delta > 0 || (prob < exp(delta / T))) {
                    std::swap(solution[i], solution[j]);
                }
            }
            else if (std::rand() % 2) {
                delta = calculate_deltas_node(solution, instance->get_matrix(), i, j,
                                              instance->get_size());
                if (delta > 0 || (prob < exp(delta / T))) {
                    std::swap(solution[i], solution[j]);
                }

            }
            else {
                delta = calculate_deltas_edge(solution, instance->get_matrix(), i, j,
                                              instance->get_size());
                if (delta > 0 || (prob < exp(delta / T))) {
                    std::reverse(solution + i, solution + j); //can we use this or implement our own reverse?
                }
            }

            cur_cost -= delta;

            if( delta <= 0)
            {
                no_improvement_counter++;
            }else{
                no_improvement_counter = 0;
            }
        }
        T *= alpha;
        iteration += L;

    }
    int* out = new int[2];
    out[0] = c;
    out[1] = c;
    return make_tuple(best_solution, out);
}


std::tuple<int*, int> Solver::steepest(Instance *instance,SolutionWriter* solution_writer) {
    mt19937 rng = get_rng();
    int* solution = shuffle(instance->get_size(), rng);
//    solution_writer->append_solution(solution,0,"initial_solution",true,0);
    int** matrix = instance->get_matrix();
    int current_best[4];
    int c = 0;
    do {
        c++;
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
    return std::make_tuple(solution, c);
}

std::tuple<int*, int> Solver::greedy(Instance *instance, SolutionWriter* solution_writer) {
    mt19937 rng = get_rng();
    int* solution = shuffle(instance->get_size(), rng);
//    solution_writer->append_solution(solution,0,"initial_solution",true, 0);
    int** matrix = instance->get_matrix();
    size_t total_count = (instance->get_size()*(instance->get_size()-1)
                              + instance->get_size()*(instance->get_size()-3)) / 2;
    int combinations[total_count][3];
    int current_best[4];
    int count, c = 0, ev = 0;
    count = 0;
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
    do {
        c++;
        current_best[0] = -1;
        shuffle(combinations, total_count, rng);
        for (auto& neighbor : combinations) {
            ev++;
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
    cout << "greedy evaluated: " << ev << endl;
    return std::make_tuple(solution, c);
}

std::tuple<int*, int> Solver::nearest_neighbour(Instance *instance, int start) {
    int* solution = new int[instance->get_size()];
    int** matrix = instance->get_matrix();

    int* available_nodes = new int[instance->get_size()];
    for (int i = 0; i < instance->get_size(); i++) {
        available_nodes[i] = i;
    }

    //initialize solution
    solution[0] = start;
    std::swap(available_nodes[start], available_nodes[instance->get_size() - 1]);

    //start from one because we already have the starting node
    for (int i = 1; i < instance->get_size(); i++) {
        int closest_node_index = 0;
        int min_distance = INT_MAX;
        for (int j = 0; j < instance->get_size() - i; j++) {
            if (matrix[solution[i-1]][available_nodes[j]] < min_distance) {
                min_distance = matrix[solution[i-1]][available_nodes[j]];
                closest_node_index = j;
            }
        }
        solution[i] = available_nodes[closest_node_index];

        std::swap(available_nodes[closest_node_index], available_nodes[instance->get_size() - i -1]);

    }
    return std::make_tuple(solution, instance->get_size());
}

std::tuple<int*, int> Solver::deterministic_greedy(Instance *instance,SolutionWriter* solution_writer) {
    mt19937 rng = get_rng();
    int* solution = shuffle(instance->get_size(), rng);
//    solution_writer->append_solution(solution,0,"initial_solution",true, 0);
    int** matrix = instance->get_matrix();
    int current_best[4];
    int c = 0;
    do {
        c++;
        current_best[0] = -1;
        for (int i = 0; i < instance->get_size(); i++) {
            if (current_best[0] > 0) break;
            for (int j = i + 2; j < instance->get_size() - 1; j++) {
                if (current_best[0] > 0) break;
                calculate_deltas_node(solution, matrix, current_best, i, j, instance->get_size());
                if (current_best[0] > 0) break;
                calculate_deltas_edge(solution, matrix, current_best, i, j, instance->get_size());
            }
        }
        // special case: neighbors
        // maybe i=0 outside?
        for (int i = 0; i < instance->get_size() - 1; i++) {
            if (current_best[0] > 0) break;
            calculate_special_deltas(solution, matrix, current_best, i, i+1, instance->get_size());
        }
        // case: last node in list with every other except neighbors
        int j = instance->get_size() - 1;
        for (int i = 1; i < instance->get_size() - 2; i++) {
            if (current_best[0] > 0) break;
            calculate_deltas_node(solution, matrix, current_best, i, j, instance->get_size());
            if (current_best[0] > 0) break;
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
    return std::make_tuple(solution, c);
}

std::tuple<int*, int> Solver::deterministic_greedy_2(Instance *instance, SolutionWriter *solution_writer) {
    mt19937 rng = get_rng();
    int* solution = shuffle(instance->get_size(), rng);
//    solution_writer->append_solution(solution,0,"initial_solution",true, 0);
    int** matrix = instance->get_matrix();
    size_t total_count = (instance->get_size()*(instance->get_size()-1)
                          + instance->get_size()*(instance->get_size()-3)) / 2;
    int combinations[total_count][3];
    int current_best[4];
    int count, c = 0, ev = 0;
    count = 0;
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
    do {
        c++;
        current_best[0] = -1;
//        shuffle(combinations, total_count, rng);
        for (auto& neighbor : combinations) {
            ev++;
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
    cout << "greedy evaluated: " << ev << endl;
    return std::make_tuple(solution, c);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::tuple<int*, int*> Solver::tabu_search(Instance* instance, SolutionWriter* solution_writer) {
    mt19937 rng = get_rng();
    // Initialize the solution and its evaluation
    int* solution = shuffle(instance->get_size(), rng);
    int best_eval = cost(solution, instance->get_matrix(), instance->get_size());
    int cur_eval = best_eval;

    static auto get_delta = [&](int i, int j, int type) -> int {
        if (type) {
            return calculate_deltas_edge(solution, instance->get_matrix(), i, j, instance->get_size());
        } else {
//            if (j < i) std::swap(i, j);
            if ((i+1 == j) || (i == 0 && j == instance->get_size()-1)) {
                return calculate_special_deltas(solution, instance->get_matrix(), i, j, instance->get_size());
            } else {
                return calculate_deltas_node(solution, instance->get_matrix(), i, j, instance->get_size());
            }
        }
    };

    // Initialize tabu search parameters
    const int tabu_tenure = instance->get_size() / 4;
    const int elite_size = instance->get_size() / 10;
    class TabuList {
    private:
        int n; // size
        int tenure;
        int** tabuList;
        // calculate the index of (i,j) in the tabuList array
        int index(int i, int j) {
            return (i * (n - 1) - ((i - 1) * i) / 2 + j - i - 1);
        }

    public:
        TabuList(int tenure, int size) {
            n = size;
            this->tenure = tenure;
//            tabuList = reinterpret_cast<int **>(new int[n * (n - 1) / 2][2]);
            this->tabuList = new int*[n * (n - 1) / 2];
            for (int i = 0; i < n * (n - 1) / 2; i++) {
                this->tabuList[i] = new int[2];
            }
            for (int i = 0; i < n * (n - 1) / 2; i++) {
                tabuList[i][0] = 0; // initialize all entries to 0
                tabuList[i][1] = 0;
            }
        }

        int isTabu(int i, int j, int type) {
            // check if (i,j) is in the upper triangle of the tabu list
            if (i >= j) {
                swap(i, j);
            }
            return tabuList[index(i, j)][type];
        }

        void insert(int i, int j, int type) {
            // add (i,j) with type to the tabu list with tenure
            if (i >= j) {
                swap(i, j);
            }
            tabuList[index(i, j)][type] = tenure;
        }

        void decrementTabu() {
            for (int i = 0; i < (n * (n - 1) / 2); i++) {
                if (tabuList[i][0] > 0) tabuList[i][0]--;
                if (tabuList[i][1] > 0) tabuList[i][1]--;
            }
        }

        ~TabuList() {
            delete[] tabuList;
        }
    };

    class EliteCandidateList {
    private:
        int size;
        int numElements;
        int* node_a;
        int* node_b;
        int* neighborhood_type;
        int* delta;

    public:
        EliteCandidateList(int size) {
            this->size = size;
            numElements = 0;
            node_a = new int[size];
            node_b = new int[size];
            neighborhood_type = new int[size];
            delta = new int[size];
        }

        ~EliteCandidateList() {
            delete[] node_a;
            delete[] node_b;
            delete[] neighborhood_type;
            delete[] delta;
        }

        void insert(int node_a, int node_b, int neighborhood_type, int delta) {
            // If the list is empty, just add the element
            if (numElements == 0) {
                this->node_a[0] = node_a;
                this->node_b[0] = node_b;
                this->neighborhood_type[0] = neighborhood_type;
                this->delta[0] = delta;
                numElements++;
                return;
            }

            if (numElements == size) {
                if (delta <= this->delta[size-1]) return;
                numElements--;
            }

            // Find the index where the new element should be inserted
            int i = numElements;
            while (i > 0 && delta > this->delta[i - 1]) {
                this->node_a[i] = this->node_a[i - 1];
                this->node_b[i] = this->node_b[i - 1];
                this->neighborhood_type[i] = this->neighborhood_type[i - 1];
                this->delta[i] = this->delta[i - 1];
                i--;
            }
            // Insert the new element at the correct index
            this->node_a[i] = node_a;
            this->node_b[i] = node_b;
            this->neighborhood_type[i] = neighborhood_type;
            this->delta[i] = delta;
            numElements++;
//        return;
        }
        EliteCandidateList& operator=(const EliteCandidateList& other) {
            if (this == &other) {
                return *this;
            }
            delete[] node_a;
            delete[] node_b;
            delete[] neighborhood_type;
            delete[] delta;
            size = other.size;
            numElements = other.numElements;
            node_a = new int[size];
            node_b = new int[size];
            neighborhood_type = new int[size];
            delta = new int[size];
            memcpy(node_a, other.node_a, size * sizeof(int));
            memcpy(node_b, other.node_b, size * sizeof(int));
            memcpy(neighborhood_type, other.neighborhood_type, size * sizeof(int));
            memcpy(delta, other.delta, size * sizeof(int));
            return *this;
        }

        EliteCandidateList recalculate_deltas() {
            // Recalculate the delta for each move
            EliteCandidateList new_list(size);
            int threshold = delta[numElements-1];
            for (int i = 0; i < numElements; i++) {
                // Call an external method to recalculate the delta for this move
                int new_delta = get_delta(node_a[i], node_b[i], neighborhood_type[i]);
//                cout << "=====" << endl;
//                cout << node_a[i] << " " << node_b[i] << " " << neighborhood_type[i] << " " << new_delta << endl;
                if (new_delta >= threshold)
                    new_list.insert(node_a[i], node_b[i], neighborhood_type[i], new_delta);
            }
//            *this = new_list;
            return new_list;
        }

        bool pop(int move[]) {
            if (numElements == 0) {
                return false;
            }

            // Remove the first element from the list
            move[0] = this->node_a[0];
            move[1] = this->node_b[0];
            move[2] = this->neighborhood_type[0];
            move[3] = this->delta[0];

            // Shift the remaining elements to the left
            for (int i = 1; i < numElements; i++) {
                this->node_a[i - 1] = this->node_a[i];
                this->node_b[i - 1] = this->node_b[i];
                this->neighborhood_type[i - 1] = this->neighborhood_type[i];
                this->delta[i - 1] = this->delta[i];
            }

            // Clear the last element to avoid duplicates or invalid values
            this->node_a[numElements - 1] = 0;
            this->node_b[numElements - 1] = 0;
            this->neighborhood_type[numElements - 1] = 0;
            this->delta[numElements - 1] = 0.0;

            // Decrement the size of the list
            numElements--;
            return true;
        }
        void clear() {
            numElements = 0;
            node_a = new int[size];
            node_b = new int[size];
            neighborhood_type = new int[size];
            delta = new int[size];
        }

        void print() {
            std::cout << "Elite Candidate List: " << std::endl;
            for (int i = 0; i < size; i++) {
                std::cout << "(" << node_a[i] << ", " << node_b[i] << ", " << neighborhood_type[i] << ", " << delta[i] << ")" << std::endl;
            }
        }
    };

    static EliteCandidateList elite_list(elite_size);
    TabuList tabu_list(tabu_tenure, instance->get_size());
    int iter = 0, c = 0;
    static int visited = 0;
    const int NEIGHBORHOOD_SIZE = (instance->get_size()*(instance->get_size()-1)
            + instance->get_size()*(instance->get_size()-3)) / 2;
    int* best_solution = new int[instance->get_size()];
    std::copy(solution, solution + instance->get_size(), best_solution);

    while(iter < 15) {
        iter++;
        tabu_list.decrementTabu();
        int current_best[4];
        bool found = elite_list.pop(current_best);
//        cout << "current_best:" << current_best[0] << " " << current_best[1] << " " << current_best[2] << " " << current_best[3] << endl;
//
//        elite_list.pop(current_best);
//        cout << "current_best:" << current_best[0] << " " << current_best[1] << " " << current_best[2] << " " << current_best[3] << endl;

//        cout << "found:" << found << endl;
        if (found) {
//            cout << "current_best:" << current_best[0] << " " << current_best[1] << " " << current_best[2] << " " << current_best[3] << endl;
            int *least_tabu = new int[4];
            std::copy(current_best, current_best + 4, least_tabu);
            int is_tabu;
            int lowest_tabu = tabu_list.isTabu(solution[current_best[0]], solution[current_best[1]], current_best[2]);
            while ((cur_eval - current_best[3] >= best_eval) &&
                   (is_tabu = tabu_list.isTabu(solution[current_best[0]], solution[current_best[1]],
                                               current_best[2]))) {
                if (is_tabu < lowest_tabu) {
                    std::copy(current_best, current_best + 4, least_tabu);
                    lowest_tabu = is_tabu;
                }
                if (!(found = elite_list.pop(current_best))) {
                    std::copy(least_tabu, least_tabu + 4, current_best);
                    break;
                }
            }
            c++;
            if (!current_best[2]) {
                std::swap(solution[current_best[0]], solution[current_best[1]]);
            } else {
                std::reverse(solution + current_best[0], solution + current_best[1]);
            }

            cur_eval -= current_best[3];
//            if (cost(solution, instance->get_matrix(), instance->get_size()) != cur_eval) {
//                cout << "cost error" << endl;
//                cout << cost(solution, instance->get_matrix(), instance->get_size()) << endl;
//                cout << cur_eval << endl;
//                cout << current_best[0] << current_best[1] << current_best[2] << current_best[3] << endl;
//                throw;
//            }
            EliteCandidateList newlist = elite_list.recalculate_deltas(); // does it also count to evaluated solutions?
            elite_list = newlist;
//            elite_list.print();
//            cout << "outside" << endl;
//            elite_list.print();

            if (cur_eval < best_eval) {
//                cout << cur_eval << endl;
//                cout << "actual cost" << cost(solution, instance->get_matrix(), instance->get_size()) << endl;
                best_eval = cur_eval;
                std::copy(solution, solution + instance->get_size(), best_solution);
                iter = 0;
            }
            tabu_list.insert(solution[current_best[0]], solution[current_best[1]], current_best[2]);
        }
        else {
            not_found:
            elite_list.clear();
            int delta;
            for (int i = 0; i < instance->get_size(); i++) {
                for (int j = i + 2; j < instance->get_size() - 1; j++) {
                    delta = calculate_deltas_node(solution, instance->get_matrix(), i, j, instance->get_size());
                    elite_list.insert(i, j, 0, delta);
                    delta = calculate_deltas_edge(solution, instance->get_matrix(), i, j, instance->get_size());
                    elite_list.insert(i, j, 1, delta);
                }
            }
            // special case: neighbors
            for (int i = 0; i < instance->get_size() - 1; i++) {
                delta = calculate_special_deltas(solution, instance->get_matrix(), i, i+1, instance->get_size());
                elite_list.insert(i, i+1, 0, delta);
            }
            // case: last node in list with every other except neighbors
            int j = instance->get_size() - 1;
            for (int i = 1; i < instance->get_size() - 2; i++) {
                delta = calculate_deltas_node(solution, instance->get_matrix(), i, j, instance->get_size());
                elite_list.insert(i, j, 0, delta);
                delta = calculate_deltas_edge(solution, instance->get_matrix(), i, j, instance->get_size());
                elite_list.insert(i, j, 1, delta);
            }
            visited += NEIGHBORHOOD_SIZE;
        }
    }
//    cout << "evaluated solutions: " << visited << endl;
    int* out = new int[2];
    out[0] = c;
    out[1] = visited;
    return make_tuple(best_solution, out);
}
