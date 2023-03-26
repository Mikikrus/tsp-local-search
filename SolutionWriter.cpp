#include "SolutionWriter.h"
#include "Instance.h"
#include "Solver.h"

SolutionWriter::SolutionWriter(Instance* instance, const string& dest_dir, string algorithm_name, int number_of_runs)
{
    this->instance = instance;
    this->dest_dir = dest_dir;
    this->destination_filename = dest_dir +algorithm_name+"_"+ instance->name + ".sol";
    this->number_of_runs = number_of_runs;
    this->calculated_solutions = new int* [this->instance->size];
    for (int i = 0; i < this->instance->size; i++) this->calculated_solutions [i] = new int[number_of_runs];
    this->time_table = new long long[number_of_runs];
    this->optimal_solution = get_optimal_solution(dest_dir+instance->name+".opt.tour");
}

int* SolutionWriter::get_optimal_solution(string optimal_solution_path)
{
    ifstream file;
    file.open(optimal_solution_path);
    int* solution = new int[this->instance->get_size()];
    bool read_line = false;
    string line;
    int i = 0;
    while (getline(file, line))
    {
        if (StringFunctions::starts_with(line, "TOUR_SECTION")) {
            read_line = true;
        }
        else if (read_line){
            if (StringFunctions::starts_with(line, "-1")) {
                read_line = false;
            }
            else{
                solution[i] = stoi(line)-1;
                i++;
            }

        }
    }
    file.close();
    return solution;
}
void SolutionWriter::append_solution(int* solution, long long elapsed_time){
    for (int i = 0; i < this->instance->get_size(); i++) {
        this->calculated_solutions[i][this->number_of_appended_solutions] = solution[i];
    }
    this->time_table[this->number_of_appended_solutions] = elapsed_time;
    this->number_of_appended_solutions++;
}

void SolutionWriter::write_solution()
{
    ofstream file;
    cout<<this->destination_filename<<endl;
    cout<< instance->get_size()<<endl;
    file.open(this->destination_filename);
    string optimal_node;
    string node;
    string node_index;
    string elapsed_time;
    string x_coor;
    string y_coor;
    string file_header = "node_index,x_coor,y_corr,optimal_order,";
    for (int i = 0; i < this->number_of_appended_solutions; i++) {
        file_header += "calculated_order_" + std::to_string(i) + ",";
    }
    file << file_header << endl;
    for (int i = 0; i < instance->get_size(); i++)
    {
        optimal_node = std::to_string(this->optimal_solution[i]);
        x_coor = std::to_string(instance->coords[i][0]);
        y_coor = std::to_string(instance->coords[i][1]);
        node_index = std::to_string(i);
        for (int j = 0; j < this->number_of_appended_solutions; j++) {
            node += std::to_string(this->calculated_solutions[i][j]) + ",";
        }
        file << node_index + "," + x_coor + "," + y_coor + "," + optimal_node + "," + node << endl;
        node = "";

    }
    for (int j = 0; j < this->number_of_appended_solutions; j++) {
        elapsed_time += std::to_string(this->time_table[j]) + ",";
    }
    file << "time [ms],,,," + elapsed_time << endl;
    file.close();
}
void SolutionWriter::print_summary()
{
    int optimal_cost = Solver::cost(this->optimal_solution, this->instance->get_matrix(), this->instance->get_size());
    double avg_calculated_cost = 0.0;
    double avg_elapsed_time = 0.0;
    for (int i = 0; i < this->number_of_appended_solutions; i++) {
        avg_calculated_cost += Solver::cost(this->calculated_solutions[i], this->instance->get_matrix(),this->instance->get_size());
        avg_elapsed_time+= this->time_table[i];
    }
    avg_calculated_cost /= this->number_of_appended_solutions;
    avg_elapsed_time /= this->number_of_appended_solutions;
    cout << "Optimal cost: " << optimal_cost << endl;
    cout << "Average calculated cost: " << avg_calculated_cost << endl;
    cout << "Average elapsed time: " << avg_elapsed_time << endl;
}