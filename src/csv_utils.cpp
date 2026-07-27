#include "csv_utils.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {

// Splits a single CSV line on commas, respecting simple double-quoted fields.
std::vector<std::string> split_csv_line(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    bool in_quotes = false;

    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            fields.push_back(current);
            current.clear();
        } else {
            current += c;
        }
    }
    fields.push_back(current);
    return fields;
}

std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Wraps a field in quotes if it contains a comma, quote, or newline,
// per standard CSV escaping rules.
std::string csv_escape(const std::string& field) {
    bool needs_quotes = field.find_first_of(",\"\n\r") != std::string::npos;
    if (!needs_quotes) return field;

    std::string escaped = "\"";
    for (char c : field) {
        if (c == '"') escaped += "\"\""; // double up embedded quotes
        else escaped += c;
    }
    escaped += "\"";
    return escaped;
}

} // namespace

std::vector<Elevator> read_elevators_csv(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open input file: " + path);
    }

    std::vector<Elevator> elevators;
    std::string line;
    bool first_line = true;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Skip the header row.
        if (first_line) {
            first_line = false;
            continue;
        }

        auto fields = split_csv_line(line);
        // Expect: id,name,building,address,lat,lng
        while (fields.size() < 6) fields.push_back("");

        Elevator e;
        e.id = trim(fields[0]);
        e.name = trim(fields[1]);
        e.building = trim(fields[2]);
        e.address = trim(fields[3]);

        std::string lat_str = trim(fields[4]);
        std::string lng_str = trim(fields[5]);

        if (!lat_str.empty() && !lng_str.empty()) {
            try {
                e.lat = std::stod(lat_str);
                e.lng = std::stod(lng_str);
                e.has_coords = true;
            } catch (...) {
                e.has_coords = false;
            }
        }

        elevators.push_back(e);
    }

    return elevators;
}

void write_elevators_csv(const std::string& path, const std::vector<Elevator>& elevators) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open output file: " + path);
    }

    file << "id,name,building,address,lat,lng,cluster_id,nearest_id,nearest_distance_m\n";
    for (const auto& e : elevators) {
        file << csv_escape(e.id) << "," << csv_escape(e.name) << "," << csv_escape(e.building)
             << "," << csv_escape(e.address) << ","
             << (e.has_coords ? std::to_string(e.lat) : "") << ","
             << (e.has_coords ? std::to_string(e.lng) : "") << ","
             << e.cluster_id << "," << e.nearest_id << ","
             << (e.nearest_distance_m >= 0 ? std::to_string(e.nearest_distance_m) : "")
             << "\n";
    }
}
