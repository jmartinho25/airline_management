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
    * @brief Constructor of the Flight class. SourceAirportCode, targetAirportCode and airlineCode correspond to the given values
    * @details Time complexity: O(1)
    * @param sourceAirportCode Code of the source airport of the flight
    * @param targetAirportCode Code of the destination airport of the flight
    * @param airlineCode Code of the airline responsible for the flight
    */
    Flight(std::string sourceAirportCode, std::string targetAirportCode, std::string airlineCode) : sourceAirportCode(
            sourceAirportCode), targetAirportCode(targetAirportCode), airlineCode(airlineCode) {}

    /**
    * @brief Returns the code of the source airport of the flight
    * @details Time complexity: O(1)
    * @return code of the source airport of the flight
    */
    std::string getSourceAirportCode() const { return sourceAirportCode; }

    /**
    * @brief Returns the code of the destination airport of the flight
    * @details Time complexity: O(1)
    * @return code of the destination airport of the flight
    */
    std::string getTargetAirportCode() const { return targetAirportCode; }

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