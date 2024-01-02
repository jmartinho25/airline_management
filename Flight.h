#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>

/**
    * @brief Class to store the information about a given Flight
    */
class Flight {
private:
    /** @brief Code of the  source airport of the flight*/
    std::string sourceAirportCode;
    /** @brief Code of the destination airport of the flight*/
    std::string targetAirportCode;
    /** @brief Code of the airline responsible for the flight*/
    std::string airlineCode;

public:
    /**
    * @brief Returns the code of the airline responsible for the flight
    * @details Time complexity: O(1)
    * @return code of the airline responsible for the flight
    */
    std::string getAirlineCode() const { return airlineCode; }

    /**
    * @brief Returns if two flights are the same or not by comparing their source and destination airports, as well as their airline
    * @details Time complexity: O(1)
    * @return True if they have the same source and destination airports, as well as the same airline, false otherwise
    */
    bool operator==(const Flight &other) const {
        return sourceAirportCode == other.sourceAirportCode &&
               targetAirportCode == other.targetAirportCode &&
               airlineCode == other.airlineCode;
    }
};

#endif // FLIGHT_H