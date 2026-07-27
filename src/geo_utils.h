#pragma once

// Great-circle distance between two lat/lng points, in meters.
// Uses the haversine formula.
double haversine_distance_m(double lat1, double lng1, double lat2, double lng2);
