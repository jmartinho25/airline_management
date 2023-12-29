
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
if (airport.getName() == airportName) {
return vertex;
}
}
return nullptr;
}

void FlightManager::bfoairporttoairport(string airportCode1, string airportCode2){
    Vertex<Airport>* sourceVertex;
    Vertex<Airport>* targetVertex;

    if (airportCode1.size() > 3) {
        sourceVertex = findAirportVertexByName(airportCode1);
    } else if (airportCode1.size() == 3) {
        sourceVertex = airportsGraph.getGraph().findVertex(Airport(airportCode1, "", "", "", 0.0, 0.0));
    } else {
        std::cout << "Invalid source airport code" << std::endl;
        return;
    }

    if (airportCode2.size() > 3) {
        targetVertex = findAirportVertexByName(airportCode2);
    } else if (airportCode2.size() == 3) {
        targetVertex = airportsGraph.getGraph().findVertex(Airport(airportCode2, "", "", "", 0.0, 0.0));
    } else {
        std::cout << "Invalid target airport code" << std::endl;
        return;
    }

    if (!sourceVertex || !targetVertex) {
        std::cout << "Airport not found" << std::endl;
        return;
    }

    std::unordered_map<Vertex<Airport>*, std::pair<int, std::vector<Airport>>> paths;
    std::queue<Vertex<Airport>*> queue;

    queue.push(sourceVertex);
    paths[sourceVertex] = {0, {sourceVertex->getInfo()}};

    while (!queue.empty()) {
        Vertex<Airport>* currentVertex = queue.front();
        queue.pop();

        for (const Edge<Airport>& edge : currentVertex->getAdj()) {
            Vertex<Airport>* nextVertex = edge.getDest();

            if (paths.find(nextVertex) == paths.end() || paths[nextVertex].first > paths[currentVertex].first + 1) {
                paths[nextVertex] = {paths[currentVertex].first + 1, paths[currentVertex].second};
                paths[nextVertex].second.push_back(nextVertex->getInfo());
                queue.push(nextVertex);
            }
        }
    }

    if (paths.find(targetVertex) != paths.end()) {
        const auto& bestPath = paths[targetVertex].second;
        std::cout << "Best Flight Option (" << paths[targetVertex].first << " stops):" << std::endl;
        for (const auto& airport : bestPath) {
            std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")" << std::endl;
        }
    } else {
        std::cout << "No valid flight path found between " << airportCode1 << " and " << airportCode2 << std::endl;
    }
}

void FlightManager::bfoairporttocity(string airport, string cityName) {
    Vertex<Airport> *sourceVertex;

    if (airport.size() > 3) {
        sourceVertex = findAirportVertexByName(airport);
    } else if (airport.size() == 3) {
        sourceVertex = airportsGraph.getGraph().findVertex(Airport(airport, "", "", "", 0.0, 0.0));
    } else {
        std::cout << "Invalid source airport code" << std::endl;
        return;
    }

    std::queue<Vertex<Airport> *> airportsincity;
    for (const auto airportVertex: airportsGraph.getGraph().getVertexSet()) {
        const Airport &airport = airportVertex->getInfo();
        if (airport.getCity() == cityName) {
            airportsincity.push(airportVertex);
        }
    }

    std::unordered_map<Vertex<Airport> *, std::pair<int, std::vector<Airport>>> paths;
    std::queue<Vertex<Airport> *> queue;

    queue.push(sourceVertex);
    paths[sourceVertex] = {0, {sourceVertex->getInfo()}};

    while (!queue.empty()) {
        Vertex<Airport> *currentVertex = queue.front();
        queue.pop();

        for (const Edge<Airport> &edge: currentVertex->getAdj()) {
            Vertex<Airport> *nextVertex = edge.getDest();

            if (paths.find(nextVertex) == paths.end() || paths[nextVertex].first > paths[currentVertex].first + 1) {
                paths[nextVertex] = {paths[currentVertex].first + 1, paths[currentVertex].second};
                paths[nextVertex].second.push_back(nextVertex->getInfo());
                queue.push(nextVertex);
            }
        }
    }

    int minStops = INT_MAX;
    Vertex<Airport> *bestCityAirport = nullptr;
    std::queue<Vertex<Airport> *> best;

    while (!airportsincity.empty()){
        Vertex<Airport>* targetVertex= airportsincity.front();
        airportsincity.pop();
        if (paths.find(targetVertex) != paths.end()) {
            int stops = paths[targetVertex].first;
            if (stops < minStops) {
                minStops = stops;
                while(!best.empty()){
                    best.pop();
                }
                best.push(targetVertex);
            }
            else if(stops==minStops){
                best.push(targetVertex);
            }
        }
    }

    if (!best.empty()) {
        string a;
        if(minStops==1){
            a=" stop";
        }
        else{
            a=" stops";
        }
        if (best.size()==1){
            while(!best.empty()) {
                bestCityAirport = best.front();
                best.pop();

                const auto &bestPath = paths[bestCityAirport].second;
                std::cout << "Best Flight Option (" << minStops << a<<"):" << std::endl;
                for (const auto &airport: bestPath) {
                    std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")" << std::endl;
                }
            }
        }else if(best.size()>1){
            std::cout<<"There are "<<best.size()<<" equivalent flight options with "<<minStops<<a<<std::endl;
            int option =1;
            while(!best.empty()){
                bestCityAirport=best.front();
                best.pop();
                const auto &bestPath = paths[bestCityAirport].second;
                std::cout << "Option "<<option<<": "<<std::endl;
                for (const auto &airport : bestPath) {
                    std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")" << std::endl;
                }
                option++;
            }
        }
    } else {
        std::cout << "No valid flight path found between " << airport << " and any airport in " << cityName << std::endl;
    }
}


