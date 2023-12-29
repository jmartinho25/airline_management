
#include "FlightManager.h"
#include "Parsing.h"
#include "Graph.h"
#include "AirportsGraph.h"
#include "Airport.h"
#include "Airlines.h"
#include <iostream>
#include <set>
#include <map>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <climits>
#include <limits>

FlightManager::FlightManager() {
    ;
}

void FlightManager::loadAirports(const std::string &filename) {
    Parsing parsing;
    parsing.parseAirports(filename, airportsGraph);
}

void FlightManager::loadAirlines(const std::string &filename) {
    Parsing parsing;
    parsing.parseAirlines(filename, airportsGraph);
}

void FlightManager::loadFlights(const std::string &filename) {
    Parsing parsing;
    parsing.parseFlights(filename, airportsGraph);
}

void FlightManager::listnrairportsflights() {
    int totalAirports = airportsGraph.getGraph().getNumVertex();
    int totalFlights = 0;
    for (const auto &vertex: airportsGraph.getGraph().getVertexSet()) {
        totalFlights += vertex->getAdj().size();
    }
    std::cout << "Global number of airports and flights:" << std::endl;
    std::cout << "Total Airports: " << totalAirports << std::endl;
    std::cout << "Total Flights: " << totalFlights << std::endl;

}

void FlightManager::listflightsfromairport() {
    std::string airportCode;
    std::cout << "Enter airport code: ";
    std::cin >> airportCode;
    Vertex<Airport> *airport = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));
    if (airport) {
        unordered_set<string> uniqueAirlines;
        for (Edge<Airport> flight: airport->getAdj()) {
            uniqueAirlines.insert(flight.getAirlineCode());
        }
        std::cout << "Flights from Airport " << airportCode << ":" << std::endl;
        std::cout << "Number of flights: " << airport->getAdj().size() << std::endl;
        std::cout << "Number of airlines: " << uniqueAirlines.size() << std::endl;

    } else {
        std::cout << "Airport with code " << airportCode << " not found." << std::endl;
    }
}


std::string getAirlineName(const std::string &airlineCode, const AirportsGraph &airportsGraph) {
    for (const auto &airline: airportsGraph.getAirlines()) {
        if (airline.getCode() == airlineCode) {
            return airline.getName();
        }
    }
    return "Unknown";
}

void FlightManager::listflightspercityperairline() {
    std::map<std::pair<std::string, std::string>, int> flightsPerCityAirline;

    for (auto &vertex: airportsGraph.getGraph().getVertexSet()) {
        for (Edge<Airport> flight: vertex->getAdj()) {
            std::string airlineName = getAirlineName(flight.getAirlineCode(), airportsGraph);
            std::pair<std::string, std::string> cityAirline = {vertex->getInfo().getCity(), airlineName};

            if (flightsPerCityAirline.find(cityAirline) == flightsPerCityAirline.end()) {
                flightsPerCityAirline[cityAirline] = 1;
            } else {
                flightsPerCityAirline[cityAirline]++;
            }
        }
    }

    std::cout << "Number of Flights per City per Airline:" << std::endl;

    for (const auto &entry: flightsPerCityAirline) {
        std::cout << "City: " << entry.first.first << " | Airline: " << entry.first.second << " | Flights: "
                  << entry.second << std::endl;
    }
}

void FlightManager::listdiffcountriesperairpercity() {
    std::unordered_set<std::string> countriesFlownTo;
    std::string city;
    std::cout << "Enter the city name: ";
    std::cin.ignore();
    std::getline(std::cin, city);

    for (const auto airportVertex: airportsGraph.getGraph().getVertexSet()) {
        const Airport &airport = airportVertex->getInfo();
        if (airport.getCity() == city) {
            for (const Edge<Airport> &flight: airportVertex->getAdj()) {
                countriesFlownTo.insert(flight.getDest()->getInfo().getCountry());
            }
        }
    }
    int numCountries = countriesFlownTo.size();
    if (numCountries == 0) {
        std::cout << "No flights found from " << city << std::endl;
    } else {
        std::cout << "Number of different countries flown to from " << city << ": " << numCountries << std::endl;
    }
}
void FlightManager::dfsVisit(Vertex<Airport> *pVertex, unordered_set<std::string> &airports, unordered_set<std::string> &cities,
                             unordered_set<std::string> &countries) {
    pVertex->setVisited(true);
    airports.insert(pVertex->getInfo().getCode());
    cities.insert(pVertex->getInfo().getCity() + ", " + pVertex->getInfo().getCountry());
    countries.insert(pVertex->getInfo().getCountry());
    for(auto &flight:pVertex->getAdj()){
        if(!flight.getDest()->isVisited()){
            dfsVisit(flight.getDest(),airports,cities,countries);
        }
    }



}

