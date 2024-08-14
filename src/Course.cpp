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

std::string Course::convertToJson() {
    json j;

    j["courseName"] = courseName;
    json timeSlotJson = json::array();
    for (auto& slot : preferredTimeSlots) {
        timeSlotJson.push_back(json::parse(slot.convertToJson()));
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

Course Course::reverseFromJson(const nlohmann::json &j) {
    // json j = json::parse(courseJson);
    
    std::string courseName = j["course_name"];
    std::string day = j["course_day"];
    std::string start_time = j["course_start_time"];
    std::string end_time = j["course_end_time"];
    std::vector<TimeSlot> preferredTimeSlots;
    preferredTimeSlots.push_back(TimeSlot(day, start_time, end_time));
    return Course(courseName, preferredTimeSlots);
}
