//
// Created by darksystem on 25-12-2023.
//

#ifndef PROJECT_2_FLIGHTMANAGER_H
#define PROJECT_2_FLIGHTMANAGER_H
#include "AirportsGraph.h"


class FlightManager {
private:
    AirportsGraph airportsGraph;
public:
    FlightManager();

    void loadAirports(const std::string &filename);
    void loadAirlines(const std::string &filename);
    void loadFlights(const std::string &filename);


    void listnrairportsflights();

    void listflightsfromairport();

    void listflightsfromcity();

    void listflightspercityperairline();

    void listdiffcountriesperairpercity();

    void listnrdestavailable();
};



#endif //PROJECT_2_FLIGHTMANAGER_H
