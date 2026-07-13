#include <qapplication.h>
#include <qmainwindow.h>
#include "cores/logging.hpp"
#include <config.h>
#include <cpptrace/from_current.hpp>

#include "cores/handler.hpp"

#if DEVELOPMENT_BUILD
int main(int argc, char** argv) {
#else
int WinMain(int argc, char** argv) {
#endif
    MFCPP::Handler::HandlerInit();
    CPPTRACE_TRY {
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
