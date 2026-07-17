#ifndef KHAUPHANDINHDUONG_DATABASE_HPP
#define KHAUPHANDINHDUONG_DATABASE_HPP
#include <filesystem>

#include "food_enum.hpp"

namespace MFCPP::Database {
    static void LoadFood(const std::filesystem::path& path);
    void Init();
    static void print(FoodID id);
}

#endif //KHAUPHANDINHDUONG_DATABASE_HPP
