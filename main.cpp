#include "University.hpp"

int main() 
{
    TimeSlot ts1("Monday", "10:45", "12:20");
    TimeSlot ts2("Thursday", "14:35", "16:10");
    std::vector<TimeSlot> timeSlots { ts1, ts2 };

    Course course1("Introduction to Computer Science", timeSlots);
    Course course2("MathPhys Equations", {ts2});

    
    Instructor instructor("Dr. Alice Brown");
    instructor.setAvailability(ts1);
    instructor.setPreferredCourses(course1);
    

    Instructor instructor1("John Doe");
    instructor1.setAvailability(ts2);
    instructor1.setPreferredCourses(course2);

    University uni;
    uni.addCourse(course1);
    uni.addCourse(course2);
    uni.addInstructor(instructor1);
    uni.addInstructor(instructor);
    uni.addTimeSlot(ts1);
    uni.addTimeSlot(ts2);

    uni.saveState(uni, "/home/ani/Desktop/University Timetabling System/university.json");

    University loadedUni;
    loadedUni = loadedUni.loadState("university.json");

    std::cout << "Loaded Courses:" << std::endl;
    for (const auto &course : loadedUni.courses) {
        course.displayInfo();
    }

    std::cout << "Loaded Instructors:" << std::endl;
    for (const auto &instructor : loadedUni.instructors) {
        instructor.displayInfo();
    }

    std::vector<University::Gene> schedule = uni.schedule();
    std::cout << "Best Schedule:" << std::endl;
    for (const auto& gene : schedule) {
        std::cout << "Course: " << uni.courses[gene.courseIndex].courseName
                  << ", TimeSlot: " << uni.timeSlots[gene.timeSlotIndex].day << " "
                  << uni.timeSlots[gene.timeSlotIndex].startTime << "-"
                  << uni.timeSlots[gene.timeSlotIndex].endTime
                  << ", Instructor: " << uni.instructors[gene.instructorIndex].name << std::endl;
    }

    return 0;
}
