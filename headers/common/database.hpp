#ifndef KHAUPHANDINHDUONG_DATABASE_HPP
#define KHAUPHANDINHDUONG_DATABASE_HPP
#include <filesystem>

#include "food_enum.hpp"

struct Food {
    std::string name = "Null";
    double kcal = 0.0; //kcal
    double protein = 0.0; //g
    double lipid = 0.0; //g
    double carbohydrate = 0.0; //g
    double price = 0.0; //k VND
    FoodID food_id = FoodID::FOOD_NULL;
};
struct FoodData {
    uint8_t food_select;
    uint8_t force_select;
};

namespace MFCPP::Database {
    static void LoadFood(const std::filesystem::path& path);
    static void print(FoodID id);
    void Init();
    void clearSelect();
    void setFoodSelected(FoodID id, bool val);
    void setAllFoodSelected(bool val);
    [[nodiscard]] uint8_t getFoodSelected(FoodID id);
    [[nodiscard]] const Food& getFood(FoodID id);
}

#endif //KHAUPHANDINHDUONG_DATABASE_HPP
