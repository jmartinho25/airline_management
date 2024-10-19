
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
#include <cmath>

/**
    * @brief Constructor of the FlightManager class.
    */
FlightManager::FlightManager() {
}

/**
    * @brief Function responsible for loading airports from a file
    * @details Time complexity: O(n) where n is the number of lines in the file
    * @param filename File from which the airports will be loaded
    */
void FlightManager::loadAirports(const std::string &filename) {
    Parsing parsing;
    parsing.parseAirports(filename, airportsGraph);
}

/**
    * @brief Function responsible for loading airlines from a file
    * @details Time complexity: O(n) where n is the number of lines in the file
    * @param filename File from which the airlines will be loaded
    */
void FlightManager::loadAirlines(const std::string &filename) {
    Parsing parsing;
    parsing.parseAirlines(filename, airportsGraph);
}

/**
    * @brief Function responsible for loading flights from a file
    * @details Time complexity: O(n) where n is the number of lines in the file
    * @param filename File from which the flights will be loaded
    */
void FlightManager::loadFlights(const std::string &filename) {
    Parsing parsing;
    parsing.parseFlights(filename, airportsGraph);
}

/**
 * @brief Prints the total number of airports and flights in the used graph (airportsGraph).
 * @details Time complexity: O(V + E), where V is the number of vertices, and E is the number of edges.
 */
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

/**
 * @brief Function counts the total number of flights and the number of airlines operating from the specified airport and prints it.
 * @details Time complexity: O(V + E), where V is the number of vertices, and E is the number of edges.
 */
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

/**
 * @brief The function takes an airline code and an instance of the AirportsGraph class, then iterates through the airlines in the graph to find the matching airline.
 * @details Time complexity: O(A), where A is the number of airlines in the graph.
 * @param airlineCode Code of the airline
 * @param airportsGraph Graph used in the search of the airline
 * @return Name of the airline matching with the given airlineCode
 */
std::string getAirlineName(const std::string &airlineCode, const AirportsGraph &airportsGraph) {
    for (const auto &airline: airportsGraph.getAirlines()) {
        if (airline.getCode() == airlineCode) {
            return airline.getName();
        }
    }
    return "Unknown";
}

/**
 * @brief Function counts the total number of flights for each airline in each city and prints it.
 * @details Time complexity: O(V + E), where V is the number of vertices, and E is the number of edges.
 */
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

/**
 * @brief Function counts the number of different countries flown to from the specified city and prints the result.
 * @details Time complexity: O(V + E), where V is the number of vertices, and E is the number of edges.
 */
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

/**
 * @brief Function performs a depth-first search (DFS) visit on a given vertex in the airportsGraph
 * @param airports Container in which the information about the visited airport will be inserted
 * @param cities Container in which the information about the visited city will be inserted
 * @param countries Container in which the information about the visited country will be inserted
 * @details Time complexity: O(V + E), where V is the number of vertices, and E is the number of edges.
 */
void FlightManager::dfsVisit(Vertex<Airport> *pVertex, unordered_set<std::string> &airports,
                             unordered_set<std::string> &cities,
                             unordered_set<std::string> &countries) {
    pVertex->setVisited(true);
    airports.insert(pVertex->getInfo().getCode());
    cities.insert(pVertex->getInfo().getCity() + ", " + pVertex->getInfo().getCountry());
    countries.insert(pVertex->getInfo().getCountry());
    for (auto &flight: pVertex->getAdj()) {
        if (!flight.getDest()->isVisited()) {
            dfsVisit(flight.getDest(), airports, cities, countries);
        }
    }
}

/**
 * @brief Function performs a depth-first search (DFS) traversal to count the number of available airports, cities, and countries reachable from the specified airport and prints it.
 * @details Time complexity: O(V + E), where V is the number of vertices, and E is the number of edges.
 */