void FlightManager::listnrdestavailable() {

    cout << "Enter the airport code: ";
    string airportCode;
    cin >> airportCode;
    Vertex<Airport> *sourceVertex = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));
    if (sourceVertex) {
        for(auto &vertex: airportsGraph.getGraph().getVertexSet()){
            vertex->setVisited(false);
        }
        std::unordered_set<std::string> airports;
        std::unordered_set<std::string> cities;
        std::unordered_set<std::string> countries;

for(auto &flight:sourceVertex->getAdj()){
    if(!flight.getDest()->isVisited()){
        dfsVisit(flight.getDest(),airports,cities,countries);
    }
}
    int aiportsAvailable = airports.size();
    int citiesAvailable = cities.size();
    int countriesAvailable = countries.size();

    std::cout << "Number of airports available for " << airportCode << ": " << aiportsAvailable
                              << std::endl;

    std::cout << "Number of cities available for " << airportCode << ": " << citiesAvailable
                              << std::endl;

    std::cout << "Number of countries available for " << airportCode << ": " << countriesAvailable
                              << std::endl;



    }
}

void FlightManager::listReachableDestinations(const std::string &airportCode, int maxStops, int criteria) {
    Vertex<Airport> *startVertex = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));
    if (!startVertex) {
        std::cout << "Airport not found." << std::endl;
        return;
    }

    std::unordered_set<std::string> visitedAirports;
    std::unordered_set<std::string> visitedCities;
    std::unordered_set<std::string> visitedCountries;

    std::queue<std::pair<Vertex<Airport> *, int>> queue;
    queue.push({startVertex, 0});

    while (!queue.empty()) {
        auto [currentVertex, stops] = queue.front();
        queue.pop();

        if (stops > maxStops) continue;

        Airport currentAirport = currentVertex->getInfo();
        visitedAirports.insert(currentAirport.getCode());
        visitedCities.insert(currentAirport.getCity() + ", " + currentAirport.getCountry());
        visitedCountries.insert(currentAirport.getCountry());

        for (const Edge<Airport> &edge: currentVertex->getAdj()) {
            Vertex<Airport> *nextVertex = edge.getDest();
            if (visitedAirports.find(nextVertex->getInfo().getCode()) == visitedAirports.end()) {
                queue.push({nextVertex, stops + 1});
            }
        }
    }

    int resultCount = 0;
    switch (criteria) {
        case 1:
            resultCount = visitedAirports.size();
            break;
        case 2:
            resultCount = visitedCities.size();
            break;
        case 3:
            resultCount = visitedCountries.size();
            break;
        default:
            std::cout << "Invalid criteria." << std::endl;
            return;
    }

    std::cout << "Number of reachable destinations: " << resultCount << std::endl;
}


void FlightManager::listmaxstopsbetweenairports() {
    int maxStops = 0;
    std::vector<std::pair<std::string, std::string>> maxTripAirports;

    for (auto sourceVertex: airportsGraph.getGraph().getVertexSet()) {
        std::vector<std::pair<std::string, std::string>> aux;
        int stops = calculateStopsBFS(sourceVertex, aux);
        if (stops > maxStops) {
            maxStops = stops;
            maxTripAirports = aux;
        } else if (stops == maxStops) {
            maxTripAirports.insert(maxTripAirports.end(), aux.begin(), aux.end());
        }
    }

    std::cout << "Maximum Trip with " << maxStops << " stops:" << std::endl;
    for (const auto &airports: maxTripAirports) {
        std::cout << "Source: " << airports.first << " | Destination: " << airports.second << std::endl;
    }
}

