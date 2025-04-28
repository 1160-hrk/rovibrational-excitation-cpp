#include "rvx/lvne.hpp"
#include <Eigen/Core>
#include <omp.h>

namespace rvx {

    void initialize_eigen_threads() {
    Eigen::setNbThreads(omp_get_max_threads());
}

int get_eigen_num_threads() {
    return Eigen::nbThreads();
}

void set_eigen_num_threads(int n) {
    Eigen::setNbThreads(n);
}

}