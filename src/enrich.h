#pragma once
#include <vector>
#include "elevator.h"

// For every elevator with coordinates, finds its nearest neighbor
// (by haversine distance) and fills in nearest_id / nearest_distance_m.
void compute_nearest_neighbors(std::vector<Elevator>& elevators);

// Groups elevators into clusters using union-find: any two elevators
// within cluster_threshold_m of each other end up in the same cluster.
// Fills in cluster_id (e.g. "cluster_0", "cluster_1", ...).
void assign_clusters(std::vector<Elevator>& elevators, double cluster_threshold_m);
