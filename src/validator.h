#pragma once
#include <string>
#include <vector>
#include "elevator.h"

struct ValidationIssue {
    std::string elevator_id;
    std::string type;    // e.g. "missing_coords", "duplicate_id", "malformed_address"
    std::string detail;
};

// Checks every elevator for missing coords, out-of-range coords,
// malformed addresses, and duplicate ids/coords.
// Does not modify the input.
std::vector<ValidationIssue> validate_elevators(const std::vector<Elevator>& elevators);
