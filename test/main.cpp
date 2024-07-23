#include "University.hpp"
#include "../argparse/argparse.h"

using namespace argparse;

std::vector<std::string> splitStringBySpace(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string word;
    while (iss >> word) {
        result.push_back(word);
    }
    return result;
}

int main(int argc, const char **argv)
{
    ArgumentParser parser("UniversityTimetable", "University Timetable System");

    parser.add_argument("--addInstructor", "addInstructor")
        .count(3)
        .description("Adds a new instructor by their name, preferred course and timeslot")
        .required(false);

    parser.add_argument("--addCourse", "addCourse")
        .count(2)
        .description("Adds a new course by its name and preferred time")
        .required(false);

    parser.add_argument("--addTimeslot", "addTimeslot")
        .count(3)
        .description("Adds a new timeslot by its day, start time and end time")
        .required(false);

    parser.add_argument()
        .name("--schedule")
        .description("Generate schedule")
        .required(false);

    parser.enable_help();
    auto err = parser.parse(argc, argv);
    if (err)
    {
        std::cout << err << std::endl;
        return (-1);
    }

    if (parser.exists("help"))
    {
        parser.print_help();
        return (0);
    }

    University uni;

    if (parser.exists("addInstructor"))
    {
        //here are some parts that can cause a seg fault if you give input improper way
        //so it must have 3 arguments and be careful with the 2nd argumet 
        //example` --addInstructor "John B" "Thursday 14:35 16:10" "MathPhys Eq"
        std::vector<std::string> inst_info = parser.get<std::vector<std::string>>("addInstructor");
        std::vector<std::string> time = splitStringBySpace(inst_info[1]);
        TimeSlot ts(time[0], time[1], time[2]);
        Course course(inst_info[2]);
        Instructor instructor(inst_info[0]);
        instructor.setAvailability(ts);
        instructor.setPreferredCourses(course);
        uni.addInstructor(instructor);
    }

    if (parser.exists("addCourse"))
    {
        //the same here
        //example --addCourse "Relational Databases" "Saturday 9:00 10:45"
        std::vector<std::string> course_info = parser.get<std::vector<std::string>>("addCourse");
        std::vector<std::string> time_1 = splitStringBySpace(course_info[1]);
        TimeSlot ts1(time_1[0], time_1[1], time_1[2]);
        Course course(course_info[0], {ts1});
        uni.addCourse(course);
    }

    if (parser.exists("addTimeslot"))
    {
        //the same here
        //example --addTimeslot "Monday" "12:50" "14:25"
        std::vector<std::string> time_info = parser.get<std::vector<std::string>>("addTimeslot");
        TimeSlot ts2(time_info[0], time_info[1], time_info[2]);
        uni.addTimeSlot(ts2);
    }

    uni.saveState(uni, "./result.json");

    University loadedUni;
    loadedUni = loadedUni.loadState("./result.json");

    std::cout << "Loaded Courses:" << std::endl;
    for (const auto &course : loadedUni.courses) {
        course.displayInfo();
    }

    std::cout << "Loaded Instructors:" << std::endl;
    for (const auto &instructor : loadedUni.instructors) {
        instructor.displayInfo();
    }

    if (parser.exists("--schedule"))
    {
        std::vector<University::Gene> schedule = uni.schedule();
        std::cout << "Best Schedule:" << std::endl;
        for (const auto& gene : schedule) {
            std::cout << "Course: " << uni.courses[gene.courseIndex].getCourseName()
                    << ", TimeSlot: " << uni.timeSlots[gene.timeSlotIndex].getDay() << " "
                    << uni.timeSlots[gene.timeSlotIndex].getStartTime() << "-"
                    << uni.timeSlots[gene.timeSlotIndex].getEndTime()
                    << ", Instructor: " << uni.instructors[gene.instructorIndex].getName() << std::endl;
        }
    }
    return 0;
}
