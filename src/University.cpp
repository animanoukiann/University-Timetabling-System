#include "University.hpp"

void University::addCourse(Course courseName) {
    courses.push_back(courseName);
} 

void University::addInstructor(Instructor name) {
    instructors.push_back(name);
}

void University::addTimeSlot(TimeSlot time) {
    timeSlots.push_back(time);
}

void University::saveState(University &university, const std::string &file_name) {
    json j;

    std::ifstream file_in(file_name);
    if (file_in.is_open()) {
        try {
            file_in >> j;
        }
        catch (const std::exception& e) {
            std::cerr << "Error reading existing data: " << e.what() << std::endl;
        }
        file_in.close();
    }
    json coursesJson = json::array();
    for (auto& course : university.courses) {
        coursesJson.push_back(json::parse(course.convertToJson()));
    }
    j["courses"].insert(j["courses"].end(), coursesJson.begin(), coursesJson.end());

    json instructorsJson = json::array();
    for (auto& instructor : university.instructors) {
        instructorsJson.push_back(json::parse(instructor.convertToJson()));
    }
    j["instructors"].insert(j["instructors"].end(), instructorsJson.begin(), instructorsJson.end());

    json timeSlotsJson = json::array();
    for (auto& timeSlot : university.timeSlots) {
        timeSlotsJson.push_back(json::parse(timeSlot.convertToJson()));
    }
    j["timeSlots"].insert(j["timeSlots"].end(), timeSlotsJson.begin(), timeSlotsJson.end());

    std::ofstream file(file_name);
    if (!file) {
        std::cerr << "Error while opening file: " << file_name << std::endl;
        return;
    }
    file << j.dump(4);
    file.close();
}

University University::loadState(University &university, const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Error while opening file: " << file_name << std::endl;
        return university;
    }

    json j;
    file >> j;
    file.close();

    for (const auto& courseJson : j["courses"]) {
        std::string courseJsonString = courseJson.dump();
        university.addCourse(Course::reverseFromJson(courseJsonString));
    }
    for (const auto& instructorJson : j["instructors"]) {
        std::string instructorJsonString = instructorJson.dump();
        university.addInstructor(Instructor::reverseFromJson(instructorJsonString));
    }
    for (const auto& timeSlotJson : j["timeSlots"]) {
        std::string timeSlotJsonString = timeSlotJson.dump();
        university.addTimeSlot(TimeSlot::reverseFromJson(timeSlotJsonString));
    }

    return university;
}

University::Chromosome University::createRandomChromosome() {
    Chromosome chromosome;

    if (courses.empty() || timeSlots.empty() || instructors.empty()) {
        std::cerr << "Error: Vectors (courses, timeSlots, instructors) are uninitialized or empty." << std::endl;
        return chromosome;
    }
    for (int i = 0; i < courses.size(); i++) {
        Gene gene;
        gene.courseIndex = i;
        gene.timeSlotIndex = rand() % timeSlots.size();
        gene.instructorIndex = rand() % instructors.size();
        chromosome.genes.push_back(gene);
    }
    chromosome.fitness = evaluateFitness(chromosome);
    return chromosome;
}

bool University::validateGeneIndices(const Gene& gene) {
    if (gene.courseIndex < 0 || gene.courseIndex >= courses.size()) {
        std::cerr << "Error: Gene has invalid course index." << std::endl;
        return false;
    }
    if (gene.timeSlotIndex < 0 || gene.timeSlotIndex >= timeSlots.size()) {
        std::cerr << "Error: Gene has invalid time slot index." << std::endl;
        return false;
    }
    if (gene.instructorIndex < 0 || gene.instructorIndex >= instructors.size()) {
        std::cerr << "Error: Gene has invalid instructor index." << std::endl;
        return false;
    }
    return true;
}

void University::checkUsedTimeSlots(const TimeSlot& timeSlot, std::unordered_set<std::string>& usedTimeSlots, double& fitness) {
    std::string timeSlotKey = timeSlot.getDay() + timeSlot.getStartTime() + timeSlot.getEndTime();
    if (usedTimeSlots.find(timeSlotKey) != usedTimeSlots.end()) {
        fitness -= 1000;
    } else {
        usedTimeSlots.insert(timeSlotKey);
    }
}

void University::checkInstructorAssignments(const Gene& gene, std::unordered_map<int, std::unordered_set<int>>& instructorAssignments, double& fitness) {
    if (instructorAssignments[gene.instructorIndex].find(gene.timeSlotIndex) != instructorAssignments[gene.instructorIndex].end()) {
        fitness -= 1000;
    } else {
        instructorAssignments[gene.instructorIndex].insert(gene.timeSlotIndex);
    }
}

