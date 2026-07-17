#include "cores/solver.hpp"

#include <Highs.h>
#include "cores/logging.hpp"

bool MFCPP::Solver::Solve(std::vector<int64_t>&vec, HighsModel &model) {
    Highs highs;
    highs.setOptionValue("output_flag", false);
    HighsStatus return_status = highs.passModel(model);
    assert(return_status == HighsStatus::kOk);

    const HighsLp& lp = highs.getLp();
    //solve
    return_status = highs.run();
    assert(return_status == HighsStatus::kOk);
    const HighsModelStatus& model_status = highs.getModelStatus();
    switch (model_status) {
        case HighsModelStatus::kOptimal: {
            Log::SuccessPrint("Solver: Success Find Optimal Solution");
            const HighsSolution& solution = highs.getSolution();
            for (size_t i = 0; i < solution.col_value.size(); ++i)
                vec.push_back(solution.col_value[i]);
            return true;
        }
        case HighsModelStatus::kInfeasible: {
            Log::ErrorPrint("Solver: Model is infeasible");
            break;
        }
        case HighsModelStatus::kMemoryLimit: {
            Log::ErrorPrint("Solver: Out of memory while finding solution");
            break;
        }
        case HighsModelStatus::kUnbounded: {
            Log::ErrorPrint("Solver: Model can be improved infinitely");
            break;
        }
        default: {
            Log::ErrorPrint("Solver: An error occured while solving the model");
            break;
        }
    }
    return false;
    //double cost_ans = 0.0;
    //for (size_t i = 0; i < solution.col_value.size(); ++i) {
    //    cost_ans += solution.col_value[i] * model.lp_.col_cost_[i];
    //}
    //MFCPP::Log::InfoPrint(fmt::format("Cost: {}", cost_ans));
    highs.resetGlobalScheduler(true);
}

void MFCPP::Solver::Generate(HighsModel &model) {
    // Implement later
    model.lp_.num_col_ = 2;
    model.lp_.num_row_ = 3;
    model.lp_.sense_ = ObjSense::kMinimize;
    model.lp_.offset_ = 3;

    //Force to only find interger solution
    model.lp_.integrality_.resize(model.lp_.num_col_);
    for (int col = 0; col < model.lp_.num_col_; ++col)
        model.lp_.integrality_[col] = HighsVarType::kInteger;

    model.lp_.col_cost_ = {1.0, 1.0};
    model.lp_.col_lower_ = {0.0, 1.0};
    model.lp_.col_upper_ = {4.0, kHighsInf};

    model.lp_.row_lower_ = {-kHighsInf, 5.0, 6.0};
    model.lp_.row_upper_ = {7.0, 15.0, kHighsInf};

    model.lp_.a_matrix_.format_ = MatrixFormat::kColwise;
    model.lp_.a_matrix_.start_ = {0, 3, 6};
    model.lp_.a_matrix_.index_ = {0, 1, 2, 0, 1, 2};
    model.lp_.a_matrix_.value_ = {0.0, 1.0, 3.0, 1.0, 2.0, 2.0};
}
