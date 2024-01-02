//
// Created by darksystem on 25-12-2023.
//

#ifndef PROJECT_2_FLIGHTMANAGER_H
#define PROJECT_2_FLIGHTMANAGER_H

#include <unordered_map>
#include "AirportsGraph.h"
#include "Utils.h"

/**
 * @brief Class that implements all functionalities in our code
 */
class FlightManager {
private:
    AirportsGraph airportsGraph;

    std::vector<Vertex<Airport>*> findNearestAirportToCoordinates(double lat, double lon);

    vector<Vertex<Airport>*> findAirportsInCity(const std::string& cityName);

    Vertex<Airport>* findAirportVertexByNameOrCode(const std::string& airportIdentifier);

    std::pair<int, std::vector<Airport>> findBestFlightPath(Vertex<Airport>* sourceVertex, Vertex<Airport>* destVertex);

    Vertex<Airport> *findAirportVertexByName(string airportName);

    std::pair<int, std::vector<Airport>> findBestFlightPathPAirline(
            Vertex<Airport>* sourceVertex, Vertex<Airport>* destVertex, const std::string& airlineCode);

    std::vector<std::vector<Vertex<Airport>*>> shortestPathsPAirline(
            Vertex<Airport>* startAirport, Vertex<Airport>* endAirport, const std::string& airlineCode);

public:
     FlightManager() ;

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

    void dfsVisit(Vertex<Airport> *pVertex, unordered_set<std::string> &airports, unordered_set<std::string> &cities,
                  unordered_set<std::string> &countries);

    //Find Best Path Functions

    void bfoairporttoairport(const string& airport1,const string& airport2);

    void bfoairporttocity(const string& airport,const string& cityName);

    void bfocitytoairport(const string& cityname,const string& airport);

    void bfoairporttocoordinates(const string& airportCode, double lat, double lon);

    void bfocitytocity(const string& sourceCity, const string& destCity);

    void bfocitytocoordinates(const string& cityName, double lat, double lon);

    void bfoCoordinatestoAirport(double lat, double lon, const string& airportCode);

    void bfoCoordinatestoCity(double lat, double lon, const std::string& cityName);

    void bfoCoordinatestoCoordinates(double sourceLat, double sourceLon, double destLat, double destLon);

    std::vector<std::vector<Vertex<Airport>*>> shortestPaths(Vertex<Airport>* startAirport,Vertex<Airport>* endAirport);

    //FILTERS
    //PREFERRED AIRLINE
    void bfoairporttoairportPAirline(const string& airportCode1, const string& airportCode2, const std::string& airlineCode);

    void bfoairporttocityPAirline(const string& airport, const string& cityName, const std::string& airlineCode);

    void bfoairporttocoordinatesPAirline(const string& airportCode, double lat, double lon, const std::string& airlineCode);

    void bfocitytoairportPAirline(const string& cityname,const string& airport, const std::string& airlineCode);

    void bfocitytocityPAirline(const string& sourceCity, const string& destCity, const std::string& airlineCode);

    void bfocitytocoordenatesPAirline(const string& sourceCity, double lat, double lon, const std::string& airlineCode);

    void bfoCoordinatestoAirportPAirline(double lat, double lon, const string& airportCode,const std::string& airlineCode );

    void bfoCoordinatestoCityPAirline(double lat, double lon, const std::string& cityName, const std::string& airlineCode);

    void bfoCoordinatestoCoordinatesPAirline(double sourceLat, double sourceLon, double destLat, double destLon, const std::string& airlineCode);

    void dfs_artic(Vertex<Airport> *v, unordered_set<string> &essentialAirports, unsigned int index) const;
};


#endif //PROJECT_2_FLIGHTMANAGER_H
