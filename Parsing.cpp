
#include "Parsing.h"
#include "Graph.h"
#include "Airport.h"
#include "AirportsGraph.h"
#include "Airlines.h"
#include <fstream>
#include <sstream>

/**
 * @brief Allows the parsing of the Airports
 * @details Time complexity: O(n) where n is the number of lines in the file
 * @param filename the given file
 * @param airportsGraph where the Airports will be stored
 */
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

/**
 * @brief Allows the parsing of the Airlines
 * @details Time complexity: O(n) where n is the number of lines in the file
 * @param filename the given file
 * @param airportsGraph where the Airlines will be stored
 */
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

/**
 * @brief Allows the parsing of the Flights
 * @details Time complexity: O(n) where n is the number of lines in the file
 * @param filename the given file
 * @param airportsGraph where the Flights will be stored
 */
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
