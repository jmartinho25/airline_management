#include "AirportsGraph.h"
#include "Parsing.h"

int main() {
//    ScheduleManager scheduleManager;
//    scheduleManager.loadClasses("../data/classes.csv");
//    scheduleManager.loadUCs("../data/classes_per_uc.csv");
//    scheduleManager.loadStudents("../data/students_classes.csv");
//    scheduleManager.loadStudentClasses("../data/students_classes.csv");
//    Menu menu(scheduleManager);
//    menu.displayMenu();
    AirportsGraph airportsGraph;
    Parsing parsing;
    // Call the parsing function and pass the AirportsGraph instance
    parsing.parseAirports("../dataset/airports.csv", airportsGraph);
    parsing.parseAirlines("../dataset/airlines.csv", airportsGraph);
    parsing.parseFlights("../dataset/flights.csv", airportsGraph);

    return 0;
}
