#ifndef LOGGING_HPP
#define LOGGING_HPP
#include <fmt/os.h>

namespace MFCPP {
    namespace Log {
        void ExceptionPrint(const std::exception* exp);
        void ErrorColorPrint(std::string_view str);
        void InfoPrint(std::string_view str);
        void WarningPrint(std::string_view str);
        void ErrorPrint(std::string_view str);
        void SuccessPrint(std::string_view str);

        void printSigTrace();
        void printCurrentTrace();
        void FatalPrint(std::string_view str);
    }
}


#endif //LOGGING_HPP
