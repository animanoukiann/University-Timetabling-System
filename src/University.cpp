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

    json coursesJson = json::array();
    for (auto& course : university.courses) {
        coursesJson.push_back(json::parse(course.convert_to_json()));
    }
    j["courses"] = coursesJson;

    json instructorsJson = json::array();
    for (auto& instructor : university.instructors) {
        instructorsJson.push_back(json::parse(instructor.convert_to_json()));
    }
    j["instructors"] = instructorsJson;

    json timeSlotsJson = json::array();
    for (auto& timeSlot : university.timeSlots) {
        timeSlotsJson.push_back(json::parse(timeSlot.convert_to_json()));
    }
    j["timeSlots"] = timeSlotsJson;

    std::ofstream file(file_name);
    if (!file) {
        std::cerr << "Error while opening file: " << file_name << std::endl;
        return;
    }
    file << j.dump(4);
    file.close();
}

University University::loadState(const std::string &file_name) {
    University university;
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
        university.addCourse(Course::reverse_from_json(courseJsonString));
    }

    for (const auto& instructorJson : j["instructors"]) {
        std::string instructorJsonString = instructorJson.dump();
        university.addInstructor(Instructor::reverse_from_json(instructorJsonString));
    }

    for (const auto& timeSlotJson : j["timeSlots"]) {
        std::string timeSlotJsonString = timeSlotJson.dump();
        university.addTimeSlot(TimeSlot::reverse_from_json(timeSlotJsonString));
    }

    return university;
}

// std::vector<std::string> parse_json_array(std::string &json, const std::string &key);
// std::vector<std::string> parse_json_objects(std::string &json);

// University University::loadState(const std::string &file_name) {
//     University university;
//     std::ifstream file(file_name);
//     if (!file) {
//         std::cerr << "Error while opening file: " << file_name << std::endl;
//         return university;
//     }

//     std::string json((std::istreambuf_iterator<char>(file)),
//     std::istreambuf_iterator<char>());//load all the file in json
//     file.close();

//     std::vector<std::string> course_jsons = parse_json_array(json, "courses");
//     for (int i = 0; i < course_jsons.size(); i++) {
//         university.addCourse(Course::reverse_from_json(course_jsons[i]));
//     }

//     std::vector<std::string> instructor_jsons = parse_json_array(json, "instructors");
//     for (int i = 0; i < instructor_jsons.size(); i++) {
//         university.addInstructor(Instructor::reverse_from_json(instructor_jsons[i]));
//     }

//     std::vector<std::string> timeSlot_jsons = parse_json_array(json, "timeSlots");
//     for (int i = 0; i < timeSlot_jsons.size(); i++) {
//         university.addTimeSlot(TimeSlot::reverse_from_json(timeSlot_jsons[i]));
//     }
//     return university;
// }

// std::vector<std::string> parse_json_array(std::string &json, const std::string &key) {
//     std::vector<std::string> result;

//     // Find the start of the array associated with the given key
//     int start_index = json.find("\"" + key + "\": [");
//     if (start_index == std::string::npos) {
//         return result;
//     }
//     start_index += key.length() + 5; // +5 ": "[

//     int end_index = json.find("]", start_index);
//     std::string array = json.substr(start_index, end_index - start_index);
//     return parse_json_objects(array);
// }

// std::vector<std::string> parse_json_objects(std::string &json) {
//     std::vector<std::string> result;
//     int start_index = 0, end_index;

//     start_index = json.find("{", start_index);
//     while (start_index != std::string::npos) {
//         end_index = json.find("}", start_index);
//         if (end_index == std::string::npos) {
//             break;
//         }
//         result.push_back(json.substr(start_index, end_index - start_index + 1));
//         start_index = end_index + 1;
//         start_index = json.find("{", start_index);
//     }
//     return result;
// }
