#ifndef KHAUPHANDINHDUONG_SOLVER_HPP
#define KHAUPHANDINHDUONG_SOLVER_HPP
#include <model/HighsModel.h>

namespace MFCPP::Solver {
    struct Boundary {
        double k_min, k_max; //calories
        double p_min, p_max; // protein
        double l_min, l_max; // lipid
        double c_min, c_max; // carbonhydrate
    };

    bool Solve(std::vector<int64_t>&vec, HighsModel& model);
    void Generate(HighsModel& model, const Boundary& data);
}

#endif //KHAUPHANDINHDUONG_SOLVER_HPP
