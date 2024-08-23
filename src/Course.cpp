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

std::string Course::getCourseName() const {
    return courseName;
}

std::vector<TimeSlot> Course::getPreferredTimeSlots() const {
    return preferredTimeSlots;
}

bool Course::operator==(const Course& other) const {
    return courseName == other.courseName && preferredTimeSlots == other.preferredTimeSlots;
}

Course Course::reverseFromJson(const nlohmann::json &jsonName) {
    std::string courseName = "course_name";
    std::string day = "course_day";
    std::string start_time = "course_start_time";
    std::string end_time = "course_end_time";
    std::vector<TimeSlot> preferredTimeSlots;
    preferredTimeSlots.push_back(TimeSlot(jsonName[day], jsonName[start_time], jsonName[end_time]));
    return Course(jsonName[courseName], preferredTimeSlots);
}