int FlightManager::calculateStopsBFS(Vertex<Airport> *source, std::vector<std::pair<std::string, std::string>> &aux) {
    int maxdistance = 0;
    for (auto *vertex: airportsGraph.getGraph().getVertexSet()) {
        vertex->setVisited(false);
        vertex->setProcessing(false);
    }
    std::queue<std::pair<Vertex<Airport> *, int>> q;
    q.push({source, 0});
    source->setProcessing(true);

    while (!q.empty()) {
        auto current = q.front().first;
        if (q.front().second > maxdistance) {
            maxdistance = q.front().second;
            aux = {{source->getInfo().getCode(), current->getInfo().getCode()}};
        } else if (q.front().second == maxdistance) {
            aux.push_back({source->getInfo().getCode(), current->getInfo().getCode()});
        }

        for (const Edge<Airport> &edge: current->getAdj()) {
            if (edge.getDest()->isVisited()) continue;
            if (edge.getDest()->isProcessing()) continue;
            q.push({edge.getDest(), q.front().second + 1});
            edge.getDest()->setProcessing(true);
        }
        q.pop();
        current->setVisited(true);
        current->setProcessing(false);
    }

    return maxdistance;
}

bool compareAirportTraffic(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
    return a.second > b.second;
}

void FlightManager::identifytopkairport() {
    int k;
    std::cout << "Enter the number of airports to display: ";
    std::cin >> k;
    std::vector<std::pair<std::string, int>> airportTraffic;
    for (auto airportVertex: airportsGraph.getGraph().getVertexSet()) {
        int inDegree = airportVertex->getIndegree();
        int outDegree = airportVertex->getAdj().size();
        int totalTraffic = inDegree + outDegree;
        airportTraffic.push_back({airportVertex->getInfo().getCode(), totalTraffic});
    }
    std::sort(airportTraffic.begin(), airportTraffic.end(), compareAirportTraffic);
    std::cout << "Top " << k << " Airports by Traffic Capacity:" << std::endl;
    for (int i = 0; i < k && i < airportTraffic.size(); ++i) {
        std::cout << "Airport Code: " << airportTraffic[i].first
                  << " | Total Traffic Capacity: " << airportTraffic[i].second << std::endl;
    }
}

void FlightManager::findEssentialAirports() {
    int time = 0;
    std::unordered_set<std::string> essentialAirports;

    std::unordered_map<std::string, int> disc;
    std::unordered_map<std::string, int> low;
    std::unordered_map<std::string, std::string> parent;

    for (const auto &vertex : airportsGraph.getGraph().getVertexSet()) {
        const std::string &v = vertex->getInfo().getCode();
        if (disc.find(v) == disc.end()) {
            FlightManager::findEssentialAirportsUtil(v, disc, low, parent, essentialAirports, time);
        }
    }

    if (essentialAirports.size() == 0) {
        std::cout << "No Essential Airports were found" << std::endl;
    } else {
        std::cout << "There are " << essentialAirports.size() << " Essential Airports" << std::endl;
        std::cout << "Essential Airports: " << std::endl;
        for (const auto &airportCode : essentialAirports) {
            const auto &airportVertex = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));
            if (airportVertex) {
                const std::string &airportName = airportVertex->getInfo().getName();
                std::cout << "- " << airportName << " (" << airportCode << ")" << std::endl;
            }
        }
    }
}

