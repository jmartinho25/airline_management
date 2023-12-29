//
// Created by darksystem on 25-12-2023.
//

#include "Menu.h"
#include <iostream>

void Menu::displayMenu() {
    int choice;
    do {

        std::cout << "1. Consult information" << std::endl;
        std::cout << "2. Present the best flight option" << std::endl;
        std::cout << "3. Process All Requests" << std::endl;
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
                std::cout << "9. Identify the airports that are essential to the network´s circulation capability"<<std::endl;
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
//            case 3:
////                processrequests();
//                break;
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
