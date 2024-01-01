
#ifndef PROJECT_2_AIRLINES_H
#define PROJECT_2_AIRLINES_H


#include <string>
/**
    * @brief Class to store the information about a given Airline
    */
class Airline {
private:
    /** @brief Code of the airline*/
    std::string code;
    /** @brief Name of the airline*/
    std::string name;
    /** @brief Callsign of the airline*/
    std::string callsign;
    /** @brief Country of the airline*/
    std::string country;

public:
    /**
    * @brief Constructor of the Airline class. code, name, callsign and country correspond to the given values
    * @details Time complexity: O(1)
    * @param code Airline code
    * @param name Airline name
    * @param callsign Airline callsign
    * @param country Airline country
    */
    Airline(const std::string &code, const std::string &name, const std::string &callsign, const std::string &country)
            : code(code), name(name), callsign(callsign), country(country) {}

    /**
    * @brief Returns the code of the airline
    * @details Time complexity: O(1)
    * @return code of the airline
    */
    std::string getCode() const { return code; }

    /**
    * @brief Returns the name of the airline
    * @details Time complexity: O(1)
    * @return name of the airline
    */
    std::string getName() const { return name; }

    /**
    * @brief Returns the callsign of the airline
    * @details Time complexity: O(1)
    * @return callsign of the airline
    */
    std::string getCallsign() const { return callsign; }

    /**
    * @brief Returns the country of the airline
    * @details Time complexity: O(1)
    * @return country of the airline
    */
    std::string getCountry() const { return country; }

    /**
    * @brief Returns if two airlines are the same or not by comparing their airline code
    * @details Time complexity: O(1)
    * @return True if they have the same airline code (same airline) or false if they don´t have the same airline code (different airlines)
    */
    bool operator==(const Airline &other) const {
        return code == other.code;
    }
};


#endif //PROJECT_2_AIRLINES_H
