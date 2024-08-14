#ifndef INSTRUCTOR_HPP
#define INSTRUCTOR_HPP

#include "Course.hpp" 

class Instructor {
private:
    std::string name;
    std::vector<TimeSlot> availability;
    std::vector<Course> preferredCourses;
public:
    Instructor(std::string name);
    Instructor(std::string name, std::vector<TimeSlot> availability,
    std::vector<Course> preferredCourses);
    std::string getName() const;
    std::vector<TimeSlot> getAvailability() const;
    std::vector<Course> getPreferredCourses() const;
    void displayInfo() const;
    void setAvailability(TimeSlot &timeSlot);
    void setPreferredCourses(Course &course);
    std::string convertToJson();
    static Instructor reverseFromJson(const nlohmann::json &jsonString);
    // static Instructor reverseFromJson(std::string &jsonString);
};

#endif
