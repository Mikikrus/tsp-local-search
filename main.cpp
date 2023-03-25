#include <iostream>
#include "Instance.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    Instance *x = new Instance("/Users/maciej.filanowicz/tsp-local-search/data/att48.tsp",true);
    x->print_matrix();
    return 0;
}
