#ifndef COURSE_HPP
#define COURSE_HPP

#include <vector>
#include "TimeSlot.hpp" 

class Course { 
private:
    std::string courseName;
    std::vector<TimeSlot> preferredTimeSlots;
public:
    Course(std::string courseName);
    Course(std::string courseName, std::vector<TimeSlot> preferredTimeSlots);
    bool operator==(const Course& other) const;
    void displayInfo() const;
    std::string getCourseName() const;
    std::vector<TimeSlot> getPreferredTimeSlots() const;
    void setPreferredTimeSlot(TimeSlot &timeSlot);
    static Course reverseFromJson(const nlohmann::json &courseJson);
};

#endif
