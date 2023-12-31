
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

//BEST FLIGHT PATHS FUNCTIONS

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

    auto paths= shortestPaths(sourceVertex,targetVertex);

    if(paths.empty()){
        std::cout<<"No Paths found between "<<airportCode1<<"and "<<airportCode2<<std::endl;
    }else {
        for (const auto path: paths) {
            std::cout << "Best Flight Option (" << paths[0].size() << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", " << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
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

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& cityAirport : cityAirports) {
        auto currentPath = findBestFlightPath(sourceAirport, cityAirport);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            while(!bestPath.empty()) {
                bestPath.pop();
            }
            bestPath.push(currentPath);
        }
        else if(currentPath.first==minStops){
            bestPath.push(currentPath);
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << airport << " to " << cityName << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << airport << " to " << cityName << " (" << minStops << " stops):"
                      << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}



std::vector<Vertex<Airport>*> FlightManager::findNearestAirportToCoordinates(double lat, double lon) {
    std::vector<Vertex<Airport>*> nearestAirports ;
    double minDistance = std::numeric_limits<double>::max();

    for (auto& airportVertex : airportsGraph.getGraph().getVertexSet()) {
        double distance = haversineDistance(lat, lon,
                                            airportVertex->getInfo().getLatitude(),
                                            airportVertex->getInfo().getLongitude());
        if (distance < minDistance) {
            minDistance = distance;
            nearestAirports={airportVertex};
        }else if(distance==minDistance){
            nearestAirports.push_back(airportVertex);
        }
    }

    return nearestAirports;
}

void FlightManager::bfoairporttocoordinates(const std::string& airportCode, double lat, double lon) {
    auto sourceAirport=findAirportVertexByNameOrCode(airportCode);

    std::vector<Vertex<Airport>*> targetAirports = findNearestAirportToCoordinates(lat, lon);

    if (!sourceAirport || targetAirports.empty()) {
        std::cout << "Source airport or nearest airport not found." << std::endl;
        return;
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& destAirport : targetAirports) {
        auto currentPath = findBestFlightPath(sourceAirport, destAirport);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            while(!bestPath.empty()) {
                bestPath.pop();
            }
            bestPath.push(currentPath);
        }
        else if(currentPath.first==minStops){
            bestPath.push(currentPath);
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << airportCode << " to coordinates (" << lat << ", " << lon << ")" << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << airportCode << " to coordinates (" << lat << ", " << lon << ") with " << minStops << " stops:" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }


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

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& cityAirport : cityAirports) {
        auto currentPath = findBestFlightPath(cityAirport, destAirport);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            while(!bestPath.empty()) {
                bestPath.pop();
            }
            bestPath.push(currentPath);
        }
        else if(currentPath.first==minStops){
            bestPath.push(currentPath);
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << cityName << " to " << airportCode << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << cityName << " to " << airportCode << " (" << minStops << " stops):"
                      << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
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

std::vector<std::vector<Vertex<Airport>*>> FlightManager::shortestPaths(Vertex<Airport>* startAirport,Vertex<Airport>* endAirport) {
    // Priority queue to explore paths in increasing order of length
    std::priority_queue<std::pair<int, std::vector<Vertex<Airport>*>>, std::vector<std::pair<int, std::vector<Vertex<Airport>*>>>, std::greater<>> pq;

    // Map to store the shortest distances to each airport
    std::unordered_map<Vertex<Airport>*, int> distance;

    // Initialize the distance map
    distance[startAirport] = 0;

    // Push the start airport onto the priority queue
    pq.push({0, {startAirport}});

    // Set to store unique paths
    std::set<std::vector<Vertex<Airport>*>> uniquePaths;

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();

        int currentDistance = current.first;
        auto currentPath = current.second;
        auto currentNode = currentPath.back();

        // If we reached the end airport, add the path to the set of unique paths
        if (currentNode == endAirport) {
            uniquePaths.insert(currentPath);
        }


        // Explore neighboring airports
        for (const auto& flight : currentNode->getAdj()) {
            auto neighbor = flight.getDest();

            int newDistance = currentDistance + 1;  // Assuming all flights have the same weight

            // If the new path is shorter or the distance hasn't been calculated yet
            if (distance.find(neighbor) == distance.end() || newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                std::vector<Vertex<Airport>*> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            } else if (newDistance == distance[neighbor]) {
                // If the new path has the same distance as the known shortest path
                std::vector<Vertex<Airport>*> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            }
        }
    }

    // Convert the set of unique paths to a vector and return
    return std::vector<std::vector<Vertex<Airport>*>>(uniquePaths.begin(), uniquePaths.end());
}

void FlightManager::bfocitytocity(const string& sourceCity, const string& destCity) {
    auto sourceAirports = findAirportsInCity(sourceCity);
    auto destAirports = findAirportsInCity(destCity);

    if (sourceAirports.empty() || destAirports.empty()) {
        std::cout << "Airports not found in one or both cities." << std::endl;
        return;
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();

    for (auto& sourceAirport : sourceAirports) {
        for (auto& destAirport : destAirports) {
            auto currentPath = findBestFlightPath(sourceAirport, destAirport);
            if (currentPath.first < minStops) {
                minStops = currentPath.first;
                while(!bestPath.empty()) {
                    bestPath.pop();
                }
                bestPath.push(currentPath);
            }
            else if(currentPath.first==minStops){
                bestPath.push(currentPath);
            }
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << sourceCity << " to " << destCity << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << sourceCity << " to " << destCity << " (" << minStops << " stops):"
                      << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

void FlightManager::bfocitytocoordinates(const string& cityName, double lat, double lon) {
    auto cityAirports = findAirportsInCity(cityName);
    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << cityName << std::endl;
        return;
    }

    auto nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No nearby airport found for the given coordinates." << std::endl;
        return;
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();

    for (auto& cityAirport : cityAirports) {
        for (auto& destAirport:nearestAirports) {
            auto currentPath = findBestFlightPath(cityAirport, destAirport);
            if (currentPath.first < minStops) {
                minStops = currentPath.first;
                while (!bestPath.empty()) {
                    bestPath.pop();
                }
                bestPath.push(currentPath);
            } else if (currentPath.first == minStops) {
                bestPath.push(currentPath);
            }
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << cityName << " to coordinates (" << lat << ", " << lon << ")" << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << cityName << " to coordinates (" << lat << ", " << lon << ") with " << minStops << " stops:" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

void FlightManager::bfoCoordinatestoAirport(double lat, double lon, const string& airportCode) {
    std::vector<Vertex<Airport>*> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No airport found near the given coordinates." << std::endl;
        return;
    }

    Vertex<Airport>* destAirport = findAirportVertexByNameOrCode(airportCode);
    if (!destAirport) {
        std::cout << "Destination airport not found: " << airportCode << std::endl;
        return;
    }

    for(auto nearestAirport:nearestAirports){
        if (nearestAirport == destAirport) {
            std::cout << "The nearest airport is the destination airport." << std::endl;
            return;
        }
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& nearestAirport : nearestAirports) {
        auto currentPath = findBestFlightPath(nearestAirport, destAirport);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            while(!bestPath.empty()) {
                bestPath.pop();
            }
            bestPath.push(currentPath);
        }
        else if(currentPath.first==minStops){
            bestPath.push(currentPath);
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from the nearest airport to " << airportCode << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from coordinates("<<lat<<","<<lon<<") to " << airportCode << " (" << minStops << " stops):" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

void FlightManager::bfoCoordinatestoCity(double lat, double lon, const std::string& cityName) {
    std::vector<Vertex<Airport>*> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No airport found near the given coordinates." << std::endl;
        return;
    }

    auto cityAirports = findAirportsInCity(cityName);
    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << cityName << std::endl;
        return;
    }

    std::queue<std::pair<int, std::vector<Airport>>> overallBestPath ;
    int minStops = std::numeric_limits<int>::max();

    for (auto& nearestAirport:nearestAirports){
        for (auto& cityAirport : cityAirports) {
            auto bestPath = findBestFlightPath(nearestAirport, cityAirport);
            if (bestPath.first < minStops) {
                minStops = bestPath.first;
                while(!overallBestPath.empty()) {
                    overallBestPath.pop();
                }
                overallBestPath.push(bestPath);
            }else if(bestPath.first==minStops) {
                overallBestPath.push(bestPath);
            }
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight paths found from the coordinates to any airport in " << cityName << std::endl;
    } else {
        while (!overallBestPath.empty()) {
            auto best =overallBestPath.front();
            overallBestPath.pop();
            std::cout << "Best flight option from coordinates (" << lat << ", " << lon << ") to " << cityName
                      << " (" << minStops << " stops):" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }

    }
}

void FlightManager::bfoCoordinatestoCoordinates(double sourceLat, double sourceLon, double destLat, double destLon) {
    std::vector<Vertex<Airport> *> sourceNearestAirports = findNearestAirportToCoordinates(sourceLat, sourceLon);
    if (sourceNearestAirports.empty()) {
        std::cout << "No airport found near the source coordinates." << std::endl;
        return;
    }

    std::vector<Vertex<Airport> *> destNearestAirports = findNearestAirportToCoordinates(destLat, destLon);
    if (destNearestAirports.empty()) {
        std::cout << "No airport found near the destination coordinates." << std::endl;
        return;
    }
    for (auto &sourceNearestAirport: sourceNearestAirports) {
        for (auto &destNearestAirport: destNearestAirports) {
            if (sourceNearestAirport == destNearestAirport) {
                std::cout << "The nearest airports for both source and destination coordinates are the same."
                          << std::endl;
                return;
            }
        }
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();

    for (auto &sourceNearestAirport: sourceNearestAirports) {
        for (auto &destNearestAirport: destNearestAirports) {
            auto currentPath = findBestFlightPath(sourceNearestAirport, destNearestAirport);
            if (currentPath.first < minStops) {
                minStops = currentPath.first;
                while (!bestPath.empty()) {
                    bestPath.pop();
                }
                bestPath.push(currentPath);
            } else if (currentPath.first == minStops) {
                bestPath.push(currentPath);
            }
        }
    }


    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found between the nearest airports." << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best = bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from coordinates (" << sourceLat << ", " << sourceLon
                      << ") to coordinates ("
                      << destLat << ", " << destLon << ") (" << minStops << " stops):" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }

}

//FILTERS


//PREFERRED AIRLINE FUNCTIONS


std::pair<int, std::vector<Airport>> FlightManager::findBestFlightPathPAirline(
        Vertex<Airport>* sourceVertex, Vertex<Airport>* destVertex, const std::string& airlineCode) {

    std::unordered_map<Vertex<Airport>*, std::pair<int, std::vector<Airport>>> paths;
    std::queue<Vertex<Airport>*> queue;

    queue.push(sourceVertex);
    paths[sourceVertex] = {0, {sourceVertex->getInfo()}};

    while (!queue.empty()) {
        Vertex<Airport>* currentVertex = queue.front();
        queue.pop();

        // Check if the current vertex is the destination vertex
        if (currentVertex == destVertex) {
            return paths[currentVertex];
        }

        for (const Edge<Airport>& edge : currentVertex->getAdj()) {
            Vertex<Airport>* nextVertex = edge.getDest();

            // Check if the airline code matches the desired airline
            if (edge.getAirlineCode() == airlineCode &&
                (paths.find(nextVertex) == paths.end() || paths[nextVertex].first > paths[currentVertex].first + 1)) {

                paths[nextVertex] = {paths[currentVertex].first + 1, paths[currentVertex].second};
                paths[nextVertex].second.push_back(nextVertex->getInfo());
                queue.push(nextVertex);
            }
        }
    }

    return {std::numeric_limits<int>::max(), std::vector<Airport>()};
}


void FlightManager::bfoairporttoairportPAirline(const string& airportCode1, const string& airportCode2, const std::string& airlineCode) {
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

    auto path= findBestFlightPathPAirline(sourceVertex,targetVertex,airlineCode);

    if (path.first==std::numeric_limits<int>::max()){
        std::cout << "No flight path found from " << airportCode1 << " to " << airportCode2 << " using exclusively airline '"<<airlineCode<<"'" << std::endl;
    }
    else {

        std::cout << "Best Flight Option using exclusively airline '"<<airlineCode<<"' (" << path.first << " stops):" << std::endl;
        for (const auto &airport: path.second) {
            std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                      << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                      << std::endl;
        }
    }
}

void FlightManager::bfoairporttocityPAirline(const string& airport, const string& cityName, const std::string& airlineCode) {
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

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& cityAirport : cityAirports) {
        auto currentPath = findBestFlightPathPAirline(sourceAirport, cityAirport, airlineCode);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            while(!bestPath.empty()) {
                bestPath.pop();
            }
            bestPath.push(currentPath);
        }
        else if(currentPath.first==minStops){
            bestPath.push(currentPath);
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << airport << " to " << cityName <<" using exclusively airline '"<<airlineCode<<"'"<< std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << airport << " to " << cityName << " using exclusively airline '"<<airlineCode<<"' (" << minStops << " stops):"
                      << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

void FlightManager::bfoairporttocoordinatesPAirline(const std::string &airportCode, double lat, double lon,
                                                    const std::string &airlineCode) {
    auto sourceAirport=findAirportVertexByNameOrCode(airportCode);

    std::vector<Vertex<Airport>*> targetAirports = findNearestAirportToCoordinates(lat, lon);

    if (!sourceAirport || targetAirports.empty()) {
        std::cout << "Source airport or nearest airport not found." << std::endl;
        return;
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& destAirport : targetAirports) {
        auto currentPath = findBestFlightPathPAirline(sourceAirport, destAirport, airlineCode);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            while(!bestPath.empty()) {
                bestPath.pop();
            }
            bestPath.push(currentPath);
        }
        else if(currentPath.first==minStops){
            bestPath.push(currentPath);
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << airportCode << " to coordinates (" << lat << ", " << lon << ")"<<" using exclusively airline '"<<airlineCode<<"'" << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << airportCode << " to coordinates (" << lat << ", " << lon << ") using exclusively airline '"<<airlineCode<<"' (" << minStops << " stops):" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }


}

void FlightManager::bfocitytoairportPAirline(const std::string &cityName, const std::string &airportCode,
                                             const std::string &airlineCode) {
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

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& cityAirport : cityAirports) {
        auto currentPath = findBestFlightPathPAirline(cityAirport, destAirport, airlineCode);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            while(!bestPath.empty()) {
                bestPath.pop();
            }
            bestPath.push(currentPath);
        }
        else if(currentPath.first==minStops){
            bestPath.push(currentPath);
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << cityName << " to " << airportCode <<"using exclusively airline '"<<airlineCode<<"'"<< std::endl;
    } else {

        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << cityName << " to " << airportCode << " (" << minStops << " stops) using exclusively airline '"<<airlineCode<<"':" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

void FlightManager::bfocitytocityPAirline(const std::string &sourceCity, const std::string &destCity,
                                          const std::string &airlineCode) {
    auto sourceAirports = findAirportsInCity(sourceCity);
    auto destAirports = findAirportsInCity(destCity);

    if (sourceAirports.empty() || destAirports.empty()) {
        std::cout << "Airports not found in one or both cities." << std::endl;
        return;
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();

    for (auto& sourceAirport : sourceAirports) {
        for (auto& destAirport : destAirports) {
            auto currentPath = findBestFlightPathPAirline(sourceAirport, destAirport, airlineCode);
            if (currentPath.first < minStops) {
                minStops = currentPath.first;
                while(!bestPath.empty()) {
                    bestPath.pop();
                }
                bestPath.push(currentPath);
            }
            else if(currentPath.first==minStops){
                bestPath.push(currentPath);
            }
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << sourceCity << " to " << destCity << " using exclusively airline '"<<airlineCode<<"'"<< std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << sourceCity << " to " << destCity << " using exclusively airline '"<<airlineCode<<"' (" << minStops << " stops):"
                      << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

void FlightManager::bfocitytocoordenatesPAirline(const std::string &cityName, double lat, double lon,
                                                 const std::string &airlineCode) {
    auto cityAirports = findAirportsInCity(cityName);
    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << cityName << std::endl;
        return;
    }

    auto nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No nearby airport found for the given coordinates." << std::endl;
        return;
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();

    for (auto& cityAirport : cityAirports) {
        for (auto& destAirport:nearestAirports) {
            auto currentPath = findBestFlightPathPAirline(cityAirport, destAirport, airlineCode);
            if (currentPath.first < minStops) {
                minStops = currentPath.first;
                while (!bestPath.empty()) {
                    bestPath.pop();
                }
                bestPath.push(currentPath);
            } else if (currentPath.first == minStops) {
                bestPath.push(currentPath);
            }
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from " << cityName << " to coordinates (" << lat << ", " << lon << ") using exclusively airline '"<<airlineCode<<"'" << std::endl;
    } else {
while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from " << cityName << " to coordinates (" << lat << ", " << lon << ") using exclusively airline '"<<airlineCode<<"' (" << minStops << " stops):"<< std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

void FlightManager::bfoCoordinatestoAirportPAirline(double lat, double lon, const std::string &airportCode,
                                                    const std::string &airlineCode) {
    std::vector<Vertex<Airport>*> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No airport found near the given coordinates." << std::endl;
        return;
    }

    Vertex<Airport>* destAirport = findAirportVertexByNameOrCode(airportCode);
    if (!destAirport) {
        std::cout << "Destination airport not found: " << airportCode << std::endl;
        return;
    }

    for(auto nearestAirport:nearestAirports){
        if (nearestAirport == destAirport) {
            std::cout << "The nearest airport is the destination airport." << std::endl;
            return;
        }
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();
    for (auto& nearestAirport : nearestAirports) {
        auto currentPath = findBestFlightPathPAirline(nearestAirport, destAirport, airlineCode);
        if (currentPath.first < minStops) {
            minStops = currentPath.first;
            while(!bestPath.empty()) {
                bestPath.pop();
            }
            bestPath.push(currentPath);
        }
        else if(currentPath.first==minStops){
            bestPath.push(currentPath);
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found from the nearest airport to " << airportCode << "using exclusively airline '"<<airlineCode<<"'" << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best =bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from coordinates("<<lat<<","<<lon<<") to " << airportCode << " using exclusively airline '"<<airlineCode<< "' (" << minStops << " stops):" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}
void FlightManager::bfoCoordinatestoCityPAirline(double lat, double lon, const std::string &cityName,
                                                 const std::string &airlineCode) {
    std::vector<Vertex<Airport>*> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No airport found near the given coordinates." << std::endl;
        return;
    }

    auto cityAirports = findAirportsInCity(cityName);
    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << cityName << std::endl;
        return;
    }

    std::queue<std::pair<int, std::vector<Airport>>> overallBestPath ;
    int minStops = std::numeric_limits<int>::max();

    for (auto& nearestAirport:nearestAirports){
        for (auto& cityAirport : cityAirports) {
            auto bestPath = findBestFlightPathPAirline(nearestAirport, cityAirport, airlineCode);
            if (bestPath.first < minStops) {
                minStops = bestPath.first;
                while(!overallBestPath.empty()) {
                    overallBestPath.pop();
                }
                overallBestPath.push(bestPath);
            }else if(bestPath.first==minStops) {
                overallBestPath.push(bestPath);
            }
        }
    }

    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight paths found from the coordinates to any airport in " << cityName <<" using exclusively airline '"<<airlineCode<<"'"<< std::endl;
    } else {
        while (!overallBestPath.empty()) {
            auto best =overallBestPath.front();
            overallBestPath.pop();
            std::cout << "Best flight option from coordinates (" << lat << ", " << lon << ") to " << cityName <<"using exclusively airline '"<<airlineCode<<"' "
                      << " (" << minStops << " stops):" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }

    }
}

void FlightManager::bfoCoordinatestoCoordinatesPAirline(double sourceLat, double sourceLon, double destLat,
                                                        double destLon, const std::string &airlineCode) {
    std::vector<Vertex<Airport> *> sourceNearestAirports = findNearestAirportToCoordinates(sourceLat, sourceLon);
    if (sourceNearestAirports.empty()) {
        std::cout << "No airport found near the source coordinates." << std::endl;
        return;
    }

    std::vector<Vertex<Airport> *> destNearestAirports = findNearestAirportToCoordinates(destLat, destLon);
    if (destNearestAirports.empty()) {
        std::cout << "No airport found near the destination coordinates." << std::endl;
        return;
    }
    for (auto &sourceNearestAirport: sourceNearestAirports) {
        for (auto &destNearestAirport: destNearestAirports) {
            if (sourceNearestAirport == destNearestAirport) {
                std::cout << "The nearest airports for both source and destination coordinates are the same."
                          << std::endl;
                return;
            }
        }
    }

    std::queue<std::pair<int, std::vector<Airport>>> bestPath;
    int minStops = std::numeric_limits<int>::max();

    for (auto &sourceNearestAirport: sourceNearestAirports) {
        for (auto &destNearestAirport: destNearestAirports) {
            auto currentPath = findBestFlightPathPAirline(sourceNearestAirport, destNearestAirport,airlineCode);
            if (currentPath.first < minStops) {
                minStops = currentPath.first;
                while (!bestPath.empty()) {
                    bestPath.pop();
                }
                bestPath.push(currentPath);
            } else if (currentPath.first == minStops) {
                bestPath.push(currentPath);
            }
        }
    }


    if (minStops == std::numeric_limits<int>::max()) {
        std::cout << "No flight path found between the nearest airports using exclusively airline '"<<airlineCode<<"'" << std::endl;
    } else {
        while (!bestPath.empty()) {
            auto best = bestPath.front();
            bestPath.pop();
            std::cout << "Best flight option from coordinates (" << sourceLat << ", " << sourceLon
                      << ") to coordinates ("
                      << destLat << ", " << destLon << ") using exclusively airline '"<<airlineCode<<"' (" << minStops << " stops):" << std::endl;
            for (const auto &airport: best.second) {
                std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")"
                          << " at coordinates (" << airport.getLatitude() << ", " << airport.getLongitude() << ")"
                          << std::endl;
            }
        }
    }

}