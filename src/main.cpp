#include <iostream>
#include <fstream>
#include <string>
#include "csv_utils.h"
#include "validator.h"
#include "enrich.h"

namespace {

void print_usage(const char* prog_name) {
    std::cerr << "Usage: " << prog_name
              << " <input.csv> <output.csv> [cluster_threshold_meters]\n"
              << "  input.csv    CSV with header: id,name,building,address,lat,lng\n"
              << "  output.csv   where the cleaned + enriched CSV will be written\n"
              << "  cluster_threshold_meters  optional, default 100\n";
}

void write_report(const std::string& path, const std::vector<ValidationIssue>& issues) {
    std::ofstream file(path);
    file << "LiftSpot data validation report\n";
    file << "Total issues found: " << issues.size() << "\n\n";
    for (const auto& issue : issues) {
        file << "[" << issue.type << "] id=" << issue.elevator_id
             << " -- " << issue.detail << "\n";
    }
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];
    double cluster_threshold_m = (argc >= 4) ? std::stod(argv[3]) : 100.0;

    try {
        std::vector<Elevator> elevators = read_elevators_csv(input_path);
        std::cout << "Loaded " << elevators.size() << " elevator records from "
                  << input_path << "\n";

        std::vector<ValidationIssue> issues = validate_elevators(elevators);
        std::cout << "Found " << issues.size() << " validation issue(s)\n";

        std::string report_path = output_path + ".report.txt";
        write_report(report_path, issues);
        std::cout << "Wrote validation report to " << report_path << "\n";

        compute_nearest_neighbors(elevators);
        assign_clusters(elevators, cluster_threshold_m);

        write_elevators_csv(output_path, elevators);
        std::cout << "Wrote cleaned + enriched data to " << output_path << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
