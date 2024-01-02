//
// Created by darksystem on 22-12-2023.
//

#ifndef PROJECT_2_AIRPORTSGRAPH_H
#define PROJECT_2_AIRPORTSGRAPH_H


#include <unordered_set>
#include "Airport.h"
#include "Graph.h"
#include "Airlines.h"

/**
 * @brief Graph that will have all the data processed through the parsing (basically the foundation to all our code)
 */
class AirportsGraph {
private:
    Graph<Airport> airportsGraph;
    std::vector<Airline> airlines;

public:
    bool addAirport(const std::string &code, const std::string &name, const std::string &city,
                    const std::string &country, double latitude, double longitude);

    bool removeAirport(const std::string &code);


    const Graph<Airport> &getGraph() const;

    /**
    * @brief Adds an airline to the graph airlines vector.
    * @details Time complexity: O(1)
    * @param airline Airline to be added to the vector
    */
    void addAirline(const Airline &airline) {
        airlines.push_back(airline);
    }

    /**
    * @brief Gets the vector with all the airlines present in the graph.
    * @details Time complexity: O(1)
    * @return Vector containing all the airlines present in the graph
    */
    const std::vector<Airline> &getAirlines() const {
        return airlines;
    }

    bool addFlight(const string &sourceCode, const string &targetCode, const string &airline);


};


#endif //PROJECT_2_AIRPORTSGRAPH_H
