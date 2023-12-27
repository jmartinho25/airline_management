//
// Created by darksystem on 21-12-2023.
//

#ifndef PROJECT_2_PARSING_H
#define PROJECT_2_PARSING_H


#include <string>
#include "Airport.h"
#include "AirportsGraph.h"

class Parsing {
public:

    void parseAirports(const std::string &filename, AirportsGraph &airportsGraph);

    void parseAirlines(const string &filename, AirportsGraph &airportsGraph);

    void parseFlights(const string &filename, AirportsGraph &airportsGraph);
};


#endif //PROJECT_2_PARSING_H
