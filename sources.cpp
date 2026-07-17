#include <qapplication.h>
#include <qmainwindow.h>
#include "cores/logging.hpp"
#include <config.h>
#include <cpptrace/from_current.hpp>
#include "cores/handler.hpp"
#include <Highs.h>
#include "cores/io.hpp"
#include "cores/solver.hpp"

void test() {
    std::vector<int64_t> ans;
    HighsModel model;
    MFCPP::Solver::Generate(model);
    if (MFCPP::Solver::Solve(ans, model)) {
        for (const auto &i : ans) {
            MFCPP::Log::InfoPrint(fmt::format("{}", i));
        }
    }
}
#if DEVELOPMENT_BUILD
int main(int argc, char** argv) {
#else
int WinMain(int argc, char** argv) {
#endif
    MFCPP::Handler::HandlerInit();
    int exit_code = 0;
    CPPTRACE_TRY {
        MFCPP::IO::Init();
        test();
        QApplication app(argc, argv);
        MFCPP::Log::SuccessPrint(fmt::format("Version: {}", PROJECT_VERSION));
        QMainWindow window;
        window.setWindowTitle("Test");
        window.resize(800, 600);
        window.show();
        exit_code = app.exec();
    } CPPTRACE_CATCH (std::exception& e) {
        MFCPP::Log::ExceptionPrint(&e);
        MFCPP::Log::printCurrentTrace();
        std::exit(EXIT_FAILURE);
    }
    MFCPP::IO::Deinit();
    std::exit(exit_code);
}
