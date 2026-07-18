#include "common/database.hpp"
#include <boost/unordered/unordered_flat_map.hpp>
#include "common/io.hpp"
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <magic_enum/magic_enum.hpp>
#include "common/logging.hpp"

boost::unordered_flat_map<FoodID, Food> m_food_data;
boost::unordered_flat_map<FoodID, uint8_t> m_food_select;

void MFCPP::Database::print(FoodID id) {
    const Food& dat = m_food_data[id];
    Log::InfoPrint(fmt::format("--- {} ---", dat.name));
    Log::InfoPrint(fmt::format("Calories: {}kcal", dat.kcal));
    Log::InfoPrint(fmt::format("Protein: {}g", dat.protein));
    Log::InfoPrint(fmt::format("Lipid: {}g", dat.lipid));
    Log::InfoPrint(fmt::format("Carbonhydrate: {}g", dat.carbohydrate));
    Log::InfoPrint(fmt::format("Price: {}k VND", dat.price));
    Log::InfoPrint("--- ---");
}

void MFCPP::Database::LoadFood(const std::filesystem::path &path) {
    std::string input;
    IO::LoadJSON(input, path);

    nlohmann::json foodJson;
    try {
        foodJson = nlohmann::json::parse(input);
    }
    catch (nlohmann::json::parse_error& e) {
        throw std::runtime_error(fmt::format("Database: Failed to parse {} file: {}", path.string(), e.what()));
    }
    std::string food_enum_string = foodJson.value("id", "");
    auto food_enum = magic_enum::enum_cast<FoodID>(food_enum_string, magic_enum::case_insensitive);
    if (!food_enum.has_value()) {
        throw std::runtime_error(fmt::format("Database: ID {} does not exist", food_enum_string));
    }
    Food inp;
    inp.name = foodJson.value("name",  "Null");
    inp.kcal = foodJson.value("kcal", 0.0);
    inp.protein = foodJson.value("protein", 0.0);
    inp.lipid = foodJson.value("lipid", 0.0);
    inp.carbohydrate = foodJson.value("carbohydrate", 0.0);
    inp.price = foodJson.value("price", 0.0);
    inp.food_id = food_enum.value();
    Log::InfoPrint(fmt::format("Success loaded {}", path.string()));
    m_food_data[food_enum.value()] = inp;
    m_food_select[food_enum.value()] = 0;
    print(food_enum.value());
}

void MFCPP::Database::Init() {
    std::string dataPhrase;
    IO::LoadJSON(dataPhrase, "data/food.json");

    nlohmann::json foodJson;
    try {
        foodJson = nlohmann::json::parse(dataPhrase);
    }
    catch (nlohmann::json::parse_error& e) {
        throw std::runtime_error(fmt::format("Level: Failed to parse food.json file: {}", e.what()));
    }
    Log::SuccessPrint("Success Loaded jsonData food.json");
    const nlohmann::json& datajson = foodJson["data"];
    for (const auto &data : datajson) {
        const std::string jsonPath = data.at("data_load").get<std::string>();
        LoadFood(fmt::format("data/{}", jsonPath));
    }

    //Check integrity
    constexpr auto food_entries = magic_enum::enum_entries<FoodID>();
    for (const auto& food_entry : food_entries) {
        if (food_entry.first != FoodID::FOOD_NULL && !m_food_data.contains(food_entry.first))
            throw std::runtime_error(fmt::format("Database: ID {} does not loaded when checking integrity", food_entry.second));
    }
}
void MFCPP::Database::clearSelect() {
    for (auto &i : m_food_select)
        i.second = 0;
}

void MFCPP::Database::setFoodSelected(FoodID id, bool val) {
    if (!m_food_select.contains(id)) throw std::runtime_error(fmt::format("Database: ID {} does not exist", magic_enum::enum_name(id)));
    m_food_select[id] = static_cast<uint8_t>(val);
}

uint8_t MFCPP::Database::getFoodSelected(FoodID id) {
    if (!m_food_select.contains(id)) throw std::runtime_error(fmt::format("Database: ID {} does not exist", magic_enum::enum_name(id)));
    return m_food_select[id];
}

const Food& MFCPP::Database::getFood(FoodID id) {
    if (!m_food_data.contains(id)) throw std::runtime_error(fmt::format("Database: ID {} does not exist", magic_enum::enum_name(id)));
    return m_food_data[id];
}
void MFCPP::Database::setAllFoodSelected(bool val) {
    constexpr auto food_entries = magic_enum::enum_entries<FoodID>();
    for (const auto &i : food_entries) {
        if (i.first == FoodID::FOOD_NULL) continue;
        m_food_select[i.first] = static_cast<uint8_t>(val);
    }
}