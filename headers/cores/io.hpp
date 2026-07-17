#ifndef KHAUPHANDINHDUONG_IO_HPP
#define KHAUPHANDINHDUONG_IO_HPP
#include <filesystem>
#include <string>
#include <vector>
namespace MFCPP::IO {
    void Init();
    void Deinit();
    void LoadJSON(std::string& inp, const std::filesystem::path& path);
    static void Loadbyte(std::vector<uint8_t>& vec, const std::filesystem::path& path);
}

#endif //KHAUPHANDINHDUONG_IO_HPP
