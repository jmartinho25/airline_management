#include "AirportsGraph.h"
#include "Parsing.h"
#include "FlightManager.h"
#include "Menu.h"

int main() {
//    AirportsGraph airportsGraph;
//    Parsing parsing;
//    // Call the parsing function and pass the AirportsGraph instance
//    parsing.parseAirports("../dataset/airports.csv", airportsGraph);
//    parsing.parseAirlines("../dataset/airlines.csv", airportsGraph);
//    parsing.parseFlights("../dataset/flights.csv", airportsGraph);
    FlightManager flightManager;
    flightManager.loadAirports("../dataset/airports.csv");
    flightManager.loadAirlines("../dataset/airlines.csv");
    flightManager.loadFlights("../dataset/flights.csv");
    Menu menu(flightManager);
    menu.displayMenu();


    return 0;
}
