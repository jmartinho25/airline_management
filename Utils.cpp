// Utils.cpp
#include "Utils.h"
#include <cmath>

const double EARTH_RADIUS_KM = 6371.0;

/**
 * @brief Transforms an angle from degrees to radians
 * @details Time complexity: O(1)
 * @param degrees degrees of the given angle
 * @return The angle in radians
 */
double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

/**
 * @brief Calculates the distance between two geographical points (using their coordinates)
 * @details Time complexity: O(1)
 * @param lat1 latitude of the first geographical point
 * @param lon1 longitude of the first geographical point
 * @param lat2 latitude of the second geographical point
 * @param lon2 longitude of the second geographical point
 * @return Distance between the two points
 */
double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    lat1 = degreesToRadians(lat1);
    lon1 = degreesToRadians(lon1);
    lat2 = degreesToRadians(lat2);
    lon2 = degreesToRadians(lon2);

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = pow(sin(dLat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dLon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return EARTH_RADIUS_KM * c;
}


