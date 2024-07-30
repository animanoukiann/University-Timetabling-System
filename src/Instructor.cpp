#include "Instructor.hpp"

Instructor::Instructor(std::string name): name(name) {}

Instructor::Instructor(std::string name, std::vector<TimeSlot> availability,
std::vector<Course> preferredCourses): name(name), availability(availability),
preferredCourses(preferredCourses) {} 

void Instructor::displayInfo() const {
    std::cout << "Instructor's name: '" << name << "'\nHis/Her availability" << std::endl;
    for (auto i = availability.begin(); i != availability.end(); i++) {
        i->displayInfo();//TimeSlot's displayInfo
    }
    std::cout << "His/Her preferred courses" << std::endl;
    for (auto i = preferredCourses.begin(); i != preferredCourses.end(); i++) {
        i->displayInfo();//Course's displayInfo
    }
}

std::string Instructor::getName() const {
    return name;
}

std::vector<TimeSlot> Instructor::getAvailability() const {
    return availability;
}

std::vector<Course> Instructor::getPreferredCourses() const {
    return preferredCourses;
}

void Instructor::setAvailability(TimeSlot &timeSlot) {
    availability.push_back(timeSlot);
}

void Instructor::setPreferredCourses(Course &course) {
    preferredCourses.push_back(course);
}

std::string Instructor::convert_to_json() {
    std::string json = "{\n  \"name\": \"" + name + "\",\n  \"availability\": [\n";
    for (int i = 0; i < availability.size(); i++) {
        json += "    " + availability[i].convert_to_json();
        if (i != availability.size() - 1) {
            json += ",";
        }
        json += "\n";
    }
    json += "  ],\n  \"preferredCourses\": [\n";
    for (int i = 0; i < preferredCourses.size(); i++) {
        json += "    " + preferredCourses[i].convert_to_json();
        if (i != preferredCourses.size() - 1) {
            json += ",";
        }
        json += "\n";
    }
    json += "  ]\n}";
    return json;
}

Instructor Instructor::reverse_from_json(std::string &json) {
    int name_index = json.find("\"name\": \"") + 9;
    int name_end_index = json.find("\"", name_index);
    std::string name = json.substr(name_index, name_end_index - name_index);
    std::vector<TimeSlot> availability;
    int ts_start_index = json.find("\"availability\": [") + 18;
    int ts_end_index = json.find("]", ts_start_index);
    std::string ts_arr = json.substr(ts_start_index, ts_end_index - ts_start_index);
    int start = 0;
    int end = ts_arr.find("},{", start);
    std::string arr,arr1;
    while (end != std::string::npos) {
        arr = ts_arr.substr(start, end - start + 1);
        availability.push_back(TimeSlot::reverse_from_json(arr));
        start = end + 3;
        end = ts_arr.find("},{", start);
    }
    arr = ts_arr.substr(start);
    availability.push_back(TimeSlot::reverse_from_json(arr));

    std::vector<Course> preferredCourses;
    int pc_start_index = json.find("\"preferredCourses\": [") + 21;
    int pc_end_index = json.find("]", pc_start_index);
    std::string pc_arr = json.substr(pc_start_index, pc_end_index - pc_start_index);
    start = 0;
    end = pc_arr.find("},{", start);
    while (end != std::string::npos) {
        arr1 = pc_arr.substr(start, end - start + 1);
        preferredCourses.push_back(Course::reverse_from_json(arr1));
        start = end + 3;
        end = pc_arr.find("},{", start);
    }
    arr1 = pc_arr.substr(start);
    preferredCourses.push_back(Course::reverse_from_json(arr1));

    return Instructor(name, availability, preferredCourses);
}
