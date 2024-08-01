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

std::string Instructor::convert_to_json() {
    json j;

    j["name"] = name;
    json timeSlotJson = json::array();
    json courses = json::array();
    for (auto& slot : availability) {
        timeSlotJson.push_back(json::parse(slot.convert_to_json()));
    }
    for (auto& course : preferredCourses) {
        courses.push_back(json::parse(course.convert_to_json()));
    }
    j["availability"] = timeSlotJson;
    j["preferredCourses"] = courses;
    return j.dump(4);
}

Instructor Instructor::reverse_from_json(std::string &jsonString) {
    json j = json::parse(jsonString);
    
    std::string name = j["name"];
    std::vector<TimeSlot> availability;
    std::vector<Course> preferredCourses;
    for (auto& tsJson : j["availability"]) {
        std::string tsJsonString = tsJson.dump();
        availability.push_back(TimeSlot::reverse_from_json(tsJsonString));
    }
    for (auto& course : j["preferredCourses"]) {
        std::string courseString = course.dump();
        std::cout << courseString;
        preferredCourses.push_back(Course::reverse_from_json(courseString));
    }
    
    return Instructor(name, availability, preferredCourses);
}
