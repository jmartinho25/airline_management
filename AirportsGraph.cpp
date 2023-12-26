#include "AirportsGraph.h"

bool AirportsGraph::addAirport(const std::string& code, const std::string& name, const std::string& city,
                               const std::string& country, double latitude, double longitude) {
    Airport airport(code, name, city, country, latitude, longitude);
    return airportsGraph.addVertex(airport);
}



bool AirportsGraph::addFlight(const std::string& sourceCode, const std::string& targetCode, const std::string& airline) {
    Vertex<Airport>* sourceVertex = airportsGraph.findVertex(Airport(sourceCode, "", "", "", 0.0, 0.0));
    Vertex<Airport>* targetVertex = airportsGraph.findVertex(Airport(targetCode, "", "", "", 0.0, 0.0));

    if (sourceVertex && targetVertex) {
        return airportsGraph.addEdge(sourceVertex->getInfo(), targetVertex->getInfo(), 0.0,airline);  // Weight can be added if needed
    }

    return false;
}

const Graph<Airport>& AirportsGraph::getGraph() const {
    return airportsGraph;
}

