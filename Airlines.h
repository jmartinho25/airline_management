
#ifndef PROJECT_2_AIRLINES_H
#define PROJECT_2_AIRLINES_H


#include <string>

class Airline {
private:
    std::string code;
    std::string name;
    std::string callsign;
    std::string country;

public:
    Airline(const std::string& code, const std::string& name, const std::string& callsign, const std::string& country)
            : code(code), name(name), callsign(callsign), country(country) {}

    std::string getCode() const { return code; }
    std::string getName() const { return name; }
    std::string getCallsign() const { return callsign; }
    std::string getCountry() const { return country; }
    bool operator==(const Airline& other) const {
        return code == other.code;
    }
};


#endif //PROJECT_2_AIRLINES_H
