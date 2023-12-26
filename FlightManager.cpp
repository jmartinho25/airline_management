//
// Created by darksystem on 25-12-2023.
//

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

FlightManager::FlightManager() {;
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
    for(const auto &vertex:airportsGraph.getGraph().getVertexSet()){
        totalFlights+=vertex->getAdj().size();
    }
    std::cout << "Global number of airports and flights:" << std::endl;
    std::cout<<"Total Airports: "<<totalAirports<<std::endl;
    std::cout<<"Total Flights: "<<totalFlights<<std::endl;

}

void FlightManager::listflightsfromairport() {
    std::string airportCode;
    std::cout<<"Enter airport code: ";
    std::cin>>airportCode;
    Vertex<Airport>* airport = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));
    if(airport) {
        std::set<std::string> uniqueAirlines;
        for(auto &flight:airport->getAdj()){
            uniqueAirlines.insert(flight.getDest()->getInfo().getCode());
        }
        std::cout << "Flights from Airport " << airportCode << ":" << std::endl;
        std::cout << "Number of flights: " << airport->getAdj().size() << std::endl;
        std::cout << "Number of airlines: " << uniqueAirlines.size() << std::endl;

    }
    else{
        std::cout << "Airport with code " << airportCode << " not found." << std::endl;
    }
}


std::string getAirlineName(const std::string& airlineCode, const AirportsGraph& airportsGraph) {
    for (const auto& airline : airportsGraph.getAirlines()) {
        if (airline.getCode() == airlineCode) {
            return airline.getName();
        }
    }
    return "Unknown";
}
void FlightManager::listflightspercityperairline() {
    std::map<std::pair<std::string, std::string>, int> flightsPerCityAirline;

    for (auto& vertex : airportsGraph.getGraph().getVertexSet()) {
        for (Edge<Airport> flight : vertex->getAdj()) {
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

    for (const auto& entry : flightsPerCityAirline) {
        std::cout << "City: " << entry.first.first << " | Airline: " << entry.first.second << " | Flights: " << entry.second << std::endl;
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
    if (numCountries == 0){
        std::cout << "No flights found from " << city << std::endl;
}
       else{
    std::cout << "Number of different countries flown to from " << city << ": " << numCountries << std::endl;
}
}

void FlightManager::listnrdestavailable() {
    cout << "1. By airports" << endl;
    cout << "2. By cities" << endl;
    cout << "3. By countries" << endl;
    cout << "Choose the criteria you would like the destinations to be showed in: ";
    int choice;
    cin >> choice;
    cout << "Enter the airport code: ";
    string airportCode;
    cin >> airportCode;
    Vertex<Airport>* sourceVertex = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));

    switch (choice) {
        case 1:
//            cout << "Enter the airport code: ";
//            string airportCode;
//            cin >> airportCode;
//            Vertex<Airport>* sourceVertex = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));

            if (sourceVertex) {
                const std::vector<Edge<Airport>>& flights = sourceVertex->getAdj();
                int numAirportsAvailable = flights.size();

                if (numAirportsAvailable > 0) {
                    std::cout << "Number of airports available for " << airportCode << ": " << numAirportsAvailable << std::endl;
                    std::cout << "List of available airports:" << std::endl;

                    for (const Edge<Airport>& flight : flights) {
                        const Airport& targetAirport = flight.getDest()->getInfo();
                        std::cout << "- " << targetAirport.getCode() << " (" << targetAirport.getName() << ")" << std::endl;
                    }
                } else {
                    std::cout << "No airports available for " << airportCode << std::endl;
                }
            } else {
                std::cout << "Airport with code " << airportCode << " not found" << std::endl;
            }
            break;
        case 2:
//            cout << "Enter the airport code: ";
//            string airportCode;
//            cin >> airportCode;
//            Vertex<Airport>* sourceVertex = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));

            if (sourceVertex) {
                const std::vector<Edge<Airport>>& flights = sourceVertex->getAdj();
                std::unordered_set<std::string> uniqueCities;

                for (const Edge<Airport>& flight : flights) {
                    const Airport& targetAirport = flight.getDest()->getInfo();
                    uniqueCities.insert(targetAirport.getCity());
                }

                int numCitiesAvailable = uniqueCities.size();

                if (numCitiesAvailable > 0) {
                    std::cout << "Number of cities available for " << airportCode << ": " << numCitiesAvailable << std::endl;
                    std::cout << "List of available cities:" << std::endl;

                    for (const std::string& city : uniqueCities) {
                        std::cout << "- " << city << std::endl;
                    }
                } else {
                    std::cout << "No cities available for " << airportCode << std::endl;
                }
            } else {
                std::cout << "Airport with code " << airportCode << " not found" << std::endl;
            }
            break;


        case 3:
            if (sourceVertex) {
                const std::vector<Edge<Airport>>& flights = sourceVertex->getAdj();
                std::unordered_set<std::string> uniqueCountries;

                for (const Edge<Airport>& flight : flights) {
                    const Airport& targetAirport = flight.getDest()->getInfo();
                    uniqueCountries.insert(targetAirport.getCountry());
                }

                int numCountriesAvailable = uniqueCountries.size();

                if (numCountriesAvailable > 0) {
                    std::cout << "Number of countries available for " << airportCode << ": " << numCountriesAvailable << std::endl;
                    std::cout << "List of available countries:" << std::endl;

                    for (const std::string& country : uniqueCountries) {
                        std::cout << "- " << country << std::endl;
                    }
                } else {
                    std::cout << "No countries available for " << airportCode << std::endl;
                }
            } else {
                std::cout << "Airport with code " << airportCode << " not found" << std::endl;
            }
            break;


    }
}
void FlightManager::listReachableDestinations(const std::string &airportCode, int maxStops, int criteria) {
    Vertex<Airport>* startVertex = airportsGraph.getGraph().findVertex(Airport(airportCode, "", "", "", 0.0, 0.0));
    if (!startVertex) {
        std::cout << "Airport not found." << std::endl;
        return;
    }

    std::unordered_set<std::string> visitedAirports;
    std::unordered_set<std::string> visitedCities;
    std::unordered_set<std::string> visitedCountries;

    std::queue<std::pair<Vertex<Airport>*, int>> queue;
    queue.push({startVertex, 0});

    while (!queue.empty()) {
        auto [currentVertex, stops] = queue.front();
        queue.pop();

        if (stops > maxStops) continue;

        Airport currentAirport = currentVertex->getInfo();
        visitedAirports.insert(currentAirport.getCode());
        visitedCities.insert(currentAirport.getCity());
        visitedCountries.insert(currentAirport.getCountry());

        for (const Edge<Airport>& edge : currentVertex->getAdj()) {
            Vertex<Airport>* nextVertex = edge.getDest();
            if (visitedAirports.find(nextVertex->getInfo().getCode()) == visitedAirports.end()) {
                queue.push({nextVertex, stops + 1});
            }
        }
    }

    int resultCount = 0;
    switch (criteria) {
        case 1: // Airports
            resultCount = visitedAirports.size();
            break;
        case 2: // Cities
            resultCount = visitedCities.size();
            break;
        case 3: // Countries
            resultCount = visitedCountries.size();
            break;
        default:
            std::cout << "Invalid criteria." << std::endl;
            return;
    }

    std::cout << "Number of reachable destinations: " << resultCount << std::endl;
}





