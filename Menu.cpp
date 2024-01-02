//
// Created by darksystem on 25-12-2023.
//

#include "Menu.h"
#include <iostream>
/**
    * @brief Main Menu of this program, together with all the calls and conditions that will allow the implementation of all the program´s functionalities
    */
void Menu::displayMenu() {
    int choice;
    int filterChoice;
    do {

        std::cout << "1. Consult information" << std::endl;
        std::cout << "2. Present the best flight option" << std::endl;
        std::cout << "3. Present the best flight option with filters" << std::endl;
        std::cout << "0. Exit" << std::endl;

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "1. View global number of airports and number of available flights" << std::endl;
                std::cout << "2. View number of flights out of an airport and from how many airlines" << std::endl;
                std::cout << "3. View number of flights per city per airline" << std::endl;
                std::cout << "4. View number of different countries that a given airport per city flies to"
                          << std::endl;
                std::cout << "5. View number of destinations available for a given airport" << std::endl;
                std::cout << "6. View number of reachable destinations from a airport given a max nº of stops"
                          << std::endl;
                std::cout << "7. Display the flight trips between airports with the greatest stops in between them"
                          << std::endl;
                std::cout << "8. List the top K airports with the greatest air traffic capacity" << std::endl;
                std::cout << "9. Identify the airports that are essential to the network´s circulation capability"
                          << std::endl;
                std::cout << "0. Exit" << std::endl;

                std::cout << "Enter your choice: ";
                std::cin >> choice;
                switch (choice) {
                    case 1:
                        listnrairportsflights();
                        break;
                    case 2:
                        listflightsfromairport();
                        break;
                    case 3:
                        listflightspercityperairline();
                        break;

                    case 4:
                        listdiffcountriesperairpercity();

                        break;
                    case 5:
                        listnrdestavailable();
                        break;

                    case 6:
                        listReachableDestinationsWithStops();
                        break;
                    case 7:
                        listmaxstopsbetweenairports();
                        break;
                    case 8:
                        identifytopkairport();
                        break;

                    case 9:
                        findEssentialAirports();
                        break;


                    case 0:
                        std::cout << "Exiting the program." << std::endl;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }


                break;
            case 2:
                std::cout << "1. From airport to airport" << std::endl;
                std::cout << "2. From airport to city" << std::endl;
                std::cout << "3. From airport to geographic coordinates" << std::endl;
                std::cout << "4. From city to airport" << std::endl;
                std::cout << "5. From city to city" << std::endl;
                std::cout << "6. From city to geographic coordinates" << std::endl;
                std::cout << "7. From geographic coordinates to airport" << std::endl;
                std::cout << "8. From geographic coordinates to city" << std::endl;
                std::cout << "9. From geographic coordinates to geographic coordinates" << std::endl;
                std::cout << "0. Exit" << std::endl;
                std::cout << "Enter the criteria you want: ";
                std::cin >> choice;
                switch (choice) {
                    case 1:
                        bfoairporttoairport();
                        break;

                    case 2:
                        bfoairporttocity();
                        break;
                    case 3:
                        bfoairporttocoordinates();
                        break;

                    case 4:
                        bfocitytoairport();
                        break;
                    case 5:
                        bfocitytocitity();
                        break;

                    case 6:
                        bfocitytocoordinates();
                        break;
                    case 7:
                        bfoCoordinatestoAirport();
                        break;

                    case 8:
                        bfoCoordinatestoCity();
                        break;
                    case 9:
                        bfoCoordinatestoCoordinates();
                        break;
                    case 0:
                        std::cout << "Exiting the program." << std::endl;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }
                break;
            case 3:
                handleBestFlightOptionWithFilters();
                break;
        }
    } while (choice != 0);
}

void Menu::listnrairportsflights() {
    managementSystem.listnrairportsflights();

}

void Menu::listflightsfromairport() {
    managementSystem.listflightsfromairport();

}


