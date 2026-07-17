#include "common/Logging.hpp"
#include <cpptrace/from_current.hpp>
#include <config.h>
#include <fmt/format.h>
#include <fmt/color.h>
#include <boost/type_index.hpp>

namespace MFCPP {
    namespace Log {
#if LOGGING_ENABLE
        std::ofstream out("MFCPP_runtime.log");
#endif
        void ExceptionPrint(const std::exception* exp) {
#if LOGGING_ENABLE
            out << fmt::format("[FATAL] Exception {} {}\n", typeid(*exp).name(),exp->what());
#endif
#if DEVELOPMENT_BUILD
            fmt::print(fmt::fg(fmt::color::orange_red) | fmt::emphasis::bold, "[FATAL] Exception {}\n{}\n", boost::typeindex::type_id_runtime(*exp).pretty_name(),exp->what());
#endif
        }
        void FatalPrint(const std::string_view str) {
#if LOGGING_ENABLE
            out << fmt::format("[FATAL] {}\n", str);
#endif
#if DEVELOPMENT_BUILD
            fmt::print(fmt::fg(fmt::color::orange_red) | fmt::emphasis::bold, "[FATAL] {}\n", str);
#endif
        }
        void ErrorColorPrint(const std::string_view str) {
#if LOGGING_ENABLE
            out << fmt::format("{}\n", str);
#endif
#if DEVELOPMENT_BUILD
            fmt::print(fmt::fg(fmt::color::salmon), "{}\n", str);
#endif
        }
        void ErrorPrint(const std::string_view str) {
#if LOGGING_ENABLE
            out << fmt::format("[ERROR] {}\n", str);
#endif
#if DEVELOPMENT_BUILD
            fmt::print(fmt::fg(fmt::color::salmon), "[ERROR] {}\n", str);
#endif
        }
        void InfoPrint(const std::string_view str) {
#if LOGGING_ENABLE
            out << fmt::format("[INFO] {}\n", str);
#endif
#if DEVELOPMENT_BUILD
            fmt::print(fmt::fg(fmt::color::dim_gray) | fmt::emphasis::italic, "[INFO] {}\n", str);
#endif
        }
        void SuccessPrint(const std::string_view str) {
#if LOGGING_ENABLE
            out << fmt::format("[SUCCESS] {}\n", str);
#endif
#if DEVELOPMENT_BUILD
            fmt::print(fmt::fg(fmt::color::lime) | fmt::emphasis::bold, "[SUCCESS] {}\n", str);
#endif
        }
        void WarningPrint(const std::string_view str) {
#if LOGGING_ENABLE
            out << fmt::format("[WARNING] {}\n", str);
#endif
#if DEVELOPMENT_BUILD
            fmt::print(fmt::fg(fmt::color::gold), "[WARNING] {}\n", str);
#endif
        }
        void printSigTrace() {
            const cpptrace::stacktrace& trace = cpptrace::generate_trace(1);
            ErrorColorPrint("--- Stack Trace (most recent call first) ---");
            for (const auto & frame : trace) {
                if (frame.symbol.empty() || !frame.line.has_value()) continue;

                ErrorColorPrint(
                    fmt::format(
                        "	at {}({}:{}:{})",
                        frame.symbol,
                        frame.filename,
                        frame.line.value_or(0),
                        frame.column.value_or(0)
                    )
                );
            }
        }
        void printCurrentTrace() {
            const cpptrace::stacktrace& trace = cpptrace::from_current_exception();
            ErrorColorPrint("--- Stack Trace (most recent call first) ---");
            for (const auto & frame : trace) {
                if (frame.symbol.empty() || !frame.line.has_value()) continue;

                ErrorColorPrint(
                    fmt::format(
                        "	at {}({}:{}:{})",
                        frame.symbol,
                        frame.filename,
                        frame.line.value_or(0),
                        frame.column.value_or(0)
                    )
                );
            }
        }
    }
}
