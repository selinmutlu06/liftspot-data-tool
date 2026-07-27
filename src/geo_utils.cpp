#include "geo_utils.h"
#include <cmath>

namespace {
constexpr double kEarthRadiusM = 6371000.0;

double to_radians(double degrees) {
    return degrees * M_PI / 180.0;
}
}

double haversine_distance_m(double lat1, double lng1, double lat2, double lng2) {
    double phi1 = to_radians(lat1);
    double phi2 = to_radians(lat2);
    double d_phi = to_radians(lat2 - lat1);
    double d_lambda = to_radians(lng2 - lng1);

    double a = std::sin(d_phi / 2) * std::sin(d_phi / 2) +
               std::cos(phi1) * std::cos(phi2) *
               std::sin(d_lambda / 2) * std::sin(d_lambda / 2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));

    return kEarthRadiusM * c;
}
