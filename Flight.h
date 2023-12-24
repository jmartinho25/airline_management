#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>

class Flight {
private:
    std::string sourceAirportCode;
    std::string targetAirportCode;
    std::string airlineCode;

public:
    Flight(std::string sourceAirportCode, std::string targetAirportCode, std::string airlineCode) : sourceAirportCode(sourceAirportCode), targetAirportCode(targetAirportCode), airlineCode(airlineCode) {}
    std::string getSourceAirportCode() const { return sourceAirportCode; }
    std::string getTargetAirportCode() const { return targetAirportCode; }
    std::string getAirlineCode() const { return airlineCode; }
    bool operator==(const Flight& other) const {
        return sourceAirportCode == other.sourceAirportCode &&
               targetAirportCode == other.targetAirportCode &&
               airlineCode == other.airlineCode;
    }
    };
#endif // FLIGHT_H