#include "AirportsGraph.h"
#include "Parsing.h"
#include "FlightManager.h"
#include "Menu.h"

int main() {
    FlightManager flightManager;
    flightManager.loadAirports("../dataset/airports.csv");
    flightManager.loadAirlines("../dataset/airlines.csv");
    flightManager.loadFlights("../dataset/flights.csv");
    Menu menu(flightManager);
    menu.displayMenu();


    return 0;
}
