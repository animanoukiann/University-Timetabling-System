#include "Instructor.hpp"

Instructor::Instructor(std::string name): name(name) {}

Instructor::Instructor(std::string name, std::vector<TimeSlot> availability,
std::vector<Course> preferredCourses): name(name), availability(availability),
preferredCourses(preferredCourses) {} 

void Instructor::displayInfo() const {
    std::cout << "Instructor's name: '" << name << "'\nHis/Her availability" << std::endl;
    for (const auto& i : availability) {
        i.displayInfo();
    }
    std::cout << "His/Her preferred courses" << std::endl;
    for (const auto& i : preferredCourses) {
        i.displayInfo();
    }
}

std::string Instructor::getName() const {
    return name;
}

std::vector<TimeSlot> Instructor::getAvailability() const {
    return availability;
}

std::vector<Course> Instructor::getPreferredCourses() const {
    return preferredCourses;
}

void Instructor::setAvailability(TimeSlot &timeSlot) {
    availability.push_back(timeSlot);
}

void Instructor::setPreferredCourses(Course &course) {
    preferredCourses.push_back(course);
}

Instructor Instructor::reverseFromJson(const nlohmann::json &jsonName) {
    std::string name = "instructor_name";
    std::string day = "inst_day";
    std::string start_time = "inst_start_time";
    std::string end_time = "inst_end_time";
    std::string course_name = "inst_course_name";
    std::vector<Course> preferredCourses;
    std::vector<TimeSlot> preferredTimeSlots;
    preferredTimeSlots.push_back(TimeSlot(jsonName[day], jsonName[start_time], jsonName[end_time]));
    preferredCourses.push_back(Course(jsonName[course_name]));
    return Instructor(jsonName[name], preferredTimeSlots, preferredCourses);
}
