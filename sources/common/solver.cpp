#include "common/solver.hpp"

#include <Highs.h>

#include "common/database.hpp"
#include "common/food_enum.hpp"
#include "common/logging.hpp"
#include "magic_enum/magic_enum.hpp"

void print(const std::vector<double>& vec) {
    constexpr auto food_entries = magic_enum::enum_entries<FoodID>();
    std::string out = "[";
    for (int i = 0; i < food_entries.size(); ++i) {
        if (food_entries[i].first == FoodID::FOOD_NULL) continue;
        out += std::to_string(vec[i]) + ", ";
    }
    out.pop_back();
    out.pop_back();
    out += "]";
    MFCPP::Log::InfoPrint(out);
}

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
    bool return_value = false;
    switch (model_status) {
        case HighsModelStatus::kOptimal: {
            Log::SuccessPrint("Solver: Success Find Optimal Solution");
            const HighsSolution& solution = highs.getSolution();
            for (size_t i = 0; i < solution.col_value.size(); ++i)
                vec.push_back(solution.col_value[i]);
            return_value = true;
            break;
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
    highs.resetGlobalScheduler(true);
    highs.releaseMemory();
    return return_value;
    //double cost_ans = 0.0;
    //for (size_t i = 0; i < solution.col_value.size(); ++i) {
    //    cost_ans += solution.col_value[i] * model.lp_.col_cost_[i];
    //}
    //MFCPP::Log::InfoPrint(fmt::format("Cost: {}", cost_ans));
}

void MFCPP::Solver::Generate(HighsModel &model, const Boundary& data) {
    model.clear();
    Log::InfoPrint(fmt::format("k_min: {}, k_max: {}, p_min: {}, p_max: {}", data.k_min, data.k_max, data.p_min, data.p_max));
    Log::InfoPrint(fmt::format("l_min: {}, l_max: {}, c_min: {}, c_max: {}", data.l_min, data.l_max, data.c_min, data.c_max));
    // Min        f  = a_0*x_0 + a_1*x_1 + ...           where a_i is price of food, x_i is amount of food
    // s.t.   k_min <= k_0*x_0 + k_1*x_1 + ... <= k_max  where k_i is amount calories of food
    //        p_min <= p_0*x_0 + p_1*x_1 + ... <= p_max  where p_i is amount protein of food
    //        l_min <= l_0*x_0 + l_1*x_1 + ... <= l_min  where l_i is amount lipid of food
    //        c_min <= c_0*x_0 + c_1*x_1 + ... <= c_max  where c_i is amount carbonhydrate of food
    // 0 <= x_i <= 2        I will use maximum of 2 food for now, in the future I will change depend on user want

    // the amount of food can be change depend on whether user want this food or not
    // if not select, immediately multiply with 0, else multiply with 1
    constexpr auto food_entries = magic_enum::enum_entries<FoodID>();
    Log::InfoPrint(fmt::format("Solver: Generating model with {} food entries", food_entries.size() - 1));
    model.lp_.num_col_ = food_entries.size() - 1;
    model.lp_.num_row_ = 4;
    model.lp_.sense_ = ObjSense::kMinimize;
    model.lp_.offset_ = 0;

    //Force to only find integer solution
    model.lp_.integrality_.resize(model.lp_.num_col_);
    for (int col = 0; col < model.lp_.num_col_; ++col)
        model.lp_.integrality_[col] = HighsVarType::kInteger;

    for (int i = 0; i < food_entries.size(); ++i) {
        if (food_entries[i].first == FoodID::FOOD_NULL) continue;
        model.lp_.col_cost_.push_back(Database::getFood(food_entries[i].first).price);
    }
    //model.lp_.col_cost_ = {1.0, 1.0};
    for (int i = 0; i < food_entries.size(); ++i) {
        if (food_entries[i].first == FoodID::FOOD_NULL) continue;
        model.lp_.col_lower_.push_back(0.0);
    }
    //model.lp_.col_lower_ = {0.0, 1.0};
    for (int i = 0; i < food_entries.size(); ++i) {
        if (food_entries[i].first == FoodID::FOOD_NULL) continue;
        model.lp_.col_upper_.push_back(2.0 * Database::getFoodSelected(food_entries[i].first));
    }
    //model.lp_.col_upper_ = {4.0, kHighsInf};

    model.lp_.row_lower_ = {data.k_min, data.p_min, data.l_min, data.c_min};
    //model.lp_.row_lower_ = {-kHighsInf, 5.0, 6.0};
    model.lp_.row_upper_ = {data.k_max, data.p_max, data.l_max, data.c_max};
    //model.lp_.row_upper_ = {7.0, 15.0, kHighsInf};

    model.lp_.a_matrix_.format_ = MatrixFormat::kColwise;

    model.lp_.a_matrix_.start_.push_back(0);
    for (int i = 1; i <= model.lp_.num_col_; ++i) {
        model.lp_.a_matrix_.start_.push_back(i*model.lp_.num_row_);
    }
    //model.lp_.a_matrix_.start_ = {0, 3, 6};
    for (int i = 0; i < model.lp_.num_col_; ++i) {
        for (int j = 0; j < model.lp_.num_row_; ++j) {
            model.lp_.a_matrix_.index_.push_back(j);
        }
    }
    //model.lp_.a_matrix_.index_ = {0, 1, 2, 0, 1, 2};
    for (int i = 0; i < food_entries.size(); ++i) {
        if (food_entries[i].first == FoodID::FOOD_NULL) continue;
        model.lp_.a_matrix_.value_.push_back(Database::getFood(food_entries[i].first).kcal);
        model.lp_.a_matrix_.value_.push_back(Database::getFood(food_entries[i].first).protein);
        model.lp_.a_matrix_.value_.push_back(Database::getFood(food_entries[i].first).lipid);
        model.lp_.a_matrix_.value_.push_back(Database::getFood(food_entries[i].first).carbohydrate);
    }
    //model.lp_.a_matrix_.value_ = {0.0, 1.0, 3.0, 1.0, 2.0, 2.0};
}
