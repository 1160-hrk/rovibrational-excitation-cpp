#pragma once

namespace evx {
    
void initialize_eigen_threads();
int get_eigen_num_threads();
void set_eigen_num_threads(int n);

}