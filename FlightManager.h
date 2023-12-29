//
// Created by darksystem on 25-12-2023.
//

#ifndef PROJECT_2_FLIGHTMANAGER_H
#define PROJECT_2_FLIGHTMANAGER_H

#include <unordered_map>
#include "AirportsGraph.h"
#include "Utils.h"


class FlightManager {
private:
    AirportsGraph airportsGraph;

    Vertex<Airport>* findNearestAirportToCoordinates(double lat, double lon);
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

    void listReachableDestinations(const string &airportCode, int maxStops, int criteria);

    void listmaxstopsbetweenairports();

    int calculateStopsBFS(Vertex<Airport> *source, vector<std::pair<std::string, std::string>> &aux);

    void identifytopkairport();

    void findEssentialAirportsUtil(const std::string &u, std::unordered_map<std::string, int> &disc,
                                    std::unordered_map<std::string, int> &low,
                                    std::unordered_map<std::string, std::string> &parent,
                                    std::unordered_set<std::string> &articulationPoints, int &time);

    void findEssentialAirports();

    void bfoairporttoairport(string airport1, string airport2);

    void bfoairporttocity(string airport, string cityName);

    void bfocitytoairport(string cityname, string airport);

    Vertex<Airport> *findAirportVertexByName(string airportName);

    void bfoairporttocoordinates(const std::string& airportCode, double lat, double lon);


    void dfsVisit(Vertex<Airport> *pVertex, unordered_set<std::string> &airports, unordered_set<std::string> &cities,
                  unordered_set<std::string> &countries);
};


#endif //PROJECT_2_FLIGHTMANAGER_H
