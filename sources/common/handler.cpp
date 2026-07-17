#include "common/handler.hpp"

#include <csignal>
#include "common/Logging.hpp"
#include <fmt/format.h>
#include <cpptrace/utils.hpp>

namespace MFCPP {
    void Handler::SIGHandler(int signum) {
        switch (signum) {
            case SIGFPE:
                Log::FatalPrint("Floating-point Exception");
                break;
            case SIGILL:
                Log::FatalPrint("Illegal instruction Exception");
                break;
            case SIGSEGV:
                Log::FatalPrint("Invalid memory access (Segmentation Fault)");
                break;
            case SIGINT:
                Log::FatalPrint("External Interrupted");
                break;
            case SIGTERM:
                Log::FatalPrint("Termination Request");
                break;
            case SIGABRT:
                Log::FatalPrint("Abort Request");
                break;
            default:
                Log::FatalPrint(fmt::format("Signal Code {} received.", signum));
                break;
        }
        Log::printSigTrace();
    }
    void Handler::HandlerInit() {
        cpptrace::register_terminate_handler();
        std::signal(SIGSEGV, SIGHandler);
        std::signal(SIGFPE, SIGHandler);
        std::signal(SIGILL, SIGHandler);
        std::signal(SIGINT, SIGHandler);
        std::signal(SIGTERM, SIGHandler);
        std::signal(SIGABRT, SIGHandler);
    }
}
