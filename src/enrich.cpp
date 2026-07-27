#include "enrich.h"
#include "geo_utils.h"
#include <numeric>
#include <unordered_map>

void compute_nearest_neighbors(std::vector<Elevator>& elevators) {
    for (size_t i = 0; i < elevators.size(); i++) {
        if (!elevators[i].has_coords) continue;

        double best_dist = -1.0;
        std::string best_id;

        for (size_t j = 0; j < elevators.size(); j++) {
            if (i == j || !elevators[j].has_coords) continue;

            double dist = haversine_distance_m(elevators[i].lat, elevators[i].lng,
                                                elevators[j].lat, elevators[j].lng);
            if (best_dist < 0 || dist < best_dist) {
                best_dist = dist;
                best_id = elevators[j].id;
            }
        }

        elevators[i].nearest_id = best_id;
        elevators[i].nearest_distance_m = best_dist;
    }
}

namespace {

// Simple union-find (disjoint set union) over elevator indices.
struct UnionFind {
    std::vector<int> parent;

    explicit UnionFind(size_t n) : parent(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // path compression
            x = parent[x];
        }
        return x;
    }

    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra != rb) parent[ra] = rb;
    }
};

} // namespace

void assign_clusters(std::vector<Elevator>& elevators, double cluster_threshold_m) {
    UnionFind uf(elevators.size());

    // Union any pair of coordinate-bearing elevators within the threshold.
    for (size_t i = 0; i < elevators.size(); i++) {
        if (!elevators[i].has_coords) continue;
        for (size_t j = i + 1; j < elevators.size(); j++) {
            if (!elevators[j].has_coords) continue;
            double dist = haversine_distance_m(elevators[i].lat, elevators[i].lng,
                                                elevators[j].lat, elevators[j].lng);
            if (dist <= cluster_threshold_m) {
                uf.unite(static_cast<int>(i), static_cast<int>(j));
            }
        }
    }

    // Map each root index to a readable cluster label.
    std::unordered_map<int, std::string> root_to_label;
    int next_cluster = 0;

    for (size_t i = 0; i < elevators.size(); i++) {
        if (!elevators[i].has_coords) {
            elevators[i].cluster_id = "unclustered_no_coords";
            continue;
        }
        int root = uf.find(static_cast<int>(i));
        auto it = root_to_label.find(root);
        if (it == root_to_label.end()) {
            std::string label = "cluster_" + std::to_string(next_cluster++);
            root_to_label[root] = label;
            elevators[i].cluster_id = label;
        } else {
            elevators[i].cluster_id = it->second;
        }
    }
}
