#include <qapplication.h>
#include <qmainwindow.h>
#include "common/logging.hpp"
#include <config.h>
#include <cpptrace/from_current.hpp>
#include "common/handler.hpp"
#include <Highs.h>
#include "common/io.hpp"
#include "common/solver.hpp"
#include "ui_theTestWidget.h"
#include "common/database.hpp"

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
        MFCPP::Log::SuccessPrint(fmt::format("Version: {}", PROJECT_VERSION));
        MFCPP::IO::Init();
        MFCPP::Database::Init();
        test();
        QApplication app(argc, argv);
        app.setQuitOnLastWindowClosed(true);
        QMainWindow window;
        Ui::TheTestWidget ui;
        ui.setupUi(&window);
        window.setWindowTitle("Test");
        window.resize(800, 600);
        window.show();
        exit_code = app.exec();
        MFCPP::Log::InfoPrint("Application event loop finished.");
    } CPPTRACE_CATCH (std::exception& e) {
        MFCPP::Log::ExceptionPrint(&e);
        MFCPP::Log::printCurrentTrace();
        return EXIT_FAILURE;
    }
    MFCPP::IO::Deinit();
    return 0;
}
