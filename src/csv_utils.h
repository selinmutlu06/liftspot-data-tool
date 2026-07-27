#pragma once
#include <string>
#include <vector>
#include "elevator.h"

// Reads a CSV with header row: id,name,building,address,lat,lng
// lat/lng may be empty strings -> has_coords = false.
std::vector<Elevator> read_elevators_csv(const std::string& path);

// Writes cleaned + enriched elevators back out as CSV.
void write_elevators_csv(const std::string& path, const std::vector<Elevator>& elevators);
