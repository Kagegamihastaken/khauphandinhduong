#ifndef KHAUPHANDINHDUONG_SOLVER_HPP
#define KHAUPHANDINHDUONG_SOLVER_HPP
#include <model/HighsModel.h>

namespace MFCPP::Solver {
    bool Solve(std::vector<int64_t>&vec, HighsModel& model);
    void Generate(HighsModel& model);
}

#endif //KHAUPHANDINHDUONG_SOLVER_HPP
