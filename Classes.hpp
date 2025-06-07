#pragma once
#include <functional>
#include <string>
#include <stdexcept>
#include <memory>
#include <random>
#include <iostream>
#include "gamestuff.h"




class Building//dungeon
{	

};
class Tile {
    friend class Room;
	Unit* unit = nullptr; //pointer to the unit on this tile, if any

public:
	enum class TileType {
		Empty, //no tile
		Floor, //floor tile
		Wall, //wall tile
		Door, //door tile
		StairsUp, //stairs up tile
		StairsDown, //stairs down tile
	};
private:     TileType type;
public:
	TileType getType() const { return type; } // Get the type of the tile
	Tile() : type(TileType::Empty), unit(nullptr) {} // Default constructor
	Tile(TileType t) : type(t), unit(nullptr) {}
};


class Room {
public:
    std::vector<std::vector<Tile>> tiles; // 2D array of Tile objects

    // Random number generation setup
    std::mt19937 rng; // Mersenne Twister engine

    Room(int initial_width, int initial_height) {
        // Seed the random number generator
        std::random_device rd;
        rng.seed(rd());

        // Initialize the tiles vector with Empty tiles
        // We'll fill it with Floor/Wall/Door during partitioning
        tiles.resize(initial_height); // Height (rows)
        for (int i = 0; i < initial_height; ++i) {
            tiles[i].resize(initial_width, Tile(Tile::TileType::Empty)); // Width (columns)
        }

        // Start the partitioning process
        //std::cout << "Starting partition for room of " << initial_width << "x" << initial_height << "\n";
        partition(0, 0, initial_width, initial_height);
    }

    // A helper to draw the room (for debugging)
    void printRoom() const {
        std::cout << "Room State:\n";
        for (const auto& row : tiles) {
            for (const auto& tile : row) {
                switch (tile.getType()) {
                case Tile::TileType::Empty:      std::cout << "  "; break;
                case Tile::TileType::Floor:      std::cout << ". "; break;
                case Tile::TileType::Wall:       std::cout << "# "; break;
                case Tile::TileType::Door:       std::cout << "+ "; break;
                case Tile::TileType::StairsUp:   std::cout << "< "; break;
                case Tile::TileType::StairsDown: std::cout << "> "; break;
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    bool Drawstuff() {

    }
    

private:
    // This is the core partitioning function, its like BSP if you removed the tree and just had the result
    // x, y: top-left corner of the current region
    // width, height: dimensions of the current region
    void partition(int x, int y, int width, int height) {
        static bool has_down_stairs_been_placed = false;
        static bool has_up_stairs_been_placed = false;

        // Base Case: If the room is too small, fill it with floor and potentially stairs
        const int MIN_ROOM_SIZE = 5; // Minimum size for a room segment
        if (width <= MIN_ROOM_SIZE || height <= MIN_ROOM_SIZE) {
            // Fill this entire segment with floor
            for (int i = y; i < y + height; ++i) {
                for (int j = x; j < x + width; ++j) {
                    if (i >= 0 && i < tiles.size() && j >= 0 && j < tiles[0].size()) {
                        tiles[i][j].type = Tile::TileType::Floor;
                    }
                }
            }

            // Add stairs (randomly, to one of the base cases)
            // probably want a more sophisticated way to place stairs (e.g., only one pair per level)
            std::uniform_int_distribution<> stairs_dist(0, 1);
            if (stairs_dist(rng) == 0) { // Randomly pick up or down stairs
                std::uniform_int_distribution<> rand_x(x + 1, x + width - 2); // Avoid edges
                std::uniform_int_distribution<> rand_y(y + 1, y + height - 2); // Avoid edges
                int stair_x = rand_x(rng);
                int stair_y = rand_y(rng);
                if (stair_y >= 0 && stair_y < tiles.size() && stair_x >= 0 && stair_x < tiles[0].size()) {
                    tiles[stair_y][stair_x].type = (stairs_dist(rng) == 0) ? Tile::TileType::StairsUp : Tile::TileType::StairsDown;
                }
            }
            return;
        }

        // Decide to split horizontally or vertically
        std::uniform_int_distribution<> split_orientation_dist(0, 1); // 0 for horizontal, 1 for vertical
        bool split_horizontally = (width > height) ? true : // Prefer horizontal if wider
            (height > width) ? false : // Prefer vertical if taller
            (split_orientation_dist(rng) == 0); // Random if roughly square

        if (split_horizontally) {
            // Pick a random column for the wall (between 1 and width-2 to ensure sub-regions are valid)
            // Ensure there's enough space for the wall itself and two sub-rooms
            int min_wall_x = x + MIN_ROOM_SIZE / 2;
            int max_wall_x = x + width - MIN_ROOM_SIZE / 2;
            if (min_wall_x >= max_wall_x) { // Prevent infinite recursion if room is too small
                // Fallback to base case if no valid split point
                partition(x, y, width, height);
                return;
            }
            std::uniform_int_distribution<> wall_col_dist(min_wall_x, max_wall_x);
            int wall_col = wall_col_dist(rng);

            // Draw the vertical wall
            for (int i = y; i < y + height; ++i) {
                if (i >= 0 && i < tiles.size() && wall_col >= 0 && wall_col < tiles[0].size()) {
                    tiles[i][wall_col].type = Tile::TileType::Wall;
                }
            }

            // Place a door in the wall
            // Ensure door is not on the very edge of the wall segment
            int door_min_y = y + 1;
            int door_max_y = y + height - 2;
            if (door_min_y < door_max_y) { // Only place door if segment is large enough
                std::uniform_int_distribution<> door_row_dist(door_min_y, door_max_y);
                int door_row = door_row_dist(rng);
                if (door_row >= 0 && door_row < tiles.size() && wall_col >= 0 && wall_col < tiles[0].size()) {
                    tiles[door_row][wall_col].type = Tile::TileType::Door;
                }
            }


            // Recursively call partition for the two new sub-regions
            partition(x, y, wall_col - x, height); // Left sub-region
            partition(wall_col + 1, y, width - (wall_col - x) - 1, height); // Right sub-region

        }
        else { // Split vertically
            // Pick a random row for the wall
            int min_wall_y = y + MIN_ROOM_SIZE / 2;
            int max_wall_y = y + height - MIN_ROOM_SIZE / 2;
            if (min_wall_y >= max_wall_y) { // Prevent infinite recursion
                partition(x, y, width, height);
                return;
            }
            std::uniform_int_distribution<> wall_row_dist(min_wall_y, max_wall_y);
            int wall_row = wall_row_dist(rng);

            // Draw the horizontal wall
            for (int j = x; j < x + width; ++j) {
                if (wall_row >= 0 && wall_row < tiles.size() && j >= 0 && j < tiles[0].size()) {
                    tiles[wall_row][j].type = Tile::TileType::Wall;
                }
            }

            // Place a door in the wall
            int door_min_x = x + 1;
            int door_max_x = x + width - 2;
            if (door_min_x < door_max_x) { // Only place door if segment is large enough
                std::uniform_int_distribution<> door_col_dist(door_min_x, door_max_x);
                int door_col = door_col_dist(rng);
                if (wall_row >= 0 && wall_row < tiles.size() && door_col >= 0 && door_col < tiles[0].size()) {
                    tiles[wall_row][door_col].type = Tile::TileType::Door;
                }
            }


            // Recursively call partition for the two new sub-regions
            partition(x, y, width, wall_row - y); // Top sub-region
            partition(x, wall_row + 1, width, height - (wall_row - y) - 1); // Bottom sub-region
        }
    }
};

