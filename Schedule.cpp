#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

struct Gene {
    int courseIndex;
    int timeSlotIndex;
    int instructorIndex;
};

// Chromosome representing a complete schedule
struct Chromosome {
    std::vector<Gene> genes;
    double fitness;
};

// Global variables (you may want to encapsulate these into a class)
University university;
int populationSize = 100;
int generations = 1000;
double crossoverRate = 0.8;
double mutationRate = 0.1;

// Function prototypes
Chromosome createRandomChromosome();
double evaluateFitness(const Chromosome& chromosome);
void crossover(Chromosome& offspring1, Chromosome& offspring2);
void mutate(Chromosome& chromosome);
std::vector<Chromosome> selectParents(const std::vector<Chromosome>& population);
Chromosome geneticAlgorithm();

// Create a random chromosome
Chromosome createRandomChromosome() {
    Chromosome chromosome;
    for (size_t i = 0; i < university.courses.size(); ++i) {
        Gene gene;
        gene.courseIndex = i;
        gene.timeSlotIndex = rand() % university.timeSlots.size();
        gene.instructorIndex = rand() % university.instructors.size();
        chromosome.genes.push_back(gene);
    }
    chromosome.fitness = evaluateFitness(chromosome);
    return chromosome;
}

// Evaluate the fitness of a chromosome
double evaluateFitness(const Chromosome& chromosome) {
    double fitness = 0.0;
    // Implement your fitness function here
    // Penalize for violating hard constraints
    // Reward for satisfying soft constraints
    return fitness;
}

// Crossover two chromosomes to produce offspring
void crossover(Chromosome& offspring1, Chromosome& offspring2) {
    if ((double)rand() / RAND_MAX < crossoverRate) {
        int crossoverPoint = rand() % offspring1.genes.size();
        for (size_t i = 0; i < crossoverPoint; ++i) {
            std::swap(offspring1.genes[i], offspring2.genes[i]);
        }
    }
}

// Mutate a chromosome
void mutate(Chromosome& chromosome) {
    for (auto& gene : chromosome.genes) {
        if ((double)rand() / RAND_MAX < mutationRate) {
            gene.timeSlotIndex = rand() % university.timeSlots.size();
            gene.instructorIndex = rand() % university.instructors.size();
        }
    }
    chromosome.fitness = evaluateFitness(chromosome);
}

// Select parents for reproduction using tournament selection
std::vector<Chromosome> selectParents(const std::vector<Chromosome>& population) {
    std::vector<Chromosome> parents;
    for (size_t i = 0; i < population.size(); ++i) {
        int parent1 = rand() % population.size();
        int parent2 = rand() % population.size();
        if (population[parent1].fitness > population[parent2].fitness) {
            parents.push_back(population[parent1]);
        } else {
            parents.push_back(population[parent2]);
        }
    }
    return parents;
}

// Main Genetic Algorithm function
Chromosome geneticAlgorithm() {
    srand(time(0));
    std::vector<Chromosome> population;
    for (int i = 0; i < populationSize; ++i) {
        population.push_back(createRandomChromosome());
    }

    for (int gen = 0; gen < generations; ++gen) {
        std::vector<Chromosome> newPopulation;

        // Selection
        std::vector<Chromosome> parents = selectParents(population);

        // Crossover and Mutation
        for (size_t i = 0; i < parents.size(); i += 2) {
            Chromosome offspring1 = parents[i];
            Chromosome offspring2 = parents[i + 1];
            crossover(offspring1, offspring2);
            mutate(offspring1);
            mutate(offspring2);
            newPopulation.push_back(offspring1);
            newPopulation.push_back(offspring2);
        }

        // Replace old population with new population
        population = newPopulation;

        // Evaluate fitness of the new population
        for (auto& chromosome : population) {
            chromosome.fitness = evaluateFitness(chromosome);
        }

        // Find the best chromosome in the current population
        auto bestChromosome = std::max_element(population.begin(), population.end(),
            [](const Chromosome& a, const Chromosome& b) {
                return a.fitness < b.fitness;
            });

        // Print the best fitness of the current generation
        std::cout << "Generation " << gen << " Best Fitness: " << bestChromosome->fitness << std::endl;
    }

    // Return the best chromosome found
    auto bestChromosome = std::max_element(population.begin(), population.end(),
        [](const Chromosome& a, const Chromosome& b) {
            return a.fitness < b.fitness;
        });
    return *bestChromosome;
}