void FlightManager::bfocitytoairport(string cityName, string airport) {
    Vertex<Airport> *sourceVertex;

    if (airport.size() > 3) {
        sourceVertex = findAirportVertexByName(airport);
    } else if (airport.size() == 3) {
        sourceVertex = airportsGraph.getGraph().findVertex(Airport(airport, "", "", "", 0.0, 0.0));
    } else {
        std::cout << "Invalid target airport code" << std::endl;
        return;
    }

    std::queue<Vertex<Airport> *> airportsInCity;
    for (const auto airportVertex: airportsGraph.getGraph().getVertexSet()) {
        const Airport &cityAirport = airportVertex->getInfo();
        if (cityAirport.getCity() == cityName) {
            airportsInCity.push(airportVertex);
        }
    }

    std::unordered_map<Vertex<Airport> *, std::pair<int, std::vector<Airport>>> paths;
    std::queue<Vertex<Airport> *> queue;

    queue.push(sourceVertex);
    paths[sourceVertex] = {0, {sourceVertex->getInfo()}};

    while (!queue.empty()) {
        Vertex<Airport> *currentVertex = queue.front();
        queue.pop();

        for (const Edge<Airport> &edge: currentVertex->getAdj()) {
            Vertex<Airport> *nextVertex = edge.getDest();

            if (paths.find(nextVertex) == paths.end() || paths[nextVertex].first > paths[currentVertex].first + 1) {
                paths[nextVertex] = {paths[currentVertex].first + 1, paths[currentVertex].second};
                paths[nextVertex].second.push_back(nextVertex->getInfo());
                queue.push(nextVertex);
            }
        }
    }

    int minStops = INT_MAX;
    Vertex<Airport> *bestCityAirport = nullptr;
    std::queue<Vertex<Airport> *> best;

    while (!airportsInCity.empty()) {
        Vertex<Airport>* targetVertex = airportsInCity.front();
        airportsInCity.pop();

        if (paths.find(targetVertex) != paths.end()) {
            int stops = paths[targetVertex].first;
            if (stops < minStops) {
                minStops = stops;
                while (!best.empty()) {
                    best.pop();
                }
                best.push(targetVertex);
            } else if (stops == minStops) {
                best.push(targetVertex);
            }
        }
    }

    if (!best.empty()) {
        string a;
        if (minStops == 1) {
            a = " stop";
        } else {
            a = " stops";
        }
        if (best.size() == 1) {
            while (!best.empty()) {
                bestCityAirport = best.front();
                best.pop();

                const auto &bestPath = paths[bestCityAirport].second;
                std::cout << "Best Flight Option (" << minStops << a << "):" << std::endl;
                for (const auto &airport: bestPath) {
                    std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")" << std::endl;
                }
            }
        } else if (best.size() > 1) {
            std::cout << "There are " << best.size() << " equivalent flight options with " << minStops << a << std::endl;
            int option = 1;
            while (!best.empty()) {
                bestCityAirport = best.front();
                best.pop();
                const auto &bestPath = paths[bestCityAirport].second;
                std::cout << "Option " << option << ":" << std::endl;
                for (const auto &airport : bestPath) {
                    std::cout << "- " << airport.getName() << " (" << airport.getCode() << ")" << std::endl;
                }
                option++;
            }
        }
    } else {
        std::cout << "No valid flight path found between any airport in " << cityName << " and " << airport << std::endl;
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
