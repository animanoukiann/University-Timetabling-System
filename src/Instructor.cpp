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

std::string Instructor::convertToJson() {
    json j;

    j["name"] = name;
    json timeSlotJson = json::array();
    json courses = json::array();
    for (auto& slot : availability) {
        timeSlotJson.push_back(json::parse(slot.convertToJson()));
    }
    for (auto& course : preferredCourses) {
        courses.push_back(json::parse(course.convertToJson()));
    }
    j["availability"] = timeSlotJson;
    j["preferredCourses"] = courses;
    return j.dump(4);
}

Instructor Instructor::reverseFromJson(const nlohmann::json &j) {
    
    std::string name = j["instructor_name"];
    std::vector<Course> preferredCourses;
    std::string day = j["inst_day"];
    std::string start_time = j["inst_start_time"];
    std::string end_time = j["inst_end_time"];
    std::vector<TimeSlot> preferredTimeSlots;
    preferredTimeSlots.push_back(TimeSlot(day, start_time, end_time));
    std::string course_name = j["inst_course_name"];
    preferredCourses.push_back(Course(course_name));
    return Instructor(name, preferredTimeSlots, preferredCourses);
}
