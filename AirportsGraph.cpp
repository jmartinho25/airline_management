#include "AirportsGraph.h"

bool AirportsGraph::addAirport(const std::string &code, const std::string &name, const std::string &city,
                               const std::string &country, double latitude, double longitude) {
    Airport airport(code, name, city, country, latitude, longitude);
    airportsGraph.addVertex(airport);
    Vertex<Airport> *vertex = airportsGraph.findVertex(Airport(code, "", "", "", 0.0, 0.0));
    vertex->setIndegree(0);
    return true;
}


bool
AirportsGraph::addFlight(const std::string &sourceCode, const std::string &targetCode, const std::string &airline) {
    Vertex<Airport> *sourceVertex = airportsGraph.findVertex(Airport(sourceCode, "", "", "", 0.0, 0.0));
    Vertex<Airport> *targetVertex = airportsGraph.findVertex(Airport(targetCode, "", "", "", 0.0, 0.0));
    targetVertex->increaseInDegree();
    if (sourceVertex && targetVertex) {
        return airportsGraph.addEdge(sourceVertex->getInfo(), targetVertex->getInfo(), 0.0,
                                     airline);  // Weight can be added if needed
    }

    return false;
}

const Graph<Airport> &AirportsGraph::getGraph() const {
    return airportsGraph;
}

