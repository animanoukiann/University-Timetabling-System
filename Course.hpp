#ifndef COURSE_HPP
# define COURSE_HPP

#include <vector>
#include "TimeSlot.hpp"

class Course
{
    private:
        std::string courseName;
        std::vector<TimeSlot> preferredTimeSlots;
    public:
        Course(std::string courseName);
        Course(std::string courseName, std::vector<TimeSlot> preferredTimeSlots);
        void displayInfo() const;
        void setPreferredTimeSlot(TimeSlot &timeSlot);
        std::string convert_to_json();
        static Course reverse_from_json(std::string &json);
};

#endif
