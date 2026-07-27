#include "validator.h"
#include <unordered_map>
#include <unordered_set>

std::vector<ValidationIssue> validate_elevators(const std::vector<Elevator>& elevators) {
    std::vector<ValidationIssue> issues;

    std::unordered_set<std::string> seen_ids;
    std::unordered_map<std::string, std::string> seen_coords; // "lat,lng" -> id

    for (const auto& e : elevators) {
        // Missing or malformed id.
        if (e.id.empty()) {
            issues.push_back({e.id, "missing_id", "row has no id"});
        } else if (seen_ids.count(e.id)) {
            issues.push_back({e.id, "duplicate_id", "id already seen"});
        } else {
            seen_ids.insert(e.id);
        }

        // Missing coordinates.
        if (!e.has_coords) {
            issues.push_back({e.id, "missing_coords", "lat/lng missing or unparseable"});
        } else {
            // Out-of-range coordinates.
            if (e.lat < -90.0 || e.lat > 90.0 || e.lng < -180.0 || e.lng > 180.0) {
                issues.push_back({e.id, "invalid_coords", "lat/lng out of valid range"});
            }

            // Duplicate coordinates (rounded to ~1 meter precision).
            char buf[64];
            snprintf(buf, sizeof(buf), "%.5f,%.5f", e.lat, e.lng);
            std::string key(buf);
            auto it = seen_coords.find(key);
            if (it != seen_coords.end()) {
                issues.push_back({e.id, "duplicate_coords",
                                   "same coordinates as " + it->second});
            } else {
                seen_coords[key] = e.id;
            }
        }

        // Malformed / missing address.
        if (e.address.empty()) {
            issues.push_back({e.id, "missing_address", "address field is empty"});
        } else if (e.address.size() < 5) {
            issues.push_back({e.id, "malformed_address", "address looks too short to be real"});
        }

        // Missing building name (used later for clustering).
        if (e.building.empty()) {
            issues.push_back({e.id, "missing_building", "building field is empty"});
        }
    }

    return issues;
}
