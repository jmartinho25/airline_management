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




