#include "TimeSlot.hpp"

TimeSlot::TimeSlot(std::string day, std::string startTime, std::string endTime)
: day(day), startTime(startTime), endTime(endTime) {}

std::string TimeSlot::getDay() const {
    return day;
} 

std::string TimeSlot::getStartTime() const {
    return startTime;
}

std::string TimeSlot::getEndTime() const {
    return endTime;
}

bool TimeSlot::operator==(const TimeSlot& other) const {
    return day == other.day && startTime == other.startTime && endTime == other.endTime;
}

void TimeSlot::displayInfo() const {
    std::cout << "TimeSlot: " << day << " from " << 
    startTime << " to " << endTime << std::endl;
}

std::string TimeSlot::convertToJson() { 
    json j;

    j["day"] = day;
    j["startTime"] = startTime;
    j["endTime"] = endTime;

    return j.dump(4);
}

TimeSlot TimeSlot::reverseFromJson(std::string &jsonString) {
    json j = json::parse(jsonString);
    std::string day = j["day"];
    std::string startTime = j["startTime"];
    std::string endTime = j["endTime"]; 

    return TimeSlot(day, startTime, endTime);
}