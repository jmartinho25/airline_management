
#ifndef PROJECT_2_PARSING_H
#define PROJECT_2_PARSING_H


#include <string>
#include "Airport.h"
#include "AirportsGraph.h"

/**
 * @brief Class that allows the parsing of the files
 */
class Parsing {
public:

    void parseAirports(const std::string &filename, AirportsGraph &airportsGraph);

    void parseAirlines(const string &filename, AirportsGraph &airportsGraph);

    void parseFlights(const string &filename, AirportsGraph &airportsGraph);
};


#endif //PROJECT_2_PARSING_H