void FlightManager::listnrdestavailable() {

    cout << "Enter the airport code: ";
    string airportCode;
    cin >> airportCode;
    Vertex<Airport> *sourceVertex = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));
    if (sourceVertex) {
        for (auto &vertex: airportsGraph.getGraph().getVertexSet()) {
            vertex->setVisited(false);
        }
        std::unordered_set<std::string> airports;
        std::unordered_set<std::string> cities;
        std::unordered_set<std::string> countries;

        for (auto &flight: sourceVertex->getAdj()) {
            if (!flight.getDest()->isVisited()) {
                dfsVisit(flight.getDest(), airports, cities, countries);
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

/**
 * @brief Function performs a breadth-first search (BFS) traversal starting from the specified airport, counting the reachable destinations based on the criteria, and prints the result.
 * @details Time complexity: O(V + E), where V is the number of vertices, and E is the number of edges.
 * @param airportCode Code of the source airport
 * @param maxStops Number of maximum stops in the path
 * @param criteria Criteria for the result (1 is the number of airports, 2 is the number of cities and 3 is the number of countries)
 */
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

/**
 * @brief Function iterates over all vertices in the graph, performs a breadth-first search (BFS) from each vertex to calculate the maximum number of stops, and then keeps track of the airport pairs that achieve this maximum. Finally, it prints the result.
 * @details Time complexity: O(V^2 + VE), where V is the number of vertices and E is the number of edges in the graph.
 */
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

/**
 * @brief Function calculates the maximum number of stops for trips starting from a specified source airport using a breadth-first search (BFS) approach, keeping track of the airport pairs that achieve this maximum number of stops in the vector 'aux'.
 * @details Time complexity: O(V + E), where V is the number of vertices, and E is the number of edges.
 * @param source Source Airport
 * @param aux Vector in which the airport pairs that achieve this maximum number of stops will be inserted in
 * @return Maximum number of stops achieved during BFS
 */
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

/**
 * @brief Comparison function that will be used in a sort algorithm to sort a collection of std::pair<std::string, int> elements based on the second element of each pair in descending order.
 * @details Time complexity: O(V + E), where V is the number of vertices, and E is the number of edges.
 * @param a First pair that will be used in the comparison
 * @param b Second pair that will be used in the comparison
 * @return True if the second element of 'a' is greater than the second element of 'b', False otherwise
 */
bool compareAirportTraffic(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) {
    return a.second > b.second;
}

/**
 * @brief Function identifies and displays the top k airports based on their traffic capacity. Traffic capacity is calculated as the sum of the in-degree and out-degree of each airport in the graph.
 * @details Time complexity: O(V log V), where V is the number of vertices in the graph.
 */
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

/**
 * @brief Function implements a depth-first search (DFS) algorithm to find articulation points in an undirected graph.
 * @details Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
 * @param v Start vertex of the search
 * @param essentialAirports Container in which the codes of the airports that are articulation points will be inserted in
 * @param index Represents the order in which vertices are visited during the DFS
 */
void FlightManager::dfs_artic(Vertex<Airport> *v, unordered_set<string> &essentialAirports, unsigned int index) const {
    int children = 0;
    v->setNum(index);
    v->setLow(index);
    index++;
    v->setProcessing(true);
    v->setVisited(true);

    for (const Edge<Airport> &e: v->getAdj()) {
        Vertex<Airport> *w = e.getDest();
        if (!w->isVisited()) {
            children++;
            dfs_artic(w, essentialAirports, index);
            if (v->getLow() > w->getLow()) v->setLow(w->getLow());
            if (w->getLow() >= v->getNum() and v->getNum() != 1) essentialAirports.insert(v->getInfo().getCode());
            if (v->getNum() == 1 and children > 1) essentialAirports.insert(v->getInfo().getCode());
        } else if (w->isProcessing()) {
            if (v->getLow() > w->getNum()) v->setLow(w->getNum());
        }
    }

    v->setProcessing(false);
}

/**
 * @brief Function uses a combination of DFS traversal and the identification of articulation points to find essential airports in a graph. Essential airports are identified by removing each edge in the graph and checking if the removal increases the number of connected components. Finally, it prints the essential airports.
 * @details Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
 */
void FlightManager::findEssentialAirports() {

    unsigned int index = 1;
    unordered_set<string> essentialAirports;
    queue<pair<Vertex<Airport> *, Vertex<Airport> *>> addedEdges;
    for (Vertex<Airport> *vertex: airportsGraph.getGraph().getVertexSet()) {
        vertex->setVisited(false);
        vertex->setProcessing(false);
        for (Edge<Airport> edge: vertex->getAdj()) {
            Vertex<Airport> *w = edge.getDest();
            if (airportsGraph.addFlight(w->getInfo().getCode(), vertex->getInfo().getCode(), edge.getAirlineCode())) {
                addedEdges.emplace(w, vertex);
            }
        }
    }
    for (Vertex<Airport> *v: airportsGraph.getGraph().getVertexSet()) {
        if (!v->isVisited()) {
            dfs_artic(v, essentialAirports, index);
        }
    }

    if (essentialAirports.size() == 0) {
        std::cout << "No Essential Airports were found" << std::endl;
    } else {
        std::cout << "There are " << essentialAirports.size() << " Essential Airports" << std::endl;
        std::cout << "Essential Airports: " << std::endl;
        for (const auto &airportCode: essentialAirports) {
            const auto &airportVertex = airportsGraph.getGraph().findVertex(
                    Airport(airportCode, "", "", "", 0.0, 0.0));
            if (airportVertex) {
                const std::string &airportName = airportVertex->getInfo().getName();
                std::cout << "- " << airportName << " (" << airportCode << ")" << std::endl;
            }
        }
    }

}

/**
 * @brief Function searches for a vertex in the graph based on the airport´s name.
 * @details Time complexity: O(V + E), where V is the number of vertices and E is the number of edges in the graph.
 * @param airportName Name of the airport
 * @return The corresponding vertex if a match is found, nullptr otherwise.
 */
Vertex<Airport> *FlightManager::findAirportVertexByName(string airportName) {
    for (const auto &vertex: airportsGraph.getGraph().getVertexSet()) {
        const Airport &airport = vertex->getInfo();
        if (airport.getName() == airportName)
            return vertex;
    }
    return nullptr;
}

/**
 * @brief Function finds the nearest airport (or airports in case of a tie) to a given set of coordinates (latitude and longitude).
 * @details Time complexity: O(V), where V is the number of vertices in the graph.
 * @param lat Latitude of the airport
 * @param lon Longitude of the airport
 * @return Vector containing the nearest airport(s)
 */
std::vector<Vertex<Airport> *> FlightManager::findNearestAirportToCoordinates(double lat, double lon) {
    std::vector<Vertex<Airport> *> nearestAirports;
    double minDistance = std::numeric_limits<double>::max();

    for (auto &airportVertex: airportsGraph.getGraph().getVertexSet()) {
        double distance = haversineDistance(lat, lon,
                                            airportVertex->getInfo().getLatitude(),
                                            airportVertex->getInfo().getLongitude());
        if (distance < minDistance) {
            minDistance = distance;
            nearestAirports = {airportVertex};
        } else if (distance == minDistance) {
            nearestAirports.push_back(airportVertex);
        }
    }

    return nearestAirports;
}

/**
 * @brief Function finds the airport vertices located in a specified city.
 * @details Time complexity: O(V), where V is the number of vertices in the graph.
 * @param cityName Name of the city
 * @return Vector containing airport vertices in the specified city
 */
std::vector<Vertex<Airport> *> FlightManager::findAirportsInCity(const std::string &cityName) {
    std::vector<Vertex<Airport> *> cityAirports;
    for (auto &vertex: airportsGraph.getGraph().getVertexSet()) {
        if (vertex->getInfo().getCity() == cityName) {
            cityAirports.push_back(vertex);
        }
    }
    return cityAirports;
}

/**
 * @brief Function searches for a vertex in the graph based on the airport´s name or code.
 * @details Time complexity: O(V), where V is the number of vertices in the graph.
 * @param airportIdentifier Name or Code of the airport
 * @return The corresponding vertex if a match is found, nullptr otherwise.
 */
Vertex<Airport> *FlightManager::findAirportVertexByNameOrCode(const std::string &airportIdentifier) {
    if (airportIdentifier.size() > 3) {
        return findAirportVertexByName(airportIdentifier);
    } else if (airportIdentifier.size() == 3) {
        return airportsGraph.getGraph().findVertex(Airport(airportIdentifier, "", "", "", 0.0, 0.0));
    }
    return nullptr;
}

/**
 * @brief Function finds all shortest paths from a specified start airport to a specified end airport.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices in the graph and E is the number of edges. This time complexity comes from the use of Dijkstra's algorithm with a priority queue.
 * @param startAirport Airport where the paths start
 * @param endAirport Airport where the paths must end
 * @return Vector of vectors, where each inner vector represents a unique shortest path from the start airport to the end airport.
 */
std::vector<std::vector<Vertex<Airport> *>>
FlightManager::shortestPaths(Vertex<Airport> *startAirport, Vertex<Airport> *endAirport) {
    std::priority_queue<std::pair<int, std::vector<Vertex<Airport> *>>, std::vector<std::pair<int, std::vector<Vertex<Airport> *>>>, std::greater<>> pq;

    std::unordered_map<Vertex<Airport> *, int> distance;

    distance[startAirport] = 0;

    pq.push({0, {startAirport}});

    std::set<std::vector<Vertex<Airport> *>> uniquePaths;

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();

        int currentDistance = current.first;
        auto currentPath = current.second;
        auto currentNode = currentPath.back();

        if (currentNode == endAirport) {
            uniquePaths.insert(currentPath);
        }

        for (const auto &flight: currentNode->getAdj()) {
            auto neighbor = flight.getDest();

            int newDistance = currentDistance + 1;

            if (distance.find(neighbor) == distance.end() || newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            } else if (newDistance == distance[neighbor]) {
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            }
        }
    }

    return std::vector<std::vector<Vertex<Airport> *>>(uniquePaths.begin(), uniquePaths.end());
}
//BEST FLIGHT PATHS FUNCTIONS

/**
 * @brief Function finds and displays the best flight options (shortest paths) between two airports using the 'shortestPaths' function
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airportCode1 Airport where the paths start
 * @param airportCode2 Airport where the paths must end
 */
void FlightManager::bfoairporttoairport(const string &airportCode1, const string &airportCode2) {
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

    auto paths = shortestPaths(sourceVertex, targetVertex);

    if (paths.empty()) {
        std::cout << "No Paths found between " << airportCode1 << "and " << airportCode2 << std::endl;
    } else {
        for (const auto path: paths) {
            std::cout << "Best Flight Option (" << paths[0].size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from a given source airport to any airport in a specified city.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airport Airport where the paths start
 * @param cityName City in which the paths must end
 */
void FlightManager::bfoairporttocity(const string &airport, const string &cityName) {
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

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &cityAirport: cityAirports) {
        auto paths = shortestPaths(sourceAirport, cityAirport);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << airport << " to " << cityName << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from " << airport << " to " << cityName << " (" << bestPaths[0].size() - 1
                      << " stops):"
                      << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from a given source airport to the nearest airport(s) to specified coordinates (latitude and longitude).
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airportCode Airport where the paths start
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 */
void FlightManager::bfoairporttocoordinates(const std::string &airportCode, double lat, double lon) {
    auto sourceAirport = findAirportVertexByNameOrCode(airportCode);

    std::vector<Vertex<Airport> *> targetAirports = findNearestAirportToCoordinates(lat, lon);

    if (!sourceAirport || targetAirports.empty()) {
        std::cout << "Source airport or nearest airport not found." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &destAirport: targetAirports) {
        auto paths = shortestPaths(sourceAirport, destAirport);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << airportCode << " to coordinates (" << lat << ", " << lon << ")"
                  << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from " << airportCode << " to coordinates (" << lat << ", " << lon
                      << ") with " << bestPaths[0].size() - 1 << " stops:" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from any airport in a specified city to a given destination airport.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param cityName City in which the paths start
 * @param airportCode Airport where the paths must end
 */
void FlightManager::bfocitytoairport(const string &cityName, const string &airportCode) {
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

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &cityAirport: cityAirports) {
        auto paths = shortestPaths(cityAirport, destAirport);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << cityName << " to " << airportCode << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from " << cityName << " to " << airportCode << " ("
                      << bestPaths[0].size() - 1 << " stops):"
                      << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between any airport in the source city and any airport in the destination city.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param sourceCity City in which the paths start
 * @param destCity City in which the paths must end
 */
void FlightManager::bfocitytocity(const string &sourceCity, const string &destCity) {
    auto sourceAirports = findAirportsInCity(sourceCity);
    auto destAirports = findAirportsInCity(destCity);

    if (sourceAirports.empty() || destAirports.empty()) {
        std::cout << "Airports not found in one or both cities." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;

    for (auto &sourceAirport: sourceAirports) {
        for (auto &destAirport: destAirports) {
            auto paths = shortestPaths(sourceAirport, destAirport);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << sourceCity << " to " << destCity << std::endl;
    } else {
        for (const auto path: bestPaths) {

            std::cout << "Best flight option from " << sourceCity << " to " << destCity << " ("
                      << bestPaths[0].size() - 1 << " stops):"
                      << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between any airport in the specified city and the nearest airport(s) to the given coordinates.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param cityName City in which the paths start
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 */
void FlightManager::bfocitytocoordinates(const string &cityName, double lat, double lon) {
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

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;

    for (auto &cityAirport: cityAirports) {
        for (auto &destAirport: nearestAirports) {
            auto paths = shortestPaths(cityAirport, destAirport);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << cityName << " to coordinates (" << lat << ", " << lon << ")"
                  << std::endl;
    } else {
        for (const auto path: bestPaths) {

            std::cout << "Best flight option from " << cityName << " to coordinates (" << lat << ", " << lon
                      << ") with " << bestPaths[0].size() - 1 << " stops:" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given coordinates and a specified  airport .
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param airportCode Airport in which the paths must end
 */
void FlightManager::bfoCoordinatestoAirport(double lat, double lon, const string &airportCode) {
    std::vector<Vertex<Airport> *> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No airport found near the given coordinates." << std::endl;
        return;
    }

    Vertex<Airport> *destAirport = findAirportVertexByNameOrCode(airportCode);
    if (!destAirport) {
        std::cout << "Destination airport not found: " << airportCode << std::endl;
        return;
    }

    for (auto nearestAirport: nearestAirports) {
        if (nearestAirport == destAirport) {
            std::cout << "The nearest airport is the destination airport." << std::endl;
            return;
        }
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &nearestAirport: nearestAirports) {
        auto paths = shortestPaths(nearestAirport, destAirport);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from the nearest airport to " << airportCode << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from coordinates(" << lat << "," << lon << ") to " << airportCode << " ("
                      << bestPaths[0].size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given coordinates and any airport in a specified city.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param cityName City in which the paths must end
 */
void FlightManager::bfoCoordinatestoCity(double lat, double lon, const std::string &cityName) {
    std::vector<Vertex<Airport> *> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No airport found near the given coordinates." << std::endl;
        return;
    }

    auto cityAirports = findAirportsInCity(cityName);
    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << cityName << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;

    for (auto &nearestAirport: nearestAirports) {
        for (auto &cityAirport: cityAirports) {
            auto paths = shortestPaths(nearestAirport, cityAirport);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight paths found from the coordinates to any airport in " << cityName << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from coordinates (" << lat << ", " << lon << ") to " << cityName
                      << " (" << bestPaths[0].size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }

    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given source coordinates and the nearest airport(s) to the given destination coordinates.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param sourceLat Latitude of the pair of source coordinates
 * @param sourceLon Longitude of the pair of source coordinates
 * @param destLat Latitude of the pair of source coordinates
 * @param destLon Longitude of the pair of source coordinates
 */
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

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;

    for (auto &sourceNearestAirport: sourceNearestAirports) {
        for (auto &destNearestAirport: destNearestAirports) {
            auto paths = shortestPaths(sourceNearestAirport, destNearestAirport);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }


    if (bestPaths.empty()) {
        std::cout << "No flight path found between the nearest airports." << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from coordinates (" << sourceLat << ", " << sourceLon
                      << ") to coordinates ("
                      << destLat << ", " << destLon << ") (" << bestPaths[0].size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }

}

//FILTERS


//PREFERRED AIRLINE FUNCTIONS


/**
 * @brief Function finds all shortest paths from a specified start airport to a specified end airport that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices in the graph and E is the number of edges. This time complexity comes from the use of Dijkstra's algorithm with a priority queue.
 * @param startAirport Airport where the paths start
 * @param endAirport Airport where the paths must end
 * @param airlineCode Code of the airline used in the path(s)
 * @return Vector of vectors, where each inner vector represents a unique shortest path from the start airport to the end airport.
 */
std::vector<std::vector<Vertex<Airport> *>> FlightManager::shortestPathsPAirline(
        Vertex<Airport> *startAirport, Vertex<Airport> *endAirport, const std::string &airlineCode) {

    std::priority_queue<std::pair<int, std::vector<Vertex<Airport> *>>, std::vector<std::pair<int, std::vector<Vertex<Airport> *>>>, std::greater<>> pq;

    std::unordered_map<Vertex<Airport> *, int> distance;

    distance[startAirport] = 0;

    pq.push({0, {startAirport}});

    std::set<std::vector<Vertex<Airport> *>> uniquePaths;

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();

        int currentDistance = current.first;
        auto currentPath = current.second;
        auto currentNode = currentPath.back();

        if (currentNode == endAirport) {
            uniquePaths.insert(currentPath);
        }

        for (const auto &flight: currentNode->getAdj()) {
            auto neighbor = flight.getDest();

            int newDistance = currentDistance + 1;

            if (flight.getAirlineCode() == airlineCode &&
                (distance.find(neighbor) == distance.end() || newDistance < distance[neighbor])) {
                distance[neighbor] = newDistance;
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            } else if (flight.getAirlineCode() == airlineCode && newDistance == distance[neighbor]) {
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            }
        }
    }

    return std::vector<std::vector<Vertex<Airport> *>>(uniquePaths.begin(), uniquePaths.end());
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between two airports that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airportCode1 Airport where the paths start
 * @param airportCode2 Airport where the paths must end
 * @param airlineCode Code of the airline used in the path(s)
 */
void FlightManager::bfoairporttoairportPAirline(const string &airportCode1, const string &airportCode2,
                                                const std::string &airlineCode) {
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

    auto paths = shortestPathsPAirline(sourceVertex, targetVertex, airlineCode);

    if (paths.empty()) {
        std::cout << "No flight path found from " << airportCode1 << " to " << airportCode2
                  << " using exclusively airline '" << airlineCode << "'" << std::endl;
    } else {
        for (const auto path: paths) {
            std::cout << "Best Flight Option using exclusively airline '" << airlineCode << "' (" << paths[0].size() - 1
                      << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from a given source airport to any airport in a specified city that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airport Airport where the paths start
 * @param cityName City in which the paths must end
 * @param airlineCode Code of the airline used in the path(s)
 */
void
FlightManager::bfoairporttocityPAirline(const string &airport, const string &cityName, const std::string &airlineCode) {
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

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &cityAirport: cityAirports) {
        auto paths = shortestPathsPAirline(sourceAirport, cityAirport, airlineCode);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << airport << " to " << cityName << " using exclusively airline '"
                  << airlineCode << "'" << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from " << airport << " to " << cityName << " using exclusively airline '"
                      << airlineCode << "' (" << bestPaths[0].size() - 1 << " stops):"
                      << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from a given source airport to the nearest airport(s) to specified coordinates (latitude and longitude) that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airportCode Airport where the paths start
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param airlineCode Code of the airline used in the path(s)
 */
void FlightManager::bfoairporttocoordinatesPAirline(const std::string &airportCode, double lat, double lon,
                                                    const std::string &airlineCode) {
    auto sourceAirport = findAirportVertexByNameOrCode(airportCode);

    std::vector<Vertex<Airport> *> targetAirports = findNearestAirportToCoordinates(lat, lon);

    if (!sourceAirport || targetAirports.empty()) {
        std::cout << "Source airport or nearest airport not found." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &destAirport: targetAirports) {
        auto paths = shortestPathsPAirline(sourceAirport, destAirport, airlineCode);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << airportCode << " to coordinates (" << lat << ", " << lon << ")"
                  << " using exclusively airline '" << airlineCode << "'" << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from " << airportCode << " to coordinates (" << lat << ", " << lon
                      << ") using exclusively airline '" << airlineCode << "' (" << bestPaths[0].size() - 1
                      << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }


}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from any airport in a specified city to a given destination airport that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param cityName City in which the paths start
 * @param airportCode Airport where the paths must end
 * @param airlineCode Code of the airline used in the path(s)
 */
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

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &cityAirport: cityAirports) {
        auto paths = shortestPathsPAirline(cityAirport, destAirport, airlineCode);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << cityName << " to " << airportCode << "using exclusively airline '"
                  << airlineCode << "'" << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from " << cityName << " to " << airportCode << " ("
                      << bestPaths[0].size() - 1 << " stops) using exclusively airline '" << airlineCode << "':"
                      << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between any airport in the source city and any airport in the destination city that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param sourceCity City in which the paths start
 * @param destCity City in which the paths must end
 * @param airlineCode Code of the airline used in the path(s)
 */
void FlightManager::bfocitytocityPAirline(const std::string &sourceCity, const std::string &destCity,
                                          const std::string &airlineCode) {
    auto sourceAirports = findAirportsInCity(sourceCity);
    auto destAirports = findAirportsInCity(destCity);

    if (sourceAirports.empty() || destAirports.empty()) {
        std::cout << "Airports not found in one or both cities." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;

    for (auto &sourceAirport: sourceAirports) {
        for (auto &destAirport: destAirports) {
            auto paths = shortestPathsPAirline(sourceAirport, destAirport, airlineCode);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << sourceCity << " to " << destCity << " using exclusively airline '"
                  << airlineCode << "'" << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from " << sourceCity << " to " << destCity
                      << " using exclusively airline '" << airlineCode << "' (" << bestPaths[0].size() - 1 << " stops):"
                      << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between any airport in the specified city and the nearest airport(s) to the given coordinates that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param cityName City in which the paths start
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param airlineCode Code of the airline used in the path(s)
 */
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

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;

    for (auto &cityAirport: cityAirports) {
        for (auto &destAirport: nearestAirports) {
            auto paths = shortestPathsPAirline(cityAirport, destAirport, airlineCode);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << cityName << " to coordinates (" << lat << ", " << lon
                  << ") using exclusively airline '" << airlineCode << "'" << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from " << cityName << " to coordinates (" << lat << ", " << lon
                      << ") using exclusively airline '" << airlineCode << "' (" << bestPaths[0].size() - 1
                      << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given coordinates and a specified airport that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param airportCode Airport in which the paths must end
 * @param airlineCode Code of the airline used in the path(s)
 */
void FlightManager::bfoCoordinatestoAirportPAirline(double lat, double lon, const std::string &airportCode,
                                                    const std::string &airlineCode) {
    std::vector<Vertex<Airport> *> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No airport found near the given coordinates." << std::endl;
        return;
    }

    Vertex<Airport> *destAirport = findAirportVertexByNameOrCode(airportCode);
    if (!destAirport) {
        std::cout << "Destination airport not found: " << airportCode << std::endl;
        return;
    }

    for (auto nearestAirport: nearestAirports) {
        if (nearestAirport == destAirport) {
            std::cout << "The nearest airport is the destination airport." << std::endl;
            return;
        }
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &nearestAirport: nearestAirports) {
        auto paths = shortestPathsPAirline(nearestAirport, destAirport, airlineCode);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from the nearest airport to " << airportCode << "using exclusively airline '"
                  << airlineCode << "'" << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from coordinates(" << lat << "," << lon << ") to " << airportCode
                      << " using exclusively airline '" << airlineCode << "' (" << bestPaths[0].size() - 1 << " stops):"
                      << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given coordinates and any airport in a specified city that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param cityName City in which the paths must end
 * @param airlineCode Code of the airline used in the path(s)
 */
void FlightManager::bfoCoordinatestoCityPAirline(double lat, double lon, const std::string &cityName,
                                                 const std::string &airlineCode) {
    std::vector<Vertex<Airport> *> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    if (nearestAirports.empty()) {
        std::cout << "No airport found near the given coordinates." << std::endl;
        return;
    }

    auto cityAirports = findAirportsInCity(cityName);
    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << cityName << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;

    for (auto &nearestAirport: nearestAirports) {
        for (auto &cityAirport: cityAirports) {
            auto paths = shortestPathsPAirline(nearestAirport, cityAirport, airlineCode);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight paths found from the coordinates to any airport in " << cityName
                  << " using exclusively airline '" << airlineCode << "'" << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from coordinates (" << lat << ", " << lon << ") to " << cityName
                      << "using exclusively airline '" << airlineCode << "' "
                      << " (" << bestPaths[0].size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }

    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given source coordinates and the nearest airport(s) to the given destination coordinates that use exclusively the specified airline.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param sourceLat Latitude of the pair of source coordinates
 * @param sourceLon Longitude of the pair of source coordinates
 * @param destLat Latitude of the pair of source coordinates
 * @param destLon Longitude of the pair of source coordinates
 * @param airlineCode Code of the airline used in the path(s)
 */
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

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;

    for (auto &sourceNearestAirport: sourceNearestAirports) {
        for (auto &destNearestAirport: destNearestAirports) {
            auto paths = shortestPathsPAirline(sourceNearestAirport, destNearestAirport, airlineCode);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }


    if (bestPaths.empty()) {
        std::cout << "No flight path found between the nearest airports using exclusively airline '" << airlineCode
                  << "'" << std::endl;
    } else {
        for (const auto path: bestPaths) {
            std::cout << "Best flight option from coordinates (" << sourceLat << ", " << sourceLon
                      << ") to coordinates ("
                      << destLat << ", " << destLon << ") using exclusively airline '" << airlineCode << "' ("
                      << bestPaths[0].size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }

}

/**
 * @brief Function finds all shortest paths from a specified start airport to a specified end airport, with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices in the graph and E is the number of edges. This time complexity comes from the use of Dijkstra's algorithm with a priority queue.
 * @param startAirport Airport where the paths start
 * @param endAirport Airport where the paths must end
 * @param maxStops Number of maximum stops in the trip
 * @return Vector of vectors, where each inner vector represents a unique shortest path from the start airport to the end airport.
 */
std::vector<std::vector<Vertex<Airport> *>>
FlightManager::shortestPathsWithMaxStops(Vertex<Airport> *startAirport, Vertex<Airport> *endAirport, int maxStops) {
    std::priority_queue<std::pair<int, std::vector<Vertex<Airport> *>>, std::vector<std::pair<int, std::vector<Vertex<Airport> *>>>, std::greater<>> pq;
    std::unordered_map<Vertex<Airport> *, int> distance;
    distance[startAirport] = 0;
    pq.push({0, {startAirport}});

    std::set<std::vector<Vertex<Airport> *>> uniquePaths;

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();

        int currentDistance = current.first;
        auto currentPath = current.second;
        auto currentNode = currentPath.back();

        if (currentNode == endAirport && currentPath.size() <= maxStops + 1) {
            uniquePaths.insert(currentPath);
        }

        if (currentPath.size() > maxStops + 1) continue;

        for (const auto &flight: currentNode->getAdj()) {
            auto neighbor = flight.getDest();
            int newDistance = currentDistance + 1;

            if (distance.find(neighbor) == distance.end() || newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            } else if (newDistance == distance[neighbor] && currentPath.size() <= maxStops + 1) {
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            }
        }
    }

    return std::vector<std::vector<Vertex<Airport> *>>(uniquePaths.begin(), uniquePaths.end());
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between two airports, with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airportCode1 Airport where the paths start
 * @param airportCode2 Airport where the paths must end
 * @param maxStops Number of maximum stops in the trip
 */
void
FlightManager::bfoAirportToAirportMaxStops(const std::string &sourceCode, const std::string &targetCode, int maxStops) {
    auto sourceVertex = findAirportVertexByNameOrCode(sourceCode);
    auto targetVertex = findAirportVertexByNameOrCode(targetCode);

    if (!sourceVertex || !targetVertex) {
        std::cout << "Airport(s) not found." << std::endl;
        return;
    }

    if (sourceVertex == targetVertex) {
        std::cout << "Source and destination airports are the same." << std::endl;
        return;
    }

    auto paths = shortestPathsWithMaxStops(sourceVertex, targetVertex, maxStops);

    if (paths.empty()) {
        std::cout << "No paths found between " << sourceCode << " and " << targetCode << " within " << maxStops
                  << " stops." << std::endl;
    } else {
        for (const auto &path: paths) {
            std::cout << "Flight Option (" << path.size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds all shortest paths from a specified start airport to any airport in a specified city (without printing it), with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices in the graph and E is the number of edges. This time complexity comes from the use of Dijkstra's algorithm with a priority queue.
 * @param startAirport Airport where the paths start
 * @param cityName City where the paths must end
 * @param maxStops Number of maximum stops in the trip
 * @return Vector of vectors, where each inner vector represents a unique shortest path from the start airport to the end airport of the trip.
 */
std::vector<std::vector<Vertex<Airport> *>>
FlightManager::shortestPathsToCityWithMaxStops(Vertex<Airport> *startAirport, const std::string &cityName,
                                               int maxStops) {
    std::vector<Vertex<Airport> *> cityAirports = findAirportsInCity(cityName);

    std::priority_queue<std::pair<int, std::vector<Vertex<Airport> *>>,
            std::vector<std::pair<int, std::vector<Vertex<Airport> *>>>,
            std::greater<>> pq;

    std::unordered_map<Vertex<Airport> *, int> distance;
    distance[startAirport] = 0;

    pq.push({0, {startAirport}});

    std::set<std::vector<Vertex<Airport> *>> uniquePaths;

    while (!pq.empty()) {
        auto current = pq.top();
        pq.pop();

        int currentDistance = current.first;
        auto currentPath = current.second;
        auto currentNode = currentPath.back();

        if (std::find(cityAirports.begin(), cityAirports.end(), currentNode) != cityAirports.end() &&
            currentPath.size() <= maxStops + 1) {
            uniquePaths.insert(currentPath);
        }

        if (currentPath.size() > maxStops + 1) {
            continue;
        }

        for (const auto &flight: currentNode->getAdj()) {
            auto neighbor = flight.getDest();
            int newDistance = currentDistance + 1;

            if (distance.find(neighbor) == distance.end() || newDistance < distance[neighbor]) {
                distance[neighbor] = newDistance;
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            } else if (newDistance == distance[neighbor] && currentPath.size() <= maxStops + 1) {
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push({newDistance, newPath});
            }
        }
    }

    return std::vector<std::vector<Vertex<Airport> *>>(uniquePaths.begin(), uniquePaths.end());
}


/**
 * @brief Function finds and displays the best flight options (shortest paths) from a given source airport to any airport in a specified city, with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airport Airport where the paths start
 * @param cityName City in which the paths must end
 * @param maxStops Number of maximum stops in the trip
 */
void
FlightManager::bfoAirportToCityMaxStops(const std::string &sourceCode, const std::string &targetCity, int maxStops) {
    auto sourceVertex = findAirportVertexByNameOrCode(sourceCode);

    if (!sourceVertex) {
        std::cout << "Source airport not found." << std::endl;
        return;
    }

    auto paths = shortestPathsToCityWithMaxStops(sourceVertex, targetCity, maxStops);

    if (paths.empty()) {
        std::cout << "No paths found from " << sourceCode << " to " << targetCity << " within " << maxStops << " stops."
                  << std::endl;
    } else {
        for (const auto &path: paths) {
            std::cout << "Flight Option to " << targetCity << " (" << path.size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from a given source airport to the nearest airport(s) to specified coordinates (latitude and longitude), with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airportCode Airport where the paths start
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param maxStops Number of maximum stops in the trip
 */
void
FlightManager::bfoAirportToCoordinatesMaxStops(const std::string &sourceCode, double lat, double lon, int maxStops) {
    auto sourceAirport = findAirportVertexByNameOrCode(sourceCode);
    std::vector<Vertex<Airport> *> targetAirports = findNearestAirportToCoordinates(lat, lon);

    if (!sourceAirport || targetAirports.empty()) {
        std::cout << "Source airport or nearest airport not found." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &destAirport: targetAirports) {
        auto paths = shortestPathsWithMaxStops(sourceAirport, destAirport, maxStops);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << sourceCode << " to coordinates (" << lat << ", " << lon
                  << ") within " << maxStops << " stops." << std::endl;
    } else {
        for (const auto &path: bestPaths) {
            std::cout << "Flight Option to coordinates (" << lat << ", " << lon << ") with " << path.size() - 1
                      << " stops:" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from any airport in a specified city to a given destination airport, with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param cityName City in which the paths start
 * @param airportCode Airport where the paths must end
 * @param maxStops Number of maximum stops in the trip
 */
void
FlightManager::bfoCityToAirportMaxStops(const std::string &sourceCity, const std::string &targetCode, int maxStops) {
    std::vector<Vertex<Airport> *> sourceAirports = findAirportsInCity(sourceCity);
    Vertex<Airport> *targetAirport = findAirportVertexByNameOrCode(targetCode);

    if (sourceAirports.empty() || !targetAirport) {
        std::cout << "Source city or target airport not found." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &sourceAirport: sourceAirports) {
        auto paths = shortestPathsWithMaxStops(sourceAirport, targetAirport, maxStops);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << sourceCity << " to " << targetCode << " within " << maxStops
                  << " stops." << std::endl;
    } else {
        for (const auto &path: bestPaths) {
            std::cout << "Flight Option from " << sourceCity << " to " << targetCode << " (" << path.size() - 1
                      << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between any airport in the source city and any airport in the destination city, with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param sourceCity City in which the paths start
 * @param destCity City in which the paths must end
 * @param maxStops Number of maximum stops in the trip
 */
void FlightManager::bfoCityToCityMaxStops(const std::string &sourceCity, const std::string &targetCity, int maxStops) {
    std::vector<Vertex<Airport> *> sourceAirports = findAirportsInCity(sourceCity);
    std::vector<Vertex<Airport> *> targetAirports = findAirportsInCity(targetCity);

    if (sourceAirports.empty() || targetAirports.empty()) {
        std::cout << "Source city or target city not found." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &sourceAirport: sourceAirports) {
        for (auto &targetAirport: targetAirports) {
            auto paths = shortestPathsWithMaxStops(sourceAirport, targetAirport, maxStops);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found between " << sourceCity << " and " << targetCity << " within " << maxStops
                  << " stops." << std::endl;
    } else {
        for (const auto &path: bestPaths) {
            std::cout << "Flight Option from " << sourceCity << " to " << targetCity << " (" << path.size() - 1
                      << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between any airport in the specified city and the nearest airport(s) to the given coordinates, with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param cityName City in which the paths start
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param maxStops Number of maximum stops in the trip
 */
void FlightManager::bfoCityToCoordinatesMaxStops(const std::string &cityName, double lat, double lon, int maxStops) {
    std::vector<Vertex<Airport> *> cityAirports = findAirportsInCity(cityName);
    std::vector<Vertex<Airport> *> targetAirports = findNearestAirportToCoordinates(lat, lon);

    if (cityAirports.empty() || targetAirports.empty()) {
        std::cout << "City airports or nearest airport not found." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &cityAirport: cityAirports) {
        for (auto &targetAirport: targetAirports) {
            auto paths = shortestPathsWithMaxStops(cityAirport, targetAirport, maxStops);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << cityName << " to coordinates (" << lat << ", " << lon
                  << ") within " << maxStops << " stops." << std::endl;
    } else {
        for (const auto &path: bestPaths) {
            std::cout << "Flight Option from " << cityName << " to coordinates (" << lat << ", " << lon << ") with "
                      << path.size() - 1 << " stops:" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given coordinates and a specified airport, with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param airportCode Airport in which the paths must end
 * @param maxStops Number of maximum stops in the trip
 */
void FlightManager::bfoCoordinatesToAirportMaxStops(double lat, double lon, const std::string &targetAirportCode,
                                                    int maxStops) {
    std::vector<Vertex<Airport> *> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    Vertex<Airport> *targetAirport = findAirportVertexByNameOrCode(targetAirportCode);

    if (nearestAirports.empty() || !targetAirport) {
        std::cout << "Nearest airport or destination airport not found." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &nearestAirport: nearestAirports) {
        auto paths = shortestPathsWithMaxStops(nearestAirport, targetAirport, maxStops);
        if (paths.empty()) {
            continue;
        }
        if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
            bestPaths = paths;
        } else if (paths[0].size() == bestPaths[0].size()) {
            bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from coordinates (" << lat << ", " << lon << ") to airport "
                  << targetAirportCode << " within " << maxStops << " stops." << std::endl;
    } else {
        for (const auto &path: bestPaths) {
            std::cout << "Flight Option from coordinates (" << lat << ", " << lon << ") to airport "
                      << targetAirportCode << " with " << path.size() - 1 << " stops:" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given coordinates and any airport in a specified city, with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param cityName City in which the paths must end
 * @param maxStops Number of maximum stops in the trip
 */
void FlightManager::bfoCoordinatesToCityMaxStops(double lat, double lon, const std::string &targetCity, int maxStops) {
    std::vector<Vertex<Airport> *> nearestAirports = findNearestAirportToCoordinates(lat, lon);
    std::vector<Vertex<Airport> *> cityAirports = findAirportsInCity(targetCity);

    if (nearestAirports.empty() || cityAirports.empty()) {
        std::cout << "Nearest airport or destination city airports not found." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &nearestAirport: nearestAirports) {
        for (auto &cityAirport: cityAirports) {
            auto paths = shortestPathsWithMaxStops(nearestAirport, cityAirport, maxStops);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from coordinates (" << lat << ", " << lon << ") to city " << targetCity
                  << " within " << maxStops << " stops." << std::endl;
    } else {
        for (const auto &path: bestPaths) {
            std::cout << "Flight Option from coordinates (" << lat << ", " << lon << ") to city " << targetCity
                      << " with " << path.size() - 1 << " stops:" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given source coordinates and the nearest airport(s) to the given destination coordinates, with a specified maximum of stops.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param sourceLat Latitude of the pair of source coordinates
 * @param sourceLon Longitude of the pair of source coordinates
 * @param destLat Latitude of the pair of source coordinates
 * @param destLon Longitude of the pair of source coordinates
 * @param maxStops Number of maximum stops in the trip
 */
void
FlightManager::bfoCoordinatesToCoordinatesMaxStops(double sourceLat, double sourceLon, double destLat, double destLon,
                                                   int maxStops) {
    std::vector<Vertex<Airport> *> sourceNearestAirports = findNearestAirportToCoordinates(sourceLat, sourceLon);
    std::vector<Vertex<Airport> *> destNearestAirports = findNearestAirportToCoordinates(destLat, destLon);

    if (sourceNearestAirports.empty() || destNearestAirports.empty()) {
        std::cout << "No airports found near the source or destination coordinates." << std::endl;
        return;
    }

    std::vector<std::vector<Vertex<Airport> *>> bestPaths;
    for (auto &sourceAirport: sourceNearestAirports) {
        for (auto &destAirport: destNearestAirports) {
            auto paths = shortestPathsWithMaxStops(sourceAirport, destAirport, maxStops);
            if (paths.empty()) {
                continue;
            }
            if (bestPaths.empty() || paths[0].size() < bestPaths[0].size()) {
                bestPaths = paths;
            } else if (paths[0].size() == bestPaths[0].size()) {
                bestPaths.insert(bestPaths.end(), paths.begin(), paths.end());
            }
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found between the given coordinates within " << maxStops << " stops." << std::endl;
    } else {
        for (const auto &path: bestPaths) {
            std::cout << "Flight Option from coordinates (" << sourceLat << ", " << sourceLon << ") to coordinates ("
                      << destLat << ", " << destLon << ") with " << path.size() - 1 << " stops:" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}


//MAX Flight Distance

/**
 * @brief Function takes an angle in degrees (deg) as input and transforms it into radians
 * @details Time complexity: O(1)
 * @param deg Angle in degrees
 * @return Angle in radians
 */
double degToRad(double deg) {
    return deg * (M_PI / 180);
}

/**
 * @brief Function calculates the great-circle distance between two points (in this case two airports) on the Earth's surface using the Haversine formula
 * @details Time complexity: O(1)
 * @param airport1 First of the airports
 * @param airport2 Second of the airports
 * @return Distance in Km between the two airports
 */
double calculateDistance(Vertex<Airport> *airport1, Vertex<Airport> *airport2) {
    double lat1 = degToRad(airport1->getInfo().getLatitude());
    double lon1 = degToRad(airport1->getInfo().getLongitude());
    double lat2 = degToRad(airport2->getInfo().getLatitude());
    double lon2 = degToRad(airport2->getInfo().getLongitude());

    double dLat = lat2 - lat1;
    double dLon = lon2 - lon1;

    double a = pow(sin(dLat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dLon / 2), 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    const double RADIUS_EARTH_KM = 6371;
    return RADIUS_EARTH_KM * c;
}

/**
 * @brief Function finds all shortest paths from a specified start airport to a specified end airport, with a specified maximum of flight distance in the path (Distance in the "air", not simply the distance between the source and destination airports).
 * @details Time complexity: O((V+E)log V), where V is the number of vertices in the graph and E is the number of edges. This time complexity comes from the use of Dijkstra's algorithm with a priority queue.
 * @param startAirport Airport where the paths start
 * @param endAirport Airport where the paths must end
 * @param maxDistance Maximum distance done by plane
 * @return Vector of vectors, where each inner vector represents a unique shortest path from the start airport to the end airport.
 */
std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> FlightManager::shortestPathsWithinDistance(
        Vertex<Airport> *startAirport, Vertex<Airport> *endAirport, int maxDistance) {


    std::priority_queue<std::tuple<int, std::vector<Vertex<Airport> *>, int, int>,
            std::vector<std::tuple<int, std::vector<Vertex<Airport> *>, int, int>>,
            std::greater<>> pq;

    pq.push(std::make_tuple(0, std::vector<Vertex<Airport> *>{startAirport}, 0,
                            calculateDistance(startAirport, endAirport)));

    std::set<std::pair<std::vector<Vertex<Airport> *>, int>> uniquePaths;
    int iterationCounter = 0;

    while (!pq.empty() && iterationCounter < MAX_ITERATIONS) {
        auto [currentStops, currentPath, currentTotalDistance, heuristicDistance] = pq.top();
        pq.pop();

        Vertex<Airport> *currentNode = currentPath.back();

        if (currentNode == endAirport && currentTotalDistance <= maxDistance) {
            uniquePaths.insert(std::make_pair(currentPath, currentTotalDistance));
            continue;
        }

        iterationCounter++;

        for (const auto &flight: currentNode->getAdj()) {
            auto neighbor = flight.getDest();
            int flightDistance = calculateDistance(currentNode, neighbor);

            int newTotalDistance = currentTotalDistance + flightDistance;
            if (newTotalDistance <= maxDistance) {
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                int newHeuristicDistance = calculateDistance(neighbor, endAirport);
                pq.push(std::make_tuple(currentStops + 1, newPath, newTotalDistance, newHeuristicDistance));
            }
        }
    }

    return std::vector<std::pair<std::vector<Vertex<Airport> *>, int>>(uniquePaths.begin(), uniquePaths.end());
}


/**
 * @brief Function finds and displays the best flight options (shortest paths) between two airports, with a specified maximum of flight distance in the path.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airportCode1 Airport where the paths start
 * @param airportCode2 Airport where the paths must end
 * @param maxDistance Maximum distance done by plane
 */
void FlightManager::bfoAirportToAirportMaxDistance(const std::string &sourceCode, const std::string &targetCode,
                                                   int maxDistance) {
    auto sourceVertex = findAirportVertexByNameOrCode(sourceCode);
    auto targetVertex = findAirportVertexByNameOrCode(targetCode);

    if (!sourceVertex || !targetVertex) {
        std::cout << "Airport(s) not found." << std::endl;
        return;
    }

    if (sourceVertex == targetVertex) {
        std::cout << "Source and destination airports are the same." << std::endl;
        return;
    }

    auto pathOptions = shortestPathsWithinDistance(sourceVertex, targetVertex, maxDistance);

    if (pathOptions.empty()) {
        std::cout << "No paths found within " << maxDistance << " km." << std::endl;
    } else {
        for (const auto &[path, totalDistance]: pathOptions) {
            std::cout << "Flight Option within " << maxDistance << " km (Total distance: " << totalDistance << " km, "
                      << path.size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds all shortest paths from a specified start airport to any airport in a specified city (without printing it), with a specified maximum of flight distance in the path.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices in the graph and E is the number of edges. This time complexity comes from the use of Dijkstra's algorithm with a priority queue.
 * @param startAirport Airport where the paths start
 * @param cityName City where the paths must end
 * @param maxDistance Maximum distance done by plane
 * @return Vector of vectors, where each inner vector represents a unique shortest path from the start airport to the end airport of the trip.
 */
std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> FlightManager::shortestPathsToCityWithinDistance(
        Vertex<Airport> *startAirport, const std::string &cityName, int maxDistance) {

    std::vector<Vertex<Airport> *> cityAirports = findAirportsInCity(cityName);

    std::priority_queue<std::tuple<int, std::vector<Vertex<Airport> *>, int>,
            std::vector<std::tuple<int, std::vector<Vertex<Airport> *>, int>>,
            std::greater<>> pq;

    pq.push(std::make_tuple(0, std::vector<Vertex<Airport> *>{startAirport}, 0));

    std::set<std::pair<std::vector<Vertex<Airport> *>, int>> uniquePaths;
    int iterationCounter = 0;

    while (!pq.empty() && iterationCounter < MAX_ITERATIONS) {
        auto [currentStops, currentPath, currentTotalDistance] = pq.top();
        pq.pop();
        iterationCounter++;

        Vertex<Airport> *currentNode = currentPath.back();

        if (std::find(cityAirports.begin(), cityAirports.end(), currentNode) != cityAirports.end() &&
            currentTotalDistance <= maxDistance) {
            uniquePaths.insert(std::make_pair(currentPath, currentTotalDistance));
            continue;
        }

        for (const auto &flight: currentNode->getAdj()) {
            auto neighbor = flight.getDest();
            int flightDistance = calculateDistance(currentNode, neighbor);

            int newTotalDistance = currentTotalDistance + flightDistance;
            if (newTotalDistance <= maxDistance) {
                std::vector<Vertex<Airport> *> newPath = currentPath;
                newPath.push_back(neighbor);
                pq.push(std::make_tuple(currentStops + 1, newPath, newTotalDistance));
            }
        }
    }

    return std::vector<std::pair<std::vector<Vertex<Airport> *>, int>>(uniquePaths.begin(), uniquePaths.end());
}


/**
 * @brief Function finds and displays the best flight options (shortest paths) from a given source airport to any airport in a specified city, with a specified maximum of flight distance in the path.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airport Airport where the paths start
 * @param cityName City in which the paths must end
 * @param maxDistance Maximum distance done by plane
 */
void FlightManager::bfoAirportToCityMaxDistance(const std::string &sourceCode, const std::string &targetCity,
                                                int maxDistance) {
    auto sourceVertex = findAirportVertexByNameOrCode(sourceCode);
    if (!sourceVertex) {
        std::cout << "Source airport not found." << std::endl;
        return;
    }

    auto pathOptions = shortestPathsToCityWithinDistance(sourceVertex, targetCity, maxDistance);

    if (pathOptions.empty()) {
        std::cout << "No paths found within " << maxDistance << " km to " << targetCity << "." << std::endl;
    } else {
        for (const auto &[path, totalDistance]: pathOptions) {
            std::cout << "Flight Option to " << targetCity << " within " << maxDistance << " km (Total distance: "
                      << totalDistance << " km, " << path.size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from a given source airport to the nearest airport(s) to specified coordinates (latitude and longitude), with a specified maximum of flight distance in the path.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param airportCode Airport where the paths start
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param maxDistance Maximum distance done by plane
 */
void FlightManager::bfoAirportToCoordinatesMaxDistance(const std::string &sourceCode, double lat, double lon,
                                                       int maxDistance) {
    auto sourceAirport = findAirportVertexByNameOrCode(sourceCode);
    std::vector<Vertex<Airport> *> targetAirports = findNearestAirportToCoordinates(lat, lon);

    if (!sourceAirport || targetAirports.empty()) {
        std::cout << "Source airport or nearest airport not found." << std::endl;
        return;
    }

    std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> bestPaths;
    for (auto &destAirport: targetAirports) {
        auto pathOptions = shortestPathsWithinDistance(sourceAirport, destAirport, maxDistance);
        for (auto &pathOption: pathOptions) {
            bestPaths.push_back(pathOption);
        }
    }

    if (bestPaths.empty()) {
        std::cout << "No flight path found from " << sourceCode << " to coordinates (" << lat << ", " << lon
                  << ") within " << maxDistance << " km." << std::endl;
    } else {
        for (const auto &[path, totalDistance]: bestPaths) {
            std::cout << "Flight Option to coordinates (" << lat << ", " << lon << ") with " << path.size() - 1
                      << " stops (Total distance: " << totalDistance << " km):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) from any airport in a specified city to a given destination airport, with a specified maximum of flight distance in the path.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param cityName City in which the paths start
 * @param airportCode Airport where the paths must end
 * @param maxDistance Maximum distance done by plane
 */
void FlightManager::bfoCityToAirportMaxDistance(const std::string &cityName, const std::string &airportCode,
                                                int maxDistance) {
    auto cityAirports = findAirportsInCity(cityName);
    auto targetAirport = findAirportVertexByNameOrCode(airportCode);

    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << cityName << std::endl;
        return;
    }

    if (!targetAirport) {
        std::cout << "Target airport not found: " << airportCode << std::endl;
        return;
    }

    std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> pathOptions;
    for (auto &cityAirport: cityAirports) {
        auto paths = shortestPathsWithinDistance(cityAirport, targetAirport, maxDistance);
        pathOptions.insert(pathOptions.end(), paths.begin(), paths.end());
    }

    if (pathOptions.empty()) {
        std::cout << "No paths found within " << maxDistance << " km from " << cityName << " to " << airportCode << "."
                  << std::endl;
    } else {
        for (const auto &[path, totalDistance]: pathOptions) {
            std::cout << "Flight Option within " << maxDistance << " km (Total distance: " << totalDistance << " km, "
                      << path.size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).with a specified maximum of flight distance in the path.
 * @param sourceCity City in which the paths start
 * @param destCity City in which the paths must end
 * @param maxDistance Maximum distance done by plane
 */
void
FlightManager::bfoCityToCityMaxDistance(const std::string &sourceCity, const std::string &destCity, int maxDistance) {
    auto sourceAirports = findAirportsInCity(sourceCity);
    auto destAirports = findAirportsInCity(destCity);

    if (sourceAirports.empty() || destAirports.empty()) {
        std::cout << "Airports not found in one or both cities." << std::endl;
        return;
    }

    std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> pathOptions;
    for (auto &sourceAirport: sourceAirports) {
        for (auto &destAirport: destAirports) {
            auto paths = shortestPathsWithinDistance(sourceAirport, destAirport, maxDistance);
            pathOptions.insert(pathOptions.end(), paths.begin(), paths.end());
        }
    }

    if (pathOptions.empty()) {
        std::cout << "No paths found within " << maxDistance << " km between " << sourceCity << " and " << destCity
                  << "." << std::endl;
    } else {
        for (const auto &[path, totalDistance]: pathOptions) {
            std::cout << "Flight Option within " << maxDistance << " km between " << sourceCity << " and " << destCity
                      << " (Total distance: " << totalDistance << " km, " << path.size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between any airport in the specified city and the nearest airport(s) to the given coordinates, with a specified maximum of flight distance in the path.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param cityName City in which the paths start
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param maxDistance Maximum distance done by plane
 */
void
FlightManager::bfoCityToCoordinatesMaxDistance(const std::string &cityName, double lat, double lon, int maxDistance) {
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

    std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> pathOptions;
    for (auto &cityAirport: cityAirports) {
        for (auto &nearestAirport: nearestAirports) {
            auto paths = shortestPathsWithinDistance(cityAirport, nearestAirport, maxDistance);
            pathOptions.insert(pathOptions.end(), paths.begin(), paths.end());
        }
    }

    if (pathOptions.empty()) {
        std::cout << "No paths found within " << maxDistance << " km from " << cityName << " to coordinates (" << lat
                  << ", " << lon << ")." << std::endl;
    } else {
        for (const auto &[path, totalDistance]: pathOptions) {
            std::cout << "Flight Option within " << maxDistance << " km from " << cityName << " to coordinates ("
                      << lat << ", " << lon << ") (Total distance: " << totalDistance << " km, " << path.size() - 1
                      << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given coordinates and a specified airport, with a specified maximum of flight distance in the path.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param airportCode Airport in which the paths must end
 * @param maxDistance Maximum distance done by plane
 */
void FlightManager::bfoCoordinatesToAirportMaxDistance(double sourceLat, double sourceLon,
                                                       const std::string &targetAirportCode, int maxDistance) {
    auto nearestAirports = findNearestAirportToCoordinates(sourceLat, sourceLon);
    if (nearestAirports.empty()) {
        std::cout << "No nearby airport found for the given coordinates." << std::endl;
        return;
    }

    auto targetAirport = findAirportVertexByNameOrCode(targetAirportCode);
    if (!targetAirport) {
        std::cout << "Target airport not found: " << targetAirportCode << std::endl;
        return;
    }

    std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> pathOptions;
    for (auto &nearestAirport: nearestAirports) {
        auto paths = shortestPathsWithinDistance(nearestAirport, targetAirport, maxDistance);
        pathOptions.insert(pathOptions.end(), paths.begin(), paths.end());
    }

    if (pathOptions.empty()) {
        std::cout << "No paths found within " << maxDistance << " km from coordinates (" << sourceLat << ", "
                  << sourceLon << ") to " << targetAirportCode << "." << std::endl;
    } else {
        for (const auto &[path, totalDistance]: pathOptions) {
            std::cout << "Flight Option within " << maxDistance << " km from coordinates (" << sourceLat << ", "
                      << sourceLon << ") to " << targetAirportCode
                      << " (Total distance: " << totalDistance << " km, " << path.size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given coordinates and any airport in a specified city, with a specified maximum of flight distance in the path.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param lat Latitude of the pair of coordinates
 * @param lon Longitude of the pair of coordinates
 * @param cityName City in which the paths must end
 * @param maxDistance Maximum distance done by plane
 */
void FlightManager::bfoCoordinatesToCityMaxDistance(double sourceLat, double sourceLon, const std::string &targetCity,
                                                    int maxDistance) {
    auto nearestAirports = findNearestAirportToCoordinates(sourceLat, sourceLon);
    if (nearestAirports.empty()) {
        std::cout << "No nearby airport found for the given coordinates." << std::endl;
        return;
    }

    std::vector<Vertex<Airport> *> cityAirports = findAirportsInCity(targetCity);
    if (cityAirports.empty()) {
        std::cout << "No airports found in the city: " << targetCity << std::endl;
        return;
    }

    std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> pathOptions;
    for (auto &nearestAirport: nearestAirports) {
        for (auto &cityAirport: cityAirports) {
            auto paths = shortestPathsWithinDistance(nearestAirport, cityAirport, maxDistance);
            pathOptions.insert(pathOptions.end(), paths.begin(), paths.end());
        }
    }

    if (pathOptions.empty()) {
        std::cout << "No paths found within " << maxDistance << " km from coordinates (" << sourceLat << ", "
                  << sourceLon << ") to " << targetCity << "." << std::endl;
    } else {
        for (const auto &[path, totalDistance]: pathOptions) {
            std::cout << "Flight Option within " << maxDistance << " km from coordinates (" << sourceLat << ", "
                      << sourceLon << ") to " << targetCity
                      << " (Total distance: " << totalDistance << " km, " << path.size() - 1 << " stops):" << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

/**
 * @brief Function finds and displays the best flight options (shortest paths) between the nearest airport(s) to the given source coordinates and the nearest airport(s) to the given destination coordinates, with a specified maximum of flight distance in the path.
 * @details Time complexity: O((V+E)log V), where V is the number of vertices (airports) in the graph and E is the number of edges (flights).
 * @param sourceLat Latitude of the pair of source coordinates
 * @param sourceLon Longitude of the pair of source coordinates
 * @param destLat Latitude of the pair of source coordinates
 * @param destLon Longitude of the pair of source coordinates
 * @param maxDistance Maximum distance done by plane
 */
void FlightManager::bfoCoordinatesToCoordinatesMaxDistance(double sourceLat, double sourceLon, double destLat,
                                                           double destLon, int maxDistance) {
    auto sourceNearestAirports = findNearestAirportToCoordinates(sourceLat, sourceLon);
    auto destNearestAirports = findNearestAirportToCoordinates(destLat, destLon);

    if (sourceNearestAirports.empty() || destNearestAirports.empty()) {
        std::cout << "No nearby airports found for the given coordinates." << std::endl;
        return;
    }

    std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> pathOptions;
    for (auto &sourceAirport: sourceNearestAirports) {
        for (auto &destAirport: destNearestAirports) {
            auto paths = shortestPathsWithinDistance(sourceAirport, destAirport, maxDistance);
            pathOptions.insert(pathOptions.end(), paths.begin(), paths.end());
        }
    }

    if (pathOptions.empty()) {
        std::cout << "No paths found within " << maxDistance << " km between coordinates (" << sourceLat << ", "
                  << sourceLon << ") and (" << destLat << ", " << destLon << ")." << std::endl;
    } else {
        for (const auto &[path, totalDistance]: pathOptions) {
            std::cout << "Flight Option within " << maxDistance << " km between coordinates (" << sourceLat << ", "
                      << sourceLon << ") and (" << destLat << ", " << destLon
                      << ") (Total distance: " << totalDistance << " km, " << path.size() - 1 << " stops):"
                      << std::endl;
            for (const auto &airport: path) {
                std::cout << "- " << airport->getInfo().getName() << " (" << airport->getInfo().getCode() << ")"
                          << " at coordinates (" << airport->getInfo().getLatitude() << ", "
                          << airport->getInfo().getLongitude() << ")"
                          << std::endl;
            }
        }
    }
}

