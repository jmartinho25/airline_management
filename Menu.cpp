//
// Created by darksystem on 25-12-2023.
//

#include "Menu.h"
#include <iostream>
void Menu::displayMenu() {
    int choice;
    do {

        std::cout << "1. Consult information" << std::endl;
        std::cout << "2. Management options" << std::endl;
        std::cout << "3. Process All Requests" << std::endl;
        std::cout << "0. Exit" << std::endl;

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "1. View global number of airports and number of available flights" << std::endl;
                std::cout << "2. View number of flights out of an airport and from how many airlines" << std::endl;
                std::cout << "3. View number of flights per city per airline" << std::endl;
                std::cout << "4. View number of different countries that a given airport per city flies to" << std::endl;
                std::cout << "5. View Schedule of a Class" << std::endl;
                std::cout << "6. View Students in Class" << std::endl;
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
                        cout << "VIEW CLASS SCHEDULE:" << endl;
//                        listClassSchedule();
                        break;

                    case 6:
//                        listStudentsInClass();
                        break;


                    case 0:
                        std::cout << "Exiting the program." << std::endl;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again." << std::endl;
                }


                break;
//            case 2:
//                std::cout << "1. Join new UC" << std::endl;
//                std::cout << "2. Request change of class" << std::endl;
//                std::cout << "0. Exit" << std::endl;
//                std::cout << "Enter your choice: ";
//                std::cin >> choice;
//                switch (choice) {
//                    case 1:
//                        joinNewUC();
//                        break;
//
//                    case 2:
//                        changeClassReq();
//                        break;
//                    case 0:
//                        std::cout << "Exiting the program." << std::endl;
//                        break;
//                    default:
//                        std::cout << "Invalid choice. Please try again." << std::endl;
//                }
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