void Menu::listflightspercityperairline() {
    managementSystem.listflightspercityperairline();

}

void Menu::listdiffcountriesperairpercity() {
    managementSystem.listdiffcountriesperairpercity();

}

void Menu::listnrdestavailable() {
    managementSystem.listnrdestavailable();

}

void Menu::listReachableDestinationsWithStops() {
    std::string airportCode;
    int maxStops, criteria;

    std::cout << "Enter the airport code: ";
    std::cin >> airportCode;

    std::cout << "Enter the maximum number of stops: ";
    std::cin >> maxStops;

    std::cout << "Choose the criteria for destinations: " << std::endl;
    std::cout << "1. Airports" << std::endl;
    std::cout << "2. Cities" << std::endl;
    std::cout << "3. Countries" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> criteria;

    managementSystem.listReachableDestinations(airportCode, maxStops, criteria);
}

void Menu::listmaxstopsbetweenairports() {
    managementSystem.listmaxstopsbetweenairports();

}

void Menu::identifytopkairport() {
    managementSystem.identifytopkairport();

}

void Menu::findEssentialAirports(){
    managementSystem.findEssentialAirports();
}

void Menu::bfoairporttoairport() {

    std::string airportCode1;
    std::cout << "Enter the first airport code or name: ";
    std::cin.ignore();
    std::getline(std::cin, airportCode1);
    std::string airportCode2;
    std::cout << "Enter the second airport code or name: ";
//    std::cin.ignore();
    std::getline(std::cin, airportCode2);



    managementSystem.bfoairporttoairport(airportCode1, airportCode2);
}

void Menu::bfoairporttocity() {
    std::string airport;
    std::cout << "Enter the airport code or name: ";
    std::cin.ignore();
    std::getline(std::cin, airport);
    std::string  cityName;
    std::cout << "Enter the city name: ";
//    std::cin.ignore();
    std::getline(std::cin, cityName);




    managementSystem.bfoairporttocity(airport, cityName);
}


void Menu::bfoairporttocoordinates() {
    std::string airportCode;
    double lat, lon;

    std::cout << "Enter the airport code: ";
    std::cin >> airportCode;

    std::cout << "Enter the latitude: ";
    std::cin >> lat;

    std::cout << "Enter the longitude: ";
    std::cin >> lon;

    managementSystem.bfoairporttocoordinates(airportCode, lat, lon);
}

void Menu::bfocitytoairport() {
    std::string cityName, airportCode;
    std::cout << "Enter the city name: ";
    std::cin.ignore();
    std::getline(std::cin, cityName);
    std::cout << "Enter the airport code: ";
    std::getline(std::cin, airportCode);

    managementSystem.bfocitytoairport(cityName, airportCode);
}

void Menu::bfocitytocitity() {
    std::string sourceCity, destCity;
    std::cout << "Enter the source city name: ";
    std::cin.ignore();
    std::getline(std::cin, sourceCity);
    std::cout << "Enter the destination city name: ";
    std::getline(std::cin, destCity);

    managementSystem.bfocitytocity(sourceCity, destCity);
}

void Menu::bfocitytocoordinates() {
    std::string cityName;
    double lat, lon;

    std::cout << "Enter the city name: ";
    std::cin.ignore();
    std::getline(std::cin, cityName);

    std::cout << "Enter the latitude: ";
    std::cin >> lat;

    std::cout << "Enter the longitude: ";
    std::cin >> lon;

    managementSystem.bfocitytocoordinates(cityName, lat, lon);
}

void Menu::bfoCoordinatestoAirport() {
    double lat, lon;
    std::string airportCode;

    std::cout << "Enter the latitude: ";
    std::cin >> lat;

    std::cout << "Enter the longitude: ";
    std::cin >> lon;

    std::cout << "Enter the airport code or name: ";
    std::cin.ignore();  // Clear the input buffer
    std::getline(std::cin, airportCode);

    managementSystem.bfoCoordinatestoAirport(lat, lon, airportCode);
}

