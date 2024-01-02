#include "AirportsGraph.h"

/**
* @brief Adds an airport(Vertex) with all the given information to the graph.
* @details Time complexity: O(V), where V is the number of vertices in the graph.
* @param code Airport code
* @param name Airport name
* @param city Airport city
* @param country Airport country
* @param latitude Airport latitude
* @param longitude Airport longitude
* @return True, always
*/
bool AirportsGraph::addAirport(const std::string &code, const std::string &name, const std::string &city,
                               const std::string &country, double latitude, double longitude) {
    Airport airport(code, name, city, country, latitude, longitude);
    airportsGraph.addVertex(airport);
    Vertex<Airport> *vertex = airportsGraph.findVertex(Airport(code, "", "", "", 0.0, 0.0));
    vertex->setIndegree(0);
    return true;
}

/**
* @brief Adds a Flight with all the given information to the graph.
* @details Time complexity: O(2V), where V is the number of vertices in the graph (this time complexity comes from the two findVertex operations).
* @param sourceCode Code of the source airport
* @param targetCode Code of the destination airport
* @param airline Airline of the flight
* @return True if the addition of the flight is successful, False otherwise
*/
bool
AirportsGraph::addFlight(const std::string &sourceCode, const std::string &targetCode, const std::string &airline) {
    Vertex<Airport> *sourceVertex = airportsGraph.findVertex(Airport(sourceCode, "", "", "", 0.0, 0.0));
    Vertex<Airport> *targetVertex = airportsGraph.findVertex(Airport(targetCode, "", "", "", 0.0, 0.0));
    targetVertex->increaseInDegree();
    if (sourceVertex && targetVertex) {
        return airportsGraph.addEdge(sourceVertex->getInfo(), targetVertex->getInfo(), 0.0,
                                     airline);
    }

    return false;
}

/**
* @brief Gets the graph so that it can be externally accessed.
* @details Time complexity: O(1)
* @return Graph in use (airportsGraph)
*/

const Graph<Airport> &AirportsGraph::getGraph() const {
    return airportsGraph;
}

