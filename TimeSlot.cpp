#include "TimeSlot.hpp"

TimeSlot::TimeSlot(std::string day, std::string startTime, std::string endTime)
: day(day), startTime(startTime), endTime(endTime) {}

void TimeSlot::displayInfo() const
{
    std::cout << "TimeSlot: " << day << " from " << 
    startTime << " to " << endTime << std::endl;
}

std::string TimeSlot::convert_to_json()
{
    return "{\n  \"day\": \"" + day + "\",\n  \"startTime\": \""
    + startTime + "\",\n  \"endTime\": \"" + endTime + "\"\n}";
}

TimeSlot TimeSlot::reverse_from_json(std::string &json)
{
    int day_index = json.find("day") + 7;
    int st_index = json.find("startTime") + 13;
    int et_index = json.find("endTime") + 11;

    std::string day = json.substr(day_index, json.find("\"", day_index) - day_index);
    std::string startTime = json.substr(st_index, json.find("\"", st_index) - st_index);
    std::string endTime = json.substr(et_index, json.find("\"", et_index) - et_index);

    return TimeSlot(day, startTime, endTime);
}