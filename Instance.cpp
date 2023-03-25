#include "Instance.h"
#include<cmath>

Instance::Instance(string filename, bool parse_data_to_matrix)
{
    //Specification variables
    string name;
    string comment;
    string type;
    string dimension;
    int data_start = 0;

    //Read the file
    ifstream f(filename);
    if (f.fail()) { cout << "Something went wrong while loading the file." << "\r\n"; return; }
    string line;
    vector<string> file;

    while (getline(f, line))
    {
        if (StringFunctions::starts_with(line, "NAME"))
        {
            name = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
        }
        else if (StringFunctions::starts_with(line, "COMMENT"))
        {
            comment = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
        }
        else if (StringFunctions::starts_with(line, "TYPE"))
        {
            type = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
        }
        else if (StringFunctions::starts_with(line, "DIMENSION"))
        {
            dimension = StringFunctions::trim(StringFunctions::get_substring_after_char(line, ':'));
        }
        else
        {
            stringstream ss(line);
            string s;

            while (getline(ss, s, ' '))
            {
                if (s.length() > 0) file.push_back(s);
            }
        }
    }

    for (int i = 0; i < file.size(); i++)
    {
        if (file[i] == "NODE_COORD_SECTION" || file[i] == "EDGE_WEIGHT_SECTION") { data_start = i; break; }
    }

    //Store instance size
    StringFunctions::to_int(dimension, &size);

    if (size < 0) { cout << "Bad instance size. File is corrupted and/or not supported." << "\r\n"; return; }

    //Store the information if instance data is stored as matrix or as coords
    is_instance_data_in_matrix = parse_data_to_matrix;

    if (is_instance_data_in_matrix)
    {
        //Create matrix
        matrix = new int* [size];
        for (int i = 0; i < size; i++) matrix[i] = new int[size];
    }

    //Display instance information
    cout << "Instance: " << name << "\r\n";
    cout << "Comment: " << comment << "\r\n";
    cout << "Type: " << type << "\r\n";
    cout << "Size: " << size << "\r\n";
    cout << "\r\n";

    int i = data_start;
    while (i < file.size() && file[i] != "NODE_COORD_SECTION") i++;
    i++;


    coords = parse_coords(file, i, 2);
    if (is_instance_data_in_matrix)
    {
        for (int j = 0; j < size; j++)
        {
            for (int k = 0; k < size; k++)
            {
                matrix[j][k] = calculate_distance(j, k);
            }
        }

        for (int i = 0; i < size; i++) delete[] coords[i];
        delete[] coords;
    }


    load_optimal_tour_length(filename + ".opt");
}

Instance::~Instance()
{
    if (size > 0)
    {
        if (is_instance_data_in_matrix)
        {
            for (int i = 0; i < size; i++) delete[] matrix[i];
            delete[] matrix;
        }
        else
        {
            for (int i = 0; i < size; i++) delete[] coords[i];
            delete[] coords;
        }
    }
}

int Instance::edge_weight(int from, int to)
{
    if (is_instance_data_in_matrix)	return matrix[from][to];

    return calculate_distance(from, to);
}

void Instance::display()
{
    //Save cout flags
    ios_base::fmtflags f(cout.flags());

    cout.fill(' ');

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            cout << setw(6) << edge_weight(i, j) << " ";
        }
        cout << "\r\n";
    }
    cout << "\r\n";

    //Restore cout flags
    cout.flags(f);
}

int Instance::nint(double d)
{
    return (int)(d + 0.5);
}

double** Instance::parse_coords(vector<string> file, int i, int dimensions)
{
    //Create 2D array for coords
    double** coords = new double* [size];
    for (int j = 0; j < size; j++) coords[j] = new double[dimensions];

    //Parse coords
    for (int j = 0; j < size; j++)
    {
        int index;
        StringFunctions::to_int(file[i], &index);
        index--;
        i++;
        for (int k = 0; k < dimensions; k++)
        {
            StringFunctions::to_double(file[i], &coords[index][k]);
            i++;
        }
    }

    return coords;
}

int Instance::max(int i1, int i2)
{
    return i1 > i2 ? i1 : i2;
}

int Instance::max(int i1, int i2, int i3)
{
    return i1 > i2 ? i1 > i3 ? i1 : i2 : i2 > i3 ? i2 : i3;
}

void Instance::load_optimal_tour_length(string filename)
{
    //Check if the file with optimal path length exists
    //If true, read the file and store the value
    ifstream f(filename);
    if (f.fail()) return;

    string line;
    getline(f, line);
    StringFunctions::to_int64(line, &optimal_tour_length);
}

int Instance::calculate_distance(int from_city, int to_city)
{
    if (from_city == to_city) return -1;

    double xd = coords[from_city][0] - coords[to_city][0];
    double yd = coords[from_city][1] - coords[to_city][1];

    return nint(sqrt(xd * xd + yd * yd));
}

void Instance::print_matrix() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}