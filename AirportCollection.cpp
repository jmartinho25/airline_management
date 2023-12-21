#include <vector>
#include <string>
#include "Airport.h"
#include "Flight.h"

using namespace std;

class AirportCollection {
private:
    vector<Airport*> airports;

public:
    AirportCollection() {}

    void addAirport(Airport* airport) {
        airports.push_back(airport);
    }

    Airport* getAirport(string code) {
        for (Airport* airport : airports) {
            if (airport->getCode() == code) {
                return airport;
            }
        }

        return nullptr;
    }

    vector<Airport*> getAllAirports() {
        return airports;
    }

    vector<Airport*> getAllAirportsInCountry(string country) {
        vector<Airport*> matchingAirports;

        for (Airport* airport : airports) {
            if (airport->getCountry() == country) {
                matchingAirports.push_back(airport);
            }
        }

        return matchingAirports;
    }

    vector<Airport*> getAllAirportsConnectedToAirport(string code) {
        vector<Airport*> connectedAirports;

        for (Airport* airport : airports) {
            for (Flight* flight : airport->getFlights()) {
                if (flight->getSourceAirportCode() == code) {
                    connectedAirports.push_back(airport);
                    break;
                } else if (flight->getTargetAirportCode() == code) {
                    connectedAirports.push_back(airport);
                    break;
                }
            }
        }

        return connectedAirports;
    }
};