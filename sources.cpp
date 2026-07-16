#include <qapplication.h>
#include <qmainwindow.h>
#include "cores/logging.hpp"
#include <config.h>
#include <cpptrace/from_current.hpp>
#include "cores/handler.hpp"
#include <Highs.h>
void test() {
    HighsModel model;
    model.lp_.num_col_ = 2;
    model.lp_.num_row_ = 2;
    model.lp_.sense_ = ObjSense::kMaximize;
    model.lp_.offset_ = 0;

    model.lp_.col_cost_ = {3.5, 2.0};
    model.lp_.col_lower_ = {0.0, 0.0};
    model.lp_.col_upper_ = {kHighsInf, kHighsInf};

    model.lp_.row_lower_ = {-kHighsInf, -kHighsInf};
    model.lp_.row_upper_ = {100.0, 120.0};

    model.lp_.a_matrix_.format_ = MatrixFormat::kColwise;
    model.lp_.a_matrix_.start_ = {0, 2, 4};
    model.lp_.a_matrix_.index_ = {0, 1, 0, 1};
    model.lp_.a_matrix_.value_ = {1.0, 2.0, 1.0, 1.0};

    Highs highs;
    highs.setOptionValue("output_flag", false);
    HighsStatus return_status;
    return_status = highs.passModel(model);
    assert(return_status == HighsStatus::kOk);

    const HighsLp& lp = highs.getLp();
    //solve
    return_status = highs.run();
    assert(return_status == HighsStatus::kOk);
    const HighsModelStatus& model_status = highs.getModelStatus();
    assert(model_status == HighsModelStatus::kOptimal);

    const HighsSolution& solution = highs.getSolution();
    for (size_t i = 0; i < solution.col_value.size(); ++i) {
        MFCPP::Log::InfoPrint(fmt::format("x{}: {}", i, solution.col_value[i]));
    }
    double cost_ans = 0.0;
    for (size_t i = 0; i < solution.col_value.size(); ++i) {
        cost_ans += solution.col_value[i] * model.lp_.col_cost_[i];
    }
    MFCPP::Log::InfoPrint(fmt::format("Cost: {}", cost_ans));
}
#if DEVELOPMENT_BUILD
int main(int argc, char** argv) {
#else
int WinMain(int argc, char** argv) {
#endif
    MFCPP::Handler::HandlerInit();
    CPPTRACE_TRY {
        test();
        QApplication app(argc, argv);
        MFCPP::Log::SuccessPrint(fmt::format("Version: {}", PROJECT_VERSION));
        QMainWindow window;
        window.setWindowTitle("Test");
        window.resize(800, 600);
        window.show();
        std::exit(app.exec());
    } CPPTRACE_CATCH (std::exception& e) {
        MFCPP::Log::ExceptionPrint(&e);
        MFCPP::Log::printCurrentTrace();
        std::exit(EXIT_FAILURE);
    }
}