void FlightManager::findEssentialAirportsUtil(const std::string &u, std::unordered_map<std::string, int> &disc,
                                              std::unordered_map<std::string, int> &low,
                                              std::unordered_map<std::string, std::string> &parent,
                                              std::unordered_set<std::string> &essentialAirports, int &time) {

    disc[u] = low[u] = ++time;
    int children = 0;

    for (const Edge<Airport> &edge : airportsGraph.getGraph().findVertex(Airport(u, "", "", "", 0.0, 0.0))->getAdj()) {
        const std::string &v = edge.getDest()->getInfo().getCode();

        if (disc.find(v) == disc.end()) {
            children++;
            parent[v] = u;

            findEssentialAirportsUtil(v, disc, low, parent, essentialAirports, time);

            low[u] = std::min(low[u], low[v]);

            if ((parent.find(u) == parent.end() && children > 1) ||
                (parent.find(u) != parent.end() && low[v] >= disc[u])) {
                essentialAirports.insert(u);
            }
        } else if (v != parent[u]) {
            low[u] = std::min(low[u], disc[v]);
        }
    }
}
Vertex<Airport>* FlightManager::findAirportVertexByName(string airportName) {
for (const auto &vertex : airportsGraph.getGraph().getVertexSet()) {
    const Airport &airport = vertex->getInfo();
    if (airport.getName() == airportName)
        return vertex;
    }
    return nullptr;
}

void FlightManager::bfoairporttoairport(const string& airportCode1, const string& airportCode2) {
    auto sourceVertex = findAirportVertexByNameOrCode(airportCode1);
    auto targetVertex = findAirportVertexByNameOrCode(airportCode2);

    if (!sourceVertex || !targetVertex) {
        std::cout << "Airport(s) not found." << std::endl;
        return;
    }

    if (sourceVertex == targetVertex) {
        std::cout << "Source and destination airports are the same." << std::endl;
        return;
    }

    std::unordered_map<Vertex<Airport>*, Vertex<Airport>*> prev;
    std::queue<Vertex<Airport>*> queue;
    queue.push(sourceVertex);
    prev[sourceVertex] = nullptr;

    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (current == targetVertex) break;

        for (const auto& edge : current->getAdj()) {
            auto next = edge.getDest();
            if (prev.find(next) == prev.end()) {
                queue.push(next);
                prev[next] = current;
            }
        }
    }

    if (prev.find(targetVertex) == prev.end()) {
        std::cout << "No path found between " << airportCode1 << " and " << airportCode2 << std::endl;
        return;
    }

    std::vector<Airport> path;
    for (auto at = targetVertex; at != nullptr; at = prev[at]) {
        path.push_back(at->getInfo());
    }
    std::reverse(path.begin(), path.end());

    std::cout << "Best Flight Option (" << path.size() - 1 << " stops):" << std::endl;
    for (const auto& airport : path) {
        std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")" << std::endl;
    }
}


void FlightManager::bfoairporttocity(const string& airport, const string& cityName) {
    auto sourceAirport = findAirportVertexByNameOrCode(airport);
    if (!sourceAirport) {
        std::cout << "Source airport not found: " << airport << std::endl;
        return;
    }

    auto cityAirports = findAirportsInCity(cityName);
    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << cityName << std::endl;
        return;
    }

    std::pair<int, std::vector<Airport>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& cityAirport : cityAirports) {
        auto currentPath = findBestFlightPath(sourceAirport, cityAirport);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            bestPath = currentPath;
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << airport << " to " << cityName << std::endl;
    } else {
        std::cout << "Best flight option from " << airport << " to " << cityName << " (" << minStops << " stops):" << std::endl;
        for (const auto& airport : bestPath.second) {
            std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")" << std::endl;
        }
    }
}



Vertex<Airport>* FlightManager::findNearestAirportToCoordinates(double lat, double lon) {
    Vertex<Airport>* nearestAirport = nullptr;
    double minDistance = std::numeric_limits<double>::max();

    for (auto& airportVertex : airportsGraph.getGraph().getVertexSet()) {
        double distance = haversineDistance(lat, lon,
                                            airportVertex->getInfo().getLatitude(),
                                            airportVertex->getInfo().getLongitude());
        if (distance < minDistance) {
            minDistance = distance;
            nearestAirport = airportVertex;
        }
    }

    return nearestAirport;
}

void FlightManager::bfoairporttocoordinates(const std::string& airportCode, double lat, double lon) {
    Vertex<Airport>* sourceAirport = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));
    Vertex<Airport>* targetAirport = findNearestAirportToCoordinates(lat, lon);

    if (!sourceAirport || !targetAirport) {
        std::cout << "Source airport or nearest airport not found." << std::endl;
        return;
    }

    bfoairporttoairport(sourceAirport->getInfo().getCode(), targetAirport->getInfo().getCode());
}

