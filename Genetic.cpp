#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include "University.hpp"

University::Chromosome University::createRandomChromosome()
{
    Chromosome chromosome;

    if (courses.empty() || timeSlots.empty() || instructors.empty())
    {
        std::cerr << "Error: Vectors (courses, timeSlots, instructors) are uninitialized or empty." << std::endl;
        return chromosome;
    }
    for (int i = 0; i < courses.size(); i++)
    {
        Gene gene;
        gene.courseIndex = i;
        gene.timeSlotIndex = rand() % timeSlots.size();
        gene.instructorIndex = rand() % instructors.size();
        chromosome.genes.push_back(gene);
    }
    chromosome.fitness = evaluateFitness(chromosome);
    return chromosome;
}

double University::evaluateFitness(const Chromosome& chromosome)
{
    double fitness = 0.0;
    std::unordered_set<std::string> usedTimeSlots;
    //Tracks which time slots have already been assigned to
    // courses to avoid double booking penalties.
    std::unordered_map<int, std::unordered_set<int>> instructorAssignments;
    //Tracks which time slots have been assigned to instructors 
    //to avoid double booking penalties    
    for (const auto& gene : chromosome.genes)
    {
        if (gene.courseIndex < 0 || gene.courseIndex >= courses.size())
        {
            std::cerr << "Error: Gene has invalid course index." << std::endl;
            continue;
        }
        if (gene.timeSlotIndex < 0 || gene.timeSlotIndex >= timeSlots.size())
        {
            std::cerr << "Error: Gene has invalid time slot index." << std::endl;
            continue;
        }
        if (gene.instructorIndex < 0 || gene.instructorIndex >= instructors.size())
        {
            std::cerr << "Error: Gene has invalid instructor index." << std::endl;
            continue;
        }
        const Course& course = courses[gene.courseIndex];
        const TimeSlot& timeSlot = timeSlots[gene.timeSlotIndex];
        const Instructor& instructor = instructors[gene.instructorIndex];

        std::string timeSlotKey = timeSlot.getDay() + timeSlot.getStartTime()
        + timeSlot.getEndTime();//Checks if the timeSlot has already been used 

        if (usedTimeSlots.find(timeSlotKey) != usedTimeSlots.end())
            fitness -= 1000;//time slot has already been assigned so decrease its fitness score
        else
            usedTimeSlots.insert(timeSlotKey);

        if (instructorAssignments[gene.instructorIndex].find(gene.timeSlotIndex)
         != instructorAssignments[gene.instructorIndex].end())
            fitness -= 1000;
        else
            instructorAssignments[gene.instructorIndex].insert(gene.timeSlotIndex);

        if (std::find(instructor.getAvailability().begin(), 
        instructor.getAvailability().end(), timeSlot) != instructor.getAvailability().end())
            fitness += 10;

        if (std::find(instructor.getPreferredCourses().begin(),
        instructor.getPreferredCourses().end(), course) != instructor.getPreferredCourses().end())
            fitness += 5;
    }

    return fitness;
}

//this function mixes course assignments genes between two candidate solutions 
//chromosomes to potentially create better solutions
void University::crossover(Chromosome& offspring1, Chromosome& offspring2)
{
    if (offspring1.genes.empty() || offspring2.genes.empty())
    {
        std::cerr << "Error: Empty genes vector in crossover." << std::endl;
        return ;
    }
    if (static_cast<double>(rand()) / RAND_MAX < crossoverRate)
    {
        int crossoverPoint = rand() % offspring1.genes.size();
        if (crossoverPoint >= offspring1.genes.size() || crossoverPoint >= offspring2.genes.size())
        {
            std::cerr << "Error: Crossover point out of bounds." << std::endl;
            return;
        }
        for (int i = crossoverPoint; i < offspring1.genes.size(); i++) 
            std::swap(offspring1.genes[i], offspring2.genes[i]);
    }
}


void University::mutate(Chromosome& chromosome)
{
    if (timeSlots.empty() || instructors.empty())
    {
        std::cerr << "Error: Empty timeSlots or instructors vector in mutate." << std::endl;
        return;
    }
    for (auto& gene : chromosome.genes)
    {
        if ((double)rand() / RAND_MAX < mutationRate)
        {
            gene.timeSlotIndex = rand() % timeSlots.size();
            gene.instructorIndex = rand() % instructors.size();
        }
    }
    chromosome.fitness = evaluateFitness(chromosome);
}

std::vector<University::Chromosome> University::selectParents(const std::vector<Chromosome>& population)
{
    std::vector<Chromosome> parents;

    if (population.empty())
    {
        std::cerr << "Error: Population is empty." << std::endl;
        return parents;
    }
    for (int i = 0; i < population.size(); i++)
    {
        int parent1 = rand() % population.size();
        if (parent1 < 0 || parent1 >= population.size())
        {
            std::cerr << "Error: Invalid parent index (parent1)." << std::endl;
            continue;
        }
        int parent2 = rand() % population.size();
        if (parent2 < 0 || parent2 >= population.size())
        {
            std::cerr << "Error: Invalid parent index (parent2)." << std::endl;
            continue;
        }
        if (population[parent1].fitness > population[parent2].fitness)
            parents.push_back(population[parent1]);
        else
            parents.push_back(population[parent2]);
    }
    return parents;
}

University::Chromosome University::geneticAlgorithm() {
    srand(time(0));
    std::vector<Chromosome> population;
    for (int i = 0; i < populationSize; i++)
        population.push_back(createRandomChromosome());

    for (int gen = 0; gen < generations; gen++)
    {
        std::vector<Chromosome> newPopulation;
        std::vector<Chromosome> parents = selectParents(population);

        for (int i = 0; i < parents.size(); i += 2)
        {
            Chromosome offspring1 = parents[i];
            Chromosome offspring2 = parents[i + 1];
            crossover(offspring1, offspring2);
            mutate(offspring1);
            mutate(offspring2);
            newPopulation.push_back(offspring1);
            newPopulation.push_back(offspring2);
        }

        population = newPopulation;
        for (auto& chromosome : population)
            chromosome.fitness = evaluateFitness(chromosome);
        auto bestChromosome = std::max_element(population.begin(), population.end(),
            [](const Chromosome& a, const Chromosome& b) {
                return a.fitness < b.fitness;
            });

        std::cout << "Generation " << gen << " Best Fitness: " << bestChromosome->fitness << std::endl;
    }
    auto bestChromosome = std::max_element(population.begin(), population.end(),
        [](const Chromosome& a, const Chromosome& b) {
            return a.fitness < b.fitness;
        });
    return *bestChromosome;
}

std::vector<University::Gene> University::schedule() {
    Chromosome bestChromosome = geneticAlgorithm();
    return bestChromosome.genes;
}