#ifndef AIRPORT_H
#define AIRPORT_H

#include <string>
#include <vector>
#include "Flight.h"

/**
    * @brief Class to store the information about a given Airport
    */
class Airport {
private:
    /** @brief Code of the airport*/
    std::string code;
    /** @brief Name of the airline*/
    std::string name;
    /** @brief City of the airline*/
    std::string city;
    /** @brief Country of the airline*/
    std::string country;
    /** @brief Latitude of the airline*/
    double latitude;
    /** @brief Longitude of the airline*/
    double longitude;

public:
    /**
    * @brief Constructor of the Airport class. Code, name, city, country, latitude and longitude  correspond to the given values
    * @details Time complexity: O(1)
    * @param code Airport code
    * @param name Airport name
    * @param city Airport city
    * @param country Airport country
    * @param latitude Airport latitude
    * @param longitude Airport longitude
    */
    Airport(std::string code, std::string name, std::string city, std::string country, double latitude,
            double longitude) : code(code), name(name), city(city), country(country), latitude(latitude),
                                longitude(longitude) {}

    /**
    * @brief Returns the code of the airport
    * @details Time complexity: O(1)
    * @return Code of the airport
    */
    std::string getCode() const { return code; }

    /**
    * @brief Returns the name of the airport
    * @details Time complexity: O(1)
    * @return Name of the airport
    */
    std::string getName() const { return name; }

    /**
    * @brief Returns the city of the airport
    * @details Time complexity: O(1)
    * @return City of the airport
    */
    std::string getCity() const { return city; }

    /**
    * @brief Returns the country of the airport
    * @details Time complexity: O(1)
    * @return Country of the airport
    */
    std::string getCountry() const { return country; }

    /**
    * @brief Returns the latitude of the airport
    * @details Time complexity: O(1)
    * @return Latitude of the airport
    */
    double getLatitude() const { return latitude; }

    /**
    * @brief Returns the longitude of the airport
    * @details Time complexity: O(1)
    * @return Longitude of the airport
    */
    double getLongitude() const { return longitude; }

    /**
    * @brief Returns if two airports are the same or not by comparing their airport code
    * @details Time complexity: O(1)
    * @return True if they have the same airport code (same airport) or false if they don´t have the same airport code (different airport)
    */
    bool operator==(const Airport &other) const {
        return code == other.code;
    }
};

#endif // AIRPORT_H


