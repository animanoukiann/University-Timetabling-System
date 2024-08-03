#ifndef UNIVERSITY_HPP
#define UNIVERSITY_HPP

#include "Instructor.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <unordered_set>
#include <unordered_map> 
#include <sstream> 

class University { 
public:
    std::vector<Course> courses;
    std::vector<Instructor> instructors;
    std::vector<TimeSlot> timeSlots;
    int populationSize = 100;
    int generations = 1000;
    double crossoverRate = 0.8;
    double mutationRate = 0.1;

    struct Gene {
        int courseIndex;
        int timeSlotIndex;
        int instructorIndex;
    };

    struct Chromosome {
        std::vector<Gene> genes;
        double fitness;
    };

    void addCourse(Course courseName);
    void addInstructor(Instructor name);
    void addTimeSlot(TimeSlot time);
    void saveState(University &university, const std::string &file);
    University loadState(const std::string &file);
    std::vector<Gene> schedule();

    Chromosome createRandomChromosome();
    bool validateGeneIndices(const Gene& gene);
    void checkUsedTimeSlots(const TimeSlot& timeSlot, std::unordered_set<std::string>& usedTimeSlots, double& fitness);
    void checkInstructorAssignments(const Gene& gene, std::unordered_map<int, std::unordered_set<int>>& instructorAssignments, double& fitness);
    void checkInstructorAvailability(const Instructor& instructor, const TimeSlot& timeSlot, double& fitness);
    void checkInstructorPreferredCourses(const Instructor& instructor, const Course& course, double& fitness);
    double evaluateFitness(const Chromosome& chromosome);
    void crossover(Chromosome& offspring1, Chromosome& offspring2);
    void mutate(Chromosome& chromosome);
    std::vector<Chromosome> selectParents(const std::vector<Chromosome>& population);
    Chromosome geneticAlgorithm();
};

#endif
