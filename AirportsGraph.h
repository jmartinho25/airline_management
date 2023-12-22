//
// Created by darksystem on 22-12-2023.
//

#ifndef PROJECT_2_AIRPORTSGRAPH_H
#define PROJECT_2_AIRPORTSGRAPH_H


#include "Airport.h"
#include "Graph.h"

class AirportsGraph {
private:
    Graph<Airport> airportsGraph;

public:
    bool addAirport(const std::string& code, const std::string& name, const std::string& city,
                    const std::string& country, double latitude, double longitude);

    bool removeAirport(const std::string& code);

    bool addFlight(const std::string& sourceCode, const std::string& targetCode);

    const Graph<Airport>& getGraph() const;
};


#endif //PROJECT_2_AIRPORTSGRAPH_H
