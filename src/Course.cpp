#include "Course.hpp"

Course::Course(std::string courseName) : courseName(courseName) {} 

Course::Course(std::string courseName, std::vector<TimeSlot> preferredTimeSlots) : courseName(courseName), 
preferredTimeSlots (preferredTimeSlots) {}

void Course::displayInfo() const {
    std::cout << "The course: '" << courseName << "'\nIts preferred time slots" << std::endl;
    for (const auto& i : preferredTimeSlots) {
        i.displayInfo();
    }
}

void Course::setPreferredTimeSlot(TimeSlot &timeSlot) {
    preferredTimeSlots.push_back(timeSlot);
}

std::string Course::convert_to_json() {
    json j;

    j["courseName"] = courseName;
    json timeSlotJson = json::array();
    for (auto& slot : preferredTimeSlots) {
        timeSlotJson.push_back(json::parse(slot.convert_to_json()));
    }
    j["preferredTimeSlots"] = timeSlotJson;
    return j.dump(4);
}

std::string Course::getCourseName() const {
    return courseName;
}

std::vector<TimeSlot> Course::getPreferredTimeSlots() const {
    return preferredTimeSlots;
}

bool Course::operator==(const Course& other) const {
    return courseName == other.courseName && preferredTimeSlots == other.preferredTimeSlots;
}

Course Course::reverse_from_json(std::string &jsonString) {
    json j = json::parse(jsonString);
    
    std::string courseName = j["courseName"];
    std::vector<TimeSlot> preferredTimeSlots;
    for (auto& tsJson : j["preferredTimeSlots"]) {
        std::string tsJsonString = tsJson.dump();
        preferredTimeSlots.push_back(TimeSlot::reverse_from_json(tsJsonString));
    }
    
    return Course(courseName, preferredTimeSlots);
}
