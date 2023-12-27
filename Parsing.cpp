//
// Created by darksystem on 21-12-2023.
//
#include "Parsing.h"
#include "Graph.h"
#include "Airport.h"
#include "AirportsGraph.h"
#include "Airlines.h"
#include <fstream>
#include <sstream>


void Parsing::parseAirports(const std::string &filename, AirportsGraph &airportsGraph) {
    std::ifstream file(filename);
    std::string line;
    getline(file, line);
    while (getline(file, line)) {
        std::stringstream linestream(line);
        std::string code, name, city, country;
        float latitude, longitude;
        char delimiter;
        std::getline(linestream, code, ',');
        std::getline(linestream, name, ',');
        std::getline(linestream, city, ',');
        std::getline(linestream, country, ',');
        linestream >> latitude >> delimiter;
        linestream >> longitude;
        airportsGraph.addAirport(code, name, city, country, latitude, longitude);


    }
}

void Parsing::parseAirlines(const std::string &filename, AirportsGraph &airportsGraph) {
    std::ifstream file(filename);
    std::string line;
    getline(file, line);
    while (getline(file, line)) {
        std::stringstream linestream(line);
        std::string code, name, callsign, country;

        std::getline(linestream, code, ',');
        std::getline(linestream, name, ',');
        std::getline(linestream, callsign, ',');
        std::getline(linestream, country, ',');
        Airline airline(code, name, callsign, country);
        airportsGraph.addAirline(airline);
    }

}

void Parsing::parseFlights(const std::string &filename, AirportsGraph &airportsGraph) {
    std::ifstream file(filename);
    std::string line;
    getline(file, line);
    while (getline(file, line)) {
        std::stringstream linestream(line);
        std::string source, target, airline;

        std::getline(linestream, source, ',');
        std::getline(linestream, target, ',');
        std::getline(linestream, airline, ',');
        airportsGraph.addFlight(source, target, airline);
    }
}