std::vector<Vertex<Airport>*> FlightManager::findAirportsInCity(const std::string& cityName) {
    std::vector<Vertex<Airport>*> cityAirports;
    for (auto& vertex : airportsGraph.getGraph().getVertexSet()) {
        if (vertex->getInfo().getCity() == cityName) {
            cityAirports.push_back(vertex);
        }
    }
    return cityAirports;
}

void FlightManager::bfocitytoairport(const string& cityName, const string& airportCode) {
    auto cityAirports = findAirportsInCity(cityName);
    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << cityName << std::endl;
        return;
    }

    auto destAirport = findAirportVertexByNameOrCode(airportCode);
    if (!destAirport) {
        std::cout << "Destination airport not found: " << airportCode << std::endl;
        return;
    }

    std::pair<int, std::vector<Airport>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& cityAirport : cityAirports) {
        auto currentPath = findBestFlightPath(cityAirport, destAirport);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            bestPath = currentPath;
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << cityName << " to " << airportCode << std::endl;
    } else {
        std::cout << "Best flight option from " << cityName << " to " << airportCode << " (" << minStops << " stops):" << std::endl;
        for (const auto& airport : bestPath.second) {
            std::cout << airport.getName() << " (" << airport.getCode() << ")" << std::endl;
        }
    }
}

Vertex<Airport>* FlightManager::findAirportVertexByNameOrCode(const std::string& airportIdentifier) {
    if (airportIdentifier.size() > 3) {
        return findAirportVertexByName(airportIdentifier);
    } else if (airportIdentifier.size() == 3) {
        return airportsGraph.getGraph().findVertex(Airport(airportIdentifier, "", "", "", 0.0, 0.0));
    }
    return nullptr;
}

std::pair<int, std::vector<Airport>> FlightManager::findBestFlightPath(Vertex<Airport>* sourceVertex, Vertex<Airport>* destVertex) {
    std::unordered_map<Vertex<Airport>*, std::pair<int, std::vector<Airport>>> paths;
    std::queue<Vertex<Airport>*> queue;

    queue.push(sourceVertex);
    paths[sourceVertex] = {0, {sourceVertex->getInfo()}};

    while (!queue.empty()) {
        Vertex<Airport>* currentVertex = queue.front();
        queue.pop();

        if (currentVertex == destVertex) {
            return paths[currentVertex];
        }

        for (const Edge<Airport>& edge : currentVertex->getAdj()) {
            Vertex<Airport>* nextVertex = edge.getDest();
            if (paths.find(nextVertex) == paths.end() || paths[nextVertex].first > paths[currentVertex].first + 1) {
                paths[nextVertex] = {paths[currentVertex].first + 1, paths[currentVertex].second};
                paths[nextVertex].second.push_back(nextVertex->getInfo());
                queue.push(nextVertex);
            }
        }
    }

    return {std::numeric_limits<int>::max(), std::vector<Airport>()};
}

void FlightManager::bfocitytocity(const string& sourceCity, const string& destCity) {
    auto sourceAirports = findAirportsInCity(sourceCity);
    auto destAirports = findAirportsInCity(destCity);

    if (sourceAirports.empty() || destAirports.empty()) {
        std::cout << "Airports not found in one or both cities." << std::endl;
        return;
    }

    std::pair<int, std::vector<Airport>> bestPath;
    int minStops = std::numeric_limits<int>::max();

    for (auto& sourceAirport : sourceAirports) {
        for (auto& destAirport : destAirports) {
            auto currentPath = findBestFlightPath(sourceAirport, destAirport);
            if (currentPath.first < minStops) {
                minStops = currentPath.first;
                bestPath = currentPath;
            }
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << sourceCity << " to " << destCity << std::endl;
    } else {
        std::cout << "Best flight option from " << sourceCity << " to " << destCity << " (" << minStops << " stops):" << std::endl;
        for (const auto& airport : bestPath.second) {
            std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")" << std::endl;
        }
    }
}
