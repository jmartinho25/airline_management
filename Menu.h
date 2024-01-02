//
// Created by darksystem on 25-12-2023.
//

#ifndef PROJECT_2_MENU_H
#define PROJECT_2_MENU_H


#include "FlightManager.h"

/**
    * @brief Class that includes all the Menus that are displayed in our program
    */
class Menu {
public:
    /**
    * @brief  Constructor of the Menu Class
    * @param system FlightManager object used in the program
    */
    Menu(FlightManager &system) : managementSystem(system) {};

    void displayMenu();

private:
    FlightManager &managementSystem;

    void listnrairportsflights();

    void listflightsfromairport();

    void listflightspercityperairline();

    void listdiffcountriesperairpercity();

    void listnrdestavailable();

    void listReachableDestinationsWithStops();

    void listmaxstopsbetweenairports();

    void identifytopkairport();

    void findEssentialAirports();

    //FIND BEST PATH FUNCTIONS

    void bfoairporttoairport();

    void bfoairporttocity();

    void bfocitytoairport();

    void bfoairporttocoordinates();

    void bfocitytocitity();

    void bfocitytocoordinates();

    void bfoCoordinatestoAirport();

    void bfoCoordinatestoCity();

    void bfoCoordinatestoCoordinates();

    //FILTERS

    void handleBestFlightOptionWithFilters();

    void handlePreferredAirlineFilter();

    void handleMaxStopsFilter();

    //PREFERRED AIRLINE

    void bfoairporttoairportPAirline(std::string &airline);

    void bfoairporttocityPAirline(std::string &airlineCode);

    void bfoairporttocoordinatesPAirline(std::string &airlineCode);

    void bfocitytoairportPAirline(std::string &airlineCode);

    void bfocitytocityPAirline(std::string &airlineCode);

    void bfocitytocoordinatesPAirline(std::string &airlineCode);

    void bfoCoordinatestoAirportPAirline(std::string &airlineCode);

    void bfoCoordinatestoCityPAirline(std::string &airlineCode);

    void bfoCoordinatestoCoordinatesPAirline(std::string &airlineCode);

    //MAX STOPS

    void bfoAirportToAirportMaxStops();

    void bfoAirportToCityMaxStops();

    void bfoAirportToCoordinatesMaxStops();

    void bfoCityToAirportMaxStops();

    void bfoCityToCityMaxStops();

    void bfoCityToCoordinatesMaxStops();

    void bfoCoordinatesToAirportMaxStops();

    void bfoCoordinatesToCityMaxStops();

    void bfoCoordinatesToCoordinatesMaxStops();
};


#endif //PROJECT_2_MENU_H
