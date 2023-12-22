//
// Created by darksystem on 21-12-2023.
//
#include "Parsing.h"
#include "Graph.h"
#include "Airport.h"
#include "AirportsGraph.h"
#include <fstream>
#include <sstream>


void Parsing::parseAirports(const std::string &filename,AirportsGraph &airportsGraph){
    std::ifstream file(filename);
    std::string line;
    getline(file,line);
    while(getline(file,line)){
            std::stringstream linestream(line);
            std::string code, name, city, country;
            float latitude,longitude;
            char delimiter;
            std::getline(linestream, code, ',');
            std::getline(linestream, name, ',');
            std::getline(linestream, city, ',');
            std::getline(linestream, country, ',');
            linestream>>latitude>>delimiter;
            linestream>>longitude;
            airportsGraph.addAirport(code, name, city, country, latitude, longitude);


    }
}

