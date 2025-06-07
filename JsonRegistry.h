#pragma once // Or use traditional include guards if preferred

#include "veryslowjson.hpp" // Keep this as you specified
#include "Units.hpp"        // For Monster class definition
#include <fstream>
#include <vector>
#include <string>
#include <random>           // For modern C++ random
#include <algorithm>        // For std::remove_if if we filter
#include <stdexcept>        // For exceptions like runtime_error
#include <iostream>         // For debugging output
#include "./data/tile_sprites.hpp"
using json = nlohmann::json;

// Helper struct to hold parsed monster data before creating a Monster instance
struct MonsterData {
    std::string id;
    std::string name; // Direct string for convenience
    uint16_t hp;
    uint16_t speed;
    uint16_t morale;
    uint16_t damage_amount; // First melee damage amount
    uint16_t sprite_lookup_index; // Placeholder for now, assuming you have a way to map ID to sprite

    // Calculated difficulty based on Monster::getdifficulty() logic
    int calculated_difficulty;

    // A simple constructor for easier initialization
    MonsterData(std::string_view _id, std::string_view _name, uint16_t _hp, uint16_t _speed,
        uint16_t _morale, uint16_t _damage, uint16_t _sprite_idx = 0)
        : id(_id), name(_name), hp(_hp), speed(_speed), morale(_morale),
        damage_amount(_damage), sprite_lookup_index(_sprite_idx) {
        // Calculate difficulty using the same formula as Monster::getdifficulty()
        calculated_difficulty = (hp / 10) + speed + morale / 10 + /*dodge + vision_day * 2 +*/ damage_amount * 5;
        // Note: dodge and vision_day are missing from this struct for now,
        // so the calculated difficulty might not precisely match a full Monster object's
        // if those fields were populated. Adjust MonsterData to include them if needed.
    }
};

class Monsterjsonregistry {
private:
    static json data;
    //static json sprite_json;
    static bool read; // Flag to ensure data is read only once
    static std::vector<MonsterData> parsedMonsters; // Store parsed data

    // Random number generator for picking monsters
    static std::mt19937 rng;
    static bool rng_seeded;

    // Internal helper to get a random sprite lookup index for a given monster ID
    // You'll need to define how monster IDs map to sprite lookup indices.
    // For now, it's just a dummy.
    uint16_t getSpriteLookupForMonsterId(const std::string& monster_id) {
        auto it = tile_id_to_fg_map.find(monster_id);

        if (it != tile_id_to_fg_map.end()) {
            // Found it, return the associated sprite ID
            return it->second;
        }
        else {
            // Not found, return a random sprite ID
            // Define a reasonable range for random sprite IDs.
            // A typical sprite atlas might have thousands of sprites, e.g., 0 to 30000.
            // You can adjust this max value based on your actual tileset size.
            const uint16_t MIN_SPRITE_ID = 0;
            const uint16_t MAX_SPRITE_ID = 10000; // Example max. Adjust as needed.

            // Create a distribution for the random range
            std::uniform_int_distribution<uint16_t> dist(MIN_SPRITE_ID, MAX_SPRITE_ID);

            uint16_t random_sprite_id = dist(rng); // Generate random number using the static RNG
            std::cerr << "Warning: Monster ID '" << monster_id << "' not found in tile_id_to_fg_map. Returning random sprite ID: " << random_sprite_id << "\n";
            return random_sprite_id;
        }
    }

public:
    Monsterjsonregistry() {
        if (!read) {
            std::ifstream f("./data/jiban_ate.json");

            if (!f.is_open()  ){
                throw std::runtime_error("Failed to open jsons");
            }
            try {
                f >> data;
                
                f.close();
                read = true;
                std::cout << "Monsterjsonregistry: JSON data loaded successfully.\n";
                parseAllMonsters(); // Parse data immediately after loading
            }
            catch (const json::parse_error& e) {
                std::cerr << "Monsterjsonregistry: JSON parse error: " << e.what() << '\n';
                throw; // Re-throw to indicate critical failure
            }
            catch (const std::exception& e) {
                std::cerr << "Monsterjsonregistry: An unexpected error occurred: " << e.what() << '\n';
                throw;
            }

            // Seed RNG only once
            if (!rng_seeded) {
                std::random_device rd;
                rng.seed(rd());
                rng_seeded = true;
            }
        }
    }

