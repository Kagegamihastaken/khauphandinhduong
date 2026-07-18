#include <qapplication.h>
#include "common/logging.hpp"
#include <config.h>
#include <cpptrace/from_current.hpp>
#include "common/handler.hpp"
#include <Highs.h>
#include "common/io.hpp"
#include "common/database.hpp"
#include "magic_enum/magic_enum.hpp"
#include "ui/home.hpp"

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
        QApplication app(argc, argv);
        app.setQuitOnLastWindowClosed(true);
        HomeWidget window;
        window.show();
        exit_code = app.exec();
        MFCPP::Log::InfoPrint("Application closing...");
    } CPPTRACE_CATCH (std::exception& e) {
        MFCPP::Log::ExceptionPrint(&e);
        MFCPP::Log::printCurrentTrace();
        return EXIT_FAILURE;
    }
    MFCPP::IO::Deinit();
    return 0;
}
