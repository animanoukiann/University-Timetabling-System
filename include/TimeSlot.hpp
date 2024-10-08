#ifndef TIMESLOT_HPP
#define TIMESLOT_HPP

#include <string>
#include <iostream> 
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class TimeSlot { 
private:
    std::string day;
    std::string startTime;
    std::string endTime;
public:
    TimeSlot(std::string day, std::string startTime, std::string endTime);
    void displayInfo() const;
    std::string getDay() const;
    std::string getStartTime() const;
    std::string getEndTime() const;
    bool operator==(const TimeSlot& other) const;
    static TimeSlot reverseFromJson(const nlohmann::json &jsonString);
};

#endif
