//
// Created by darksystem on 25-12-2023.
//

#ifndef PROJECT_2_MENU_H
#define PROJECT_2_MENU_H


#include "FlightManager.h"

class Menu {
public:
    Menu(FlightManager &system) : managementSystem(system) {};

    void displayMenu();

private:
    FlightManager &managementSystem;

    void listnrairportsflights();

    void listflightsfromairport();

    void listflightsfromcity();

    void listflightspercityperairline();

    void listdiffcountriesperairpercity();

    void listnrdestavailable();

    void listReachableDestinationsWithStops();

    void listmaxstopsbetweenairports();

    void identifytopkairport();

    void findEssentialAirports();

    void bfoairporttoairport();

    void bfoairporttocity();

    void bfocitytoairport();

    void bfoairporttocoordinates();

    void bfocitytocitity();

    void bfocitytocoordinates();

    void bfoCoordinatestoAirport();
};


#endif //PROJECT_2_MENU_H
