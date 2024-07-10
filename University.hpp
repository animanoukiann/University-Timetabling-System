#ifndef UNIVERSITY_HPP
# define UNIVERSITY_HPP

#include "Instructor.hpp"
#include <sstream>

class University
{
    private:
    int populationSize = 100;
    int generations = 1000;
    double crossoverRate = 0.8;
    double mutationRate = 0.1;

    struct Gene
    {
        int courseIndex;
        int timeSlotIndex;
        int instructorIndex;
    };

    struct Chromosome
    {
        std::vector<Gene> genes;
        double fitness;
    };

    Chromosome createRandomChromosome();
    double evaluateFitness(const Chromosome& chromosome);
    void crossover(Chromosome& offspring1, Chromosome& offspring2);
    void mutate(Chromosome& chromosome);
    std::vector<Chromosome> selectParents(const std::vector<Chromosome>& population);
    Chromosome geneticAlgorithm();

    public:
        std::vector<Course> courses;
        std::vector<Instructor> instructors;
        std::vector<TimeSlot> timeSlots;

        void addCourse(Course courseName);
        void addInstructor(Instructor name);
        void addTimeSlot(TimeSlot time);
        void saveState(University &university, const std::string &file);
        University loadState(const std::string &file);
        std::vector<Gene> schedule();
};

#endif