void University::checkInstructorAvailability(const Instructor& instructor, const TimeSlot& timeSlot, double& fitness) {
    auto availability = instructor.getAvailability();
    if (!availability.empty()) {
        if (std::find(availability.begin(), availability.end(), timeSlot) != availability.end()) {
            fitness += 10;
        }
    } else {
        std::cerr << "Warning: Instructor " << instructor.getName() << " has no availability data." << std::endl;
    }
}

void University::checkInstructorPreferredCourses(const Instructor& instructor, const Course& course, double& fitness) {
    auto preferredCourses = instructor.getPreferredCourses();
    if (!preferredCourses.empty()) {
        if (std::find(preferredCourses.begin(), preferredCourses.end(), course) != preferredCourses.end()) {
            fitness += 5;
        }
    } else {
        std::cerr << "Warning: Instructor " << instructor.getName() << " has no preferred courses data." << std::endl;
    }
}

double University::evaluateFitness(const Chromosome& chromosome) {
    double fitness = 0.0;
    std::unordered_set<std::string> usedTimeSlots;
    std::unordered_map<int, std::unordered_set<int>> instructorAssignments;

    for (const auto& gene : chromosome.genes) {
        if (!validateGeneIndices(gene)) {
            continue;
        }

        const Course& course = courses[gene.courseIndex];
        const TimeSlot& timeSlot = timeSlots[gene.timeSlotIndex];
        const Instructor& instructor = instructors[gene.instructorIndex];

        checkUsedTimeSlots(timeSlot, usedTimeSlots, fitness);
        checkInstructorAssignments(gene, instructorAssignments, fitness);
        checkInstructorAvailability(instructor, timeSlot, fitness);
        checkInstructorPreferredCourses(instructor, course, fitness);
    }

    return fitness;
}

//this function mixes course assignments genes between two candidate solutions 
//chromosomes to potentially create better solutions
void University::crossover(Chromosome& offspring1, Chromosome& offspring2) {
    if (offspring1.genes.empty() || offspring2.genes.empty()) {
        std::cerr << "Error: Empty genes vector in crossover." << std::endl;
        return ;
    }
    if (static_cast<double>(rand()) / RAND_MAX < crossoverRate) {
        int crossoverPoint = rand() % offspring1.genes.size();
        if (crossoverPoint >= offspring1.genes.size() || crossoverPoint >= offspring2.genes.size()) {
            std::cerr << "Error: Crossover point out of bounds." << std::endl;
            return;
        }
        for (int i = crossoverPoint; i < offspring1.genes.size(); i++) {
            std::swap(offspring1.genes[i], offspring2.genes[i]);
        }
    }
}


void University::mutate(Chromosome& chromosome) {
    if (timeSlots.empty() || instructors.empty()) {
        std::cerr << "Error: Empty timeSlots or instructors vector in mutate." << std::endl;
        return;
    }
    for (auto& gene : chromosome.genes) {
        if ((double)rand() / RAND_MAX < mutationRate) {
            gene.timeSlotIndex = rand() % timeSlots.size();
            gene.instructorIndex = rand() % instructors.size();
        }
    }
    chromosome.fitness = evaluateFitness(chromosome);
}

std::vector<University::Chromosome> University::selectParents(const std::vector<Chromosome>& population) {
    std::vector<Chromosome> parents;

    if (population.empty()) {
        std::cerr << "Error: Population is empty." << std::endl;
        return parents;
    }
    for (int i = 0; i < population.size(); i++) {
        int parent1 = rand() % population.size();
        if (parent1 < 0 || parent1 >= population.size()) {
            std::cerr << "Error: Invalid parent index (parent1)." << std::endl;
            continue;
        }
        int parent2 = rand() % population.size();
        if (parent2 < 0 || parent2 >= population.size()) {
            std::cerr << "Error: Invalid parent index (parent2)." << std::endl;
            continue;
        }
        if (population[parent1].fitness > population[parent2].fitness) {
            parents.push_back(population[parent1]);
        }
        else {
            parents.push_back(population[parent2]);
        }
    }
    return parents;
}

University::Chromosome University::geneticAlgorithm() {
    srand(time(0));
    std::vector<Chromosome> population;
    for (int i = 0; i < populationSize; i++) {
        population.push_back(createRandomChromosome());
    }

    for (int gen = 0; gen < generations; gen++) {
        std::vector<Chromosome> newPopulation;
        std::vector<Chromosome> parents = selectParents(population);

        for (int i = 0; i < parents.size(); i += 2) {
            Chromosome offspring1 = parents[i];
            Chromosome offspring2 = parents[i + 1];
            crossover(offspring1, offspring2);
            mutate(offspring1);
            mutate(offspring2);
            newPopulation.push_back(offspring1);
            newPopulation.push_back(offspring2);
        }

        population = newPopulation;
        for (auto& chromosome : population) {
            chromosome.fitness = evaluateFitness(chromosome);
        }
        auto bestChromosome = std::max_element(population.begin(), population.end(),
            [](const Chromosome& a, const Chromosome& b) {
                return a.fitness < b.fitness;
            });
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
