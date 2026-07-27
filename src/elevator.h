#pragma once
#include <string>

// One row of elevator data.
struct Elevator {
    std::string id;
    std::string name;
    std::string building;
    std::string address;
    double lat = 0.0;
    double lng = 0.0;
    bool has_coords = false;

    // Filled in during enrichment, not read from input.
    std::string cluster_id;
    std::string nearest_id;
    double nearest_distance_m = -1.0;
};
