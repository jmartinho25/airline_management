#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <vector>
#include "Flight.h"

class Airport {
private:
    std::string code;
    std::string name;
    std::string city;
    std::string country;
    double latitude;
    double longitude;

public:
    Airport(std::string code, std::string name, std::string city, std::string country, double latitude,
            double longitude) : code(code), name(name), city(city), country(country), latitude(latitude),
                                longitude(longitude) {}

    std::string getCode() const { return code; }

    std::string getName() const { return name; }

    std::string getCity() const { return city; }

    std::string getCountry() const { return country; }

    double getLatitude() const { return latitude; }

    double getLongitude() const { return longitude; }

    bool operator==(const Airport &other) const {
        return code == other.code;
    }
};

#endif // AIRPORT_H


