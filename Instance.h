#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "StringFunctions.h"

using namespace std;

class Instance
{
public:
    int size = -1;
    int64_t optimal_tour_length = -1;
    int** matrix;

    Instance(string filename, bool parse_data_to_matrix);
    ~Instance();
    int edge_weight(int from, int to);
    void display();
    void print_matrix();

private:
    double** coords;
    bool is_instance_data_in_matrix;

    int nint(double d);
    double** parse_coords(vector<string> file, int i, int dimensions);
    int max(int i1, int i2);
    int max(int i1, int i2, int i3);
    void load_optimal_tour_length(string filename);

    int calculate_distance(int from_city, int to_city);
};