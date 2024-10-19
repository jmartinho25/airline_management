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

    std::vector<Vertex<Airport> *> findNearestAirportToCoordinates(double lat, double lon);

    vector<Vertex<Airport> *> findAirportsInCity(const std::string &cityName);

    Vertex<Airport> *findAirportVertexByNameOrCode(const std::string &airportIdentifier);


    Vertex<Airport> *findAirportVertexByName(string airportName);

    std::vector<std::vector<Vertex<Airport> *>>
    shortestPathsPAirline(Vertex<Airport> *startAirport, Vertex<Airport> *endAirport, const std::string &airlineCode);

    const int MAX_ITERATIONS = 20000;
public:
    FlightManager();

    void loadAirports(const std::string &filename);

    void loadAirlines(const std::string &filename);

    void loadFlights(const std::string &filename);


    void listnrairportsflights();

    void listflightsfromairport();

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

    void bfoairporttoairport(const string &airport1, const string &airport2);

    void bfoairporttocity(const string &airport, const string &cityName);

    void bfocitytoairport(const string &cityname, const string &airport);

    void bfoairporttocoordinates(const string &airportCode, double lat, double lon);

    void bfocitytocity(const string &sourceCity, const string &destCity);

    void bfocitytocoordinates(const string &cityName, double lat, double lon);

    void bfoCoordinatestoAirport(double lat, double lon, const string &airportCode);

    void bfoCoordinatestoCity(double lat, double lon, const std::string &cityName);

    void bfoCoordinatestoCoordinates(double sourceLat, double sourceLon, double destLat, double destLon);

    std::vector<std::vector<Vertex<Airport> *>>
    shortestPaths(Vertex<Airport> *startAirport, Vertex<Airport> *endAirport);

    //FILTERS
    //PREFERRED AIRLINE
    void
    bfoairporttoairportPAirline(const string &airportCode1, const string &airportCode2, const std::string &airlineCode);

    void bfoairporttocityPAirline(const string &airport, const string &cityName, const std::string &airlineCode);

    void
    bfoairporttocoordinatesPAirline(const string &airportCode, double lat, double lon, const std::string &airlineCode);

    void bfocitytoairportPAirline(const string &cityname, const string &airport, const std::string &airlineCode);

    void bfocitytocityPAirline(const string &sourceCity, const string &destCity, const std::string &airlineCode);

    void bfocitytocoordenatesPAirline(const string &sourceCity, double lat, double lon, const std::string &airlineCode);

    void
    bfoCoordinatestoAirportPAirline(double lat, double lon, const string &airportCode, const std::string &airlineCode);

    void
    bfoCoordinatestoCityPAirline(double lat, double lon, const std::string &cityName, const std::string &airlineCode);

    void bfoCoordinatestoCoordinatesPAirline(double sourceLat, double sourceLon, double destLat, double destLon,
                                             const std::string &airlineCode);

    void dfs_artic(Vertex<Airport> *v, unordered_set<string> &essentialAirports, unsigned int index) const;

    //MAX STOPS

    void bfoAirportToAirportMaxStops(const std::string &sourceCode, const std::string &targetCode, int maxStops);

    std::vector<std::vector<Vertex<Airport> *>>
    shortestPathsWithMaxStops(Vertex<Airport> *startAirport, Vertex<Airport> *endAirport, int maxStops);

    void bfoAirportToCityMaxStops(const std::string &sourceCode, const std::string &targetCity, int maxStops);

    std::vector<std::vector<Vertex<Airport> *>>
    shortestPathsToCityWithMaxStops(Vertex<Airport> *startAirport, const std::string &cityName, int maxStops);

    void bfoAirportToCoordinatesMaxStops(const std::string &sourceCode, double lat, double lon, int maxStops);

    void bfoCityToAirportMaxStops(const std::string &sourceCity, const std::string &targetCode, int maxStops);

    void bfoCityToCityMaxStops(const std::string &sourceCity, const std::string &targetCity, int maxStops);

    void bfoCityToCoordinatesMaxStops(const std::string &cityName, double lat, double lon, int maxStops);

    void bfoCoordinatesToAirportMaxStops(double lat, double lon, const std::string &targetAirportCode, int maxStops);

    void bfoCoordinatesToCityMaxStops(double lat, double lon, const std::string &targetCity, int maxStops);

    void bfoCoordinatesToCoordinatesMaxStops(double sourceLat, double sourceLon, double destLat, double destLon,
                                             int maxStops);

    //MAX DISTANCE

    void bfoAirportToAirportMaxDistance(const std::string &sourceCode, const std::string &targetCode, int maxDistance);

    std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> shortestPathsWithinDistance(
            Vertex<Airport> *startAirport, Vertex<Airport> *endAirport, int maxDistance);

    void bfoAirportToCityMaxDistance(const std::string &sourceCode, const std::string &targetCity, int maxDistance);

    std::vector<std::pair<std::vector<Vertex<Airport> *>, int>> shortestPathsToCityWithinDistance(
            Vertex<Airport> *startAirport, const std::string &cityName, int maxDistance);

    void bfoAirportToCoordinatesMaxDistance(const std::string &sourceCode, double lat, double lon, int maxDistance);

    void bfoCityToAirportMaxDistance(const std::string &cityName, const std::string &airportCode, int maxDistance);

    void bfoCityToCityMaxDistance(const std::string &sourceCity, const std::string &destCity, int maxDistance);

    void bfoCityToCoordinatesMaxDistance(const std::string &cityName, double lat, double lon, int maxDistance);

    void bfoCoordinatesToAirportMaxDistance(double sourceLat, double sourceLon, const std::string &targetAirportCode,
                                            int maxDistance);

    void
    bfoCoordinatesToCityMaxDistance(double sourceLat, double sourceLon, const std::string &targetCity, int maxDistance);

    void bfoCoordinatesToCoordinatesMaxDistance(double sourceLat, double sourceLon, double destLat, double destLon,
                                                int maxDistance);

};


#endif //PROJECT_2_FLIGHTMANAGER_H