void Menu::bfoCoordinatestoCity() {
    double lat, lon;
    std::string cityName;

    std::cout << "Enter the latitude: ";
    std::cin >> lat;

    std::cout << "Enter the longitude: ";
    std::cin >> lon;

    std::cin.ignore(); // Clear the input buffer
    std::cout << "Enter the city name: ";
    std::getline(std::cin, cityName);

    managementSystem.bfoCoordinatestoCity(lat, lon, cityName);
}


void Menu::bfoCoordinatestoCoordinates() {
    double sourceLat, sourceLon, destLat, destLon;

    std::cout << "Enter source latitude: ";
    std::cin >> sourceLat;
    std::cout << "Enter source longitude: ";
    std::cin >> sourceLon;
    std::cout << "Enter destination latitude: ";
    std::cin >> destLat;
    std::cout << "Enter destination longitude: ";
    std::cin >> destLon;

    managementSystem.bfoCoordinatestoCoordinates(sourceLat, sourceLon, destLat, destLon);
}

//FILTERS

void Menu::handleBestFlightOptionWithFilters() {
    int filterChoice;
    std::cout << "Choose a filter:" << std::endl;
    std::cout << "1. Preferred airline" << std::endl;
    std::cout << "2. Maximum number of airlines" << std::endl;
    std::cout << "3. Preferred stop location" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> filterChoice;

    switch (filterChoice) {
        case 1:
            handlePreferredAirlineFilter();
            break;
        case 2:
            //handleMaxNumberOfAirlinesFilter();
            break;
        case 3:
            //handlePreferredStopLocationFilter();
            break;
        default:
            std::cout << "Invalid filter choice. Please try again." << std::endl;
    }
}

//PREFERRED AIRLINE
void Menu::handlePreferredAirlineFilter() {
    std::string preferredAirlineCode;
    int choice;
    std::cout << "Enter the code of the preferred airline: ";
    std::cin >> preferredAirlineCode;

    std::cout << "1. From airport to airport" << std::endl;
    std::cout << "2. From airport to city" << std::endl;
    std::cout << "3. From airport to geographic coordinates" << std::endl;
    std::cout << "4. From city to airport" << std::endl;
    std::cout << "5. From city to city" << std::endl;
    std::cout << "6. From city to geographic coordinates" << std::endl;
    std::cout << "7. From geographic coordinates to airport" << std::endl;
    std::cout << "8. From geographic coordinates to city" << std::endl;
    std::cout << "9. From geographic coordinates to geographic coordinates" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter the criteria you want: ";
    std::cin >> choice;
    switch (choice) {
        case 1:
            bfoairporttoairportPAirline(preferredAirlineCode);
            break;

        case 2:
            bfoairporttocityPAirline(preferredAirlineCode);
            break;
        case 3:
            bfoairporttocoordinatesPAirline(preferredAirlineCode);
            break;

        case 4:
            bfocitytoairportPAirline(preferredAirlineCode);
            break;
        case 5:
            bfocitytocityPAirline(preferredAirlineCode);
            break;

        case 6:
            bfocitytocoordinatesPAirline(preferredAirlineCode);
            break;
        case 7:
            bfoCoordinatestoAirportPAirline(preferredAirlineCode);
            break;

        case 8:
            bfoCoordinatestoCityPAirline(preferredAirlineCode);
            break;
        case 9:
            bfoCoordinatestoCoordinatesPAirline(preferredAirlineCode);
            break;
        case 0:
            std::cout << "Exiting the program." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
    }

}

void Menu::bfoairporttoairportPAirline(std::string& airline) {

    std::string airportCode1;
    std::cout << "Enter the first airport code or name: ";
    std::cin.ignore();
    std::getline(std::cin, airportCode1);
    std::string airportCode2;
    std::cout << "Enter the second airport code or name: ";
//    std::cin.ignore();
    std::getline(std::cin, airportCode2);



    managementSystem.bfoairporttoairportPAirline(airportCode1, airportCode2, airline);
}

