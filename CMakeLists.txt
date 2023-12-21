cmake_minimum_required(VERSION 3.26)
project(Project_2)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(Project_2
        dataset/airlines.csv
        dataset/airports.csv
        dataset/flights.csv
        Graph.h
        Airport.h
        Airport.cpp
        AirportCollection.cpp
        AirportCollection.h
        Flight.cpp
        Parsing.cpp)
