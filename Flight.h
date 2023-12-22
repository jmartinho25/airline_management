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
};
#endif // FLIGHT_