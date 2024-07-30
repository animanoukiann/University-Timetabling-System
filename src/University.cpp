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
    std::ofstream file(file_name);
    if (!file) {
        std::cerr << "Error while opening file: " << file_name << std::endl;
        return ;
    }
    std::string json = "{\n  \"courses\": [\n";
    for (int i = 0; i < university.courses.size(); i++) {
        json += "    " + university.courses[i].convert_to_json();
        if (i != university.courses.size() - 1) {
            json += ",";
        }
        json += "\n";
    }
    json += "  ],\n  \"instructors\": [\n";
    for (int i = 0; i < university.instructors.size(); i++) {
        json += "    " + university.instructors[i].convert_to_json();
        if (i != university.instructors.size() - 1) {
            json += ",";
        }
        json += "\n";
    }
    json += "  ],\n  \"timeSlots\": [\n";
    for (int i = 0; i < university.timeSlots.size(); i++) {
        json += "    " + university.timeSlots[i].convert_to_json();
        if (i != university.timeSlots.size() - 1) {
            json += ",";
        }
        json += "\n";
    }
    json += "  ]\n}";
    file << json; //writing in file
    file.close();
}

std::vector<std::string> parse_json_array(std::string &json, const std::string &key);
std::vector<std::string> parse_json_objects(std::string &json);

University University::loadState(const std::string &file_name) {
    University university;
    std::ifstream file(file_name);
    if (!file) {
        std::cerr << "Error while opening file: " << file_name << std::endl;
        return university;
    }

    std::string json((std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>());//load all the file in json
    file.close();

    std::vector<std::string> course_jsons = parse_json_array(json, "courses");
    for (int i = 0; i < course_jsons.size(); i++) {
        university.addCourse(Course::reverse_from_json(course_jsons[i]));
    }

    std::vector<std::string> instructor_jsons = parse_json_array(json, "instructors");
    for (int i = 0; i < instructor_jsons.size(); i++) {
        university.addInstructor(Instructor::reverse_from_json(instructor_jsons[i]));
    }

    std::vector<std::string> timeSlot_jsons = parse_json_array(json, "timeSlots");
    for (int i = 0; i < timeSlot_jsons.size(); i++) {
        university.addTimeSlot(TimeSlot::reverse_from_json(timeSlot_jsons[i]));
    }
    return university;
}

std::vector<std::string> parse_json_array(std::string &json, const std::string &key) {
    std::vector<std::string> result;

    // Find the start of the array associated with the given key
    int start_index = json.find("\"" + key + "\": [");
    if (start_index == std::string::npos) {
        return result;
    }
    start_index += key.length() + 5; // +5 ": "[

    int end_index = json.find("]", start_index);
    std::string array = json.substr(start_index, end_index - start_index);
    return parse_json_objects(array);
}

std::vector<std::string> parse_json_objects(std::string &json) {
    std::vector<std::string> result;
    int start_index = 0, end_index;

    start_index = json.find("{", start_index);
    while (start_index != std::string::npos) {
        end_index = json.find("}", start_index);
        if (end_index == std::string::npos) {
            break;
        }
        result.push_back(json.substr(start_index, end_index - start_index + 1));
        start_index = end_index + 1;
        start_index = json.find("{", start_index);
    }
    return result;
}
