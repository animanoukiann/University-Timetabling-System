#ifndef INSTRUCTOR_HPP
# define INSTRUCTOR_HPP

#include "Course.hpp"

class Instructor
{
    private:
        std::string name;
        std::vector<TimeSlot> availability;
        std::vector<Course> preferredCourses;
    public:
        Instructor(std::string name);
        Instructor(std::string name, std::vector<TimeSlot> availability,
        std::vector<Course> preferredCourses);
        void displayInfo() const;
        void setAvailability(TimeSlot &timeSlot);
        void setPreferredCourses(Course &course);
        std::string convert_to_json();
        static Instructor reverse_from_json(std::string &json);
};

#endif