    // This method parses all monster entries from the loaded JSON
    void parseAllMonsters() {
        if (!data.is_array()) {
            std::cerr << "Monsterjsonregistry: JSON data is not a root array. Cannot parse monsters.\n";
            return;
        }

        parsedMonsters.clear(); // Clear any previous parses
        for (const auto& monster_json : data) {
            try {
                if (monster_json.contains("type") && monster_json["type"] == "MONSTER") {
                    std::string id = monster_json["id"].get<std::string>();
                    std::string name_str = monster_json["name"]["str"].get<std::string>();
                    uint16_t hp = monster_json["hp"].get<uint16_t>();
                    uint16_t speed = monster_json["speed"].get<uint16_t>();
                    uint16_t morale = monster_json["morale"].get<uint16_t>();

                    // Get the first melee_damage amount
                    uint16_t damage_amount = 0;
                    if (monster_json.contains("melee_damage") && monster_json["melee_damage"].is_array() && !monster_json["melee_damage"].empty()) {
                        damage_amount = monster_json["melee_damage"][0]["amount"].get<uint16_t>();
                    }

                    uint16_t sprite_idx = getSpriteLookupForMonsterId(id); // Get a sprite index

                    parsedMonsters.emplace_back(id, name_str, hp, speed, morale, damage_amount, sprite_idx);
                }
            }
            catch (const json::exception& e) {
                std::cout << "Monsterjsonregistry: Error parsing monster entry: " << e.what() << '\n';
                // You might choose to skip this entry or log more info
            }
        }
        std::cout << "Monsterjsonregistry: Parsed " << parsedMonsters.size() << " monster definitions.\n";
    }

    // Get a monster appropriate to difficulty
 
    Monster* get_monster(int target_difficulty, POS spawn_pos, int difficulty_tolerance = 10) {
        std::vector<const MonsterData*> suitable_monsters;

        // Find monsters within the difficulty range
        for (const auto& m_data : parsedMonsters) {
            int lower_bound = target_difficulty - difficulty_tolerance;
            int upper_bound = target_difficulty + difficulty_tolerance;
            if (m_data.calculated_difficulty >= lower_bound && m_data.calculated_difficulty <= upper_bound) {
                suitable_monsters.push_back(&m_data);
            }
        }

        if (suitable_monsters.empty()) {
            std::cerr << "Monsterjsonregistry: No suitable monster found for difficulty " << target_difficulty
                << " (tolerance " << difficulty_tolerance << "). Returning nullptr.\n";
            return nullptr; // No monster found for this difficulty
        }

        // Pick a random monster from the suitable ones
        std::uniform_int_distribution<> dist(0, suitable_monsters.size() - 1);
        const MonsterData* chosen_data = suitable_monsters[dist(rng)];

        std::cout << "Monsterjsonregistry: Creating monster '" << chosen_data->name << "' (ID: "
            << chosen_data->id << ", Diff: " << chosen_data->calculated_difficulty
            << ") for target difficulty " << target_difficulty << ".\n";

        // Construct and return a unique_ptr to the Monster
        // Make sure your Monster constructor can accept these parameters.
        Monster(
            spawn_pos,
            chosen_data->sprite_lookup_index,
            chosen_data->hp,
            chosen_data->speed,
            chosen_data->morale,
            chosen_data->damage_amount,
            chosen_data->name,
            chosen_data->id // Pass id if Monster needs it
        );
    }
};

// Static member definitions (usually in a .cpp file)
json Monsterjsonregistry::data;
bool Monsterjsonregistry::read = false;
std::vector<MonsterData> Monsterjsonregistry::parsedMonsters;
std::mt19937 Monsterjsonregistry::rng;
bool Monsterjsonregistry::rng_seeded = false;