void Menu::bfoairporttocityPAirline(std::string& airlineCode) {
    std::string airport;
    std::cout << "Enter the airport code or name: ";
    std::cin.ignore();
    std::getline(std::cin, airport);
    std::string  cityName;
    std::cout << "Enter the city name: ";
//    std::cin.ignore();
    std::getline(std::cin, cityName);




    managementSystem.bfoairporttocityPAirline(airport, cityName, airlineCode);
}

void Menu::bfoairporttocoordinatesPAirline(std::string &airlineCode) {
    std::string airportCode;
    double lat, lon;

    std::cout << "Enter the airport code: ";
    std::cin >> airportCode;

    std::cout << "Enter the latitude: ";
    std::cin >> lat;

    std::cout << "Enter the longitude: ";
    std::cin >> lon;

    managementSystem.bfoairporttocoordinatesPAirline(airportCode, lat, lon, airlineCode);
}

void Menu::bfocitytoairportPAirline(std::string &airlineCode)  {
    std::string cityName, airportCode;
    std::cout << "Enter the city name: ";
    std::cin.ignore();
    std::getline(std::cin, cityName);
    std::cout << "Enter the airport code: ";
    std::getline(std::cin, airportCode);

    managementSystem.bfocitytoairportPAirline(cityName, airportCode, airlineCode);
}

void Menu::bfocitytocityPAirline(std::string &airlineCode) {
    std::string sourceCity, destCity;
    std::cout << "Enter the source city name: ";
    std::cin.ignore();
    std::getline(std::cin, sourceCity);
    std::cout << "Enter the destination city name: ";
    std::getline(std::cin, destCity);

    managementSystem.bfocitytocityPAirline(sourceCity, destCity, airlineCode);
}

void Menu::bfocitytocoordinatesPAirline(std::string &airlineCode) {
    std::string cityName;
    double lat, lon;

    std::cout << "Enter the city name: ";
    std::cin.ignore();
    std::getline(std::cin, cityName);

    std::cout << "Enter the latitude: ";
    std::cin >> lat;

    std::cout << "Enter the longitude: ";
    std::cin >> lon;

    managementSystem.bfocitytocoordenatesPAirline(cityName, lat, lon, airlineCode);
}

void Menu::bfoCoordinatestoAirportPAirline(std::string &airlineCode) {
    double lat, lon;
    std::string airportCode;

    std::cout << "Enter the latitude: ";
    std::cin >> lat;

    std::cout << "Enter the longitude: ";
    std::cin >> lon;

    std::cout << "Enter the airport code or name: ";
    std::cin.ignore();  // Clear the input buffer
    std::getline(std::cin, airportCode);

    managementSystem.bfoCoordinatestoAirportPAirline(lat, lon, airportCode, airlineCode);
}

void Menu::bfoCoordinatestoCityPAirline(std::string &airlineCode) {
    double lat, lon;
    std::string cityName;

    std::cout << "Enter the latitude: ";
    std::cin >> lat;

    std::cout << "Enter the longitude: ";
    std::cin >> lon;

    std::cin.ignore(); // Clear the input buffer
    std::cout << "Enter the city name: ";
    std::getline(std::cin, cityName);

    managementSystem.bfoCoordinatestoCityPAirline(lat, lon, cityName, airlineCode);
}

void Menu::bfoCoordinatestoCoordinatesPAirline(std::string &airlineCode) {
    double sourceLat, sourceLon, destLat, destLon;

    std::cout << "Enter source latitude: ";
    std::cin >> sourceLat;
    std::cout << "Enter source longitude: ";
    std::cin >> sourceLon;
    std::cout << "Enter destination latitude: ";
    std::cin >> destLat;
    std::cout << "Enter destination longitude: ";
    std::cin >> destLon;

    managementSystem.bfoCoordinatestoCoordinatesPAirline(sourceLat, sourceLon, destLat, destLon, airlineCode);
}