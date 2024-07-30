#include "Course.hpp"

Course::Course(std::string courseName) : courseName(courseName) {} 

Course::Course(std::string courseName, std::vector<TimeSlot> preferredTimeSlots) : courseName(courseName), 
preferredTimeSlots (preferredTimeSlots) {}

void Course::displayInfo() const {
    std::cout << "The course: '" << courseName << "'\nIts preferred time slots" << std::endl;
    for (auto i = preferredTimeSlots.begin(); i != preferredTimeSlots.end(); i++) {
        i->displayInfo();//TimeSlot's displayInfo
    }
}

void Course::setPreferredTimeSlot(TimeSlot &timeSlot) {
    preferredTimeSlots.push_back(timeSlot);
}

std::string Course::convert_to_json() {
    std::string json = "{\n  \"courseName\": \"" + courseName + "\",\n  \"preferredTimeSlots\": [\n";
    for (int i = 0; i < preferredTimeSlots.size(); i++) {
        json += "    " + preferredTimeSlots[i].convert_to_json();
        if (i != preferredTimeSlots.size() - 1) {
            json += ",";
        }
        json += "\n";
    }
    json += "  ]\n}";
    return json;
}

std::string Course::getCourseName() const {
    return courseName;
}

std::vector<TimeSlot> Course::getPreferredTimeSlots() const {
    return preferredTimeSlots;
}

bool Course::operator==(const Course& other) const {
    return courseName == other.courseName && preferredTimeSlots == other.preferredTimeSlots;
}

Course Course::reverse_from_json(std::string &json) {
    int name_index = json.find("courseName") + 14;
    std::string courseName = json.substr(name_index, json.find("\"", name_index) - name_index);
    std::vector<TimeSlot> preferredTimeSlots;
    int ts_start_index = json.find("preferredTimeSlots") + 20;
    int ts_end_index = json.find("]", ts_start_index);
    std::string ts_arr = json.substr(ts_start_index, ts_end_index - ts_start_index);
    int start = 0;
    int end = ts_arr.find("},{", start);
    std::string arr;
    while (end != std::string::npos) { 
        arr = ts_arr.substr(start, end - start + 1);
        preferredTimeSlots.push_back(TimeSlot::reverse_from_json(arr));
        start = end + 2;
        end = ts_arr.find("},{", start);
    }
    arr = ts_arr.substr(start);
    preferredTimeSlots.push_back(TimeSlot::reverse_from_json(arr));

    return Course(courseName, preferredTimeSlots);
}
