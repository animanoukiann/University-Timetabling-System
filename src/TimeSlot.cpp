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

TimeSlot TimeSlot::reverseFromJson(const nlohmann::json &jsonName) {
    std::string day = jsonName["day"];
    std::string startTime = jsonName["start_time"];
    std::string endTime = jsonName["end_time"]; 
    return TimeSlot(day, startTime, endTime);
}