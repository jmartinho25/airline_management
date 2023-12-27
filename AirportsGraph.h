//
// Created by darksystem on 22-12-2023.
//

#ifndef PROJECT_2_AIRPORTSGRAPH_H
#define PROJECT_2_AIRPORTSGRAPH_H


#include <unordered_set>
#include "Airport.h"
#include "Graph.h"
#include "Airlines.h"

class AirportsGraph {
private:
    Graph<Airport> airportsGraph;
    std::vector<Airline> airlines;

public:
    bool addAirport(const std::string &code, const std::string &name, const std::string &city,
                    const std::string &country, double latitude, double longitude);

    bool removeAirport(const std::string &code);


    const Graph<Airport> &getGraph() const;

    void addAirline(const Airline &airline) {
        airlines.push_back(airline);
    }

    const std::vector<Airline> &getAirlines() const {
        return airlines;
    }

    bool addFlight(const string &sourceCode, const string &targetCode, const string &airline);


};


#endif //PROJECT_2_AIRPORTSGRAPH_H
