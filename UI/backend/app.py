from flask import Flask, request, jsonify
from flask_cors import CORS
from constants import UniJsonKeys
from db_utils import connect_to_db, insert_time_slot, insert_course, insert_instructor
import requests
import json
import fileinput

app = Flask(__name__)
CORS(app)

courses = []

@app.route('/addCourse', methods=['POST'])
def add_course():
    data = request.json
    course_name = data.get(UniJsonKeys.COURSE_NAME, '')
    time_slot = data.get(UniJsonKeys.TIME_SLOT_COURSE, '')
    if not course_name:
        return jsonify({'success': False, 'message': 'Course name is required'}), requests.codes.bad_request
    insert_func_args = [course_name]
    if time_slot:
        ts_members = time_slot.split()
        if len(ts_members) != 3:
            return jsonify({'success': False, 'message': 'Invalid time slot format'}), requests.codes.bad_request
        day, start_time, end_time = ts_members
        insert_func_args.extend([day, start_time, end_time])
        insert_time_slot(day, start_time, end_time)
    try:
        insert_course(*insert_func_args)
        return jsonify({'success': True, 'message': 'Course added successfully!'})
    except Exception as db_error:
        return jsonify({'success': False, 'message': str(db_error)}), requests.codes.internal_server_error

@app.route('/addTimeslot', methods=['POST'])
def add_time_slot():
    data = request.json
    ts = data.get(UniJsonKeys.TIME_SLOT, '')
    if not ts:
        return jsonify({'success': False, 'message': 'Time slot is required'}), requests.codes.bad_request
    
    ts_members = ts.split()
    if len(ts_members) != 3:
        return jsonify({'success': False, 'message': 'Invalid time slot format'}), requests.codes.bad_request
    day, start_time, end_time = ts_members
    try:
        insert_time_slot(day, start_time, end_time)
        return jsonify({'success': True, 'message': 'Time slot added successfully!'})
    except Exception as db_error:
        return jsonify({'success': False, 'message': str(db_error)}), requests.codes.internal_server_error
    
@app.route('/addInstructor', methods=['POST'])
def add_instructor():
    data = request.json
    inst = data.get(UniJsonKeys.INSTRUCTOR, '')
    course = data.get(UniJsonKeys.COURSE_NAME_INST, '')
    time  = data.get(UniJsonKeys.TIME_SLOT_INST, '')
    if not inst:
        return jsonify({'success': False, 'message': 'Instructor is required'}), requests.codes.bad_request
    insert_func_args = [inst]
    if course:
        insert_func_args.append(course)
        insert_course_args = [course]
    if not course and time:
        return jsonify({'success': False, 'message': 'If time is inputed course is mandatory'}), requests.codes.bad_request
    if time:
        ts_members = time.split()
        if len(ts_members) != 3:
            return jsonify({'success': False, 'message': 'Invalid time slot format'}), requests.codes.bad_request
        day, start_time, end_time = ts_members
        insert_func_args.extend([day, start_time, end_time])
        insert_course_args.extend([day, start_time, end_time])
        insert_time_slot(day, start_time, end_time)
    try:
        insert_instructor(*insert_func_args)
        insert_course(*insert_course_args)
        return jsonify({'success': True, 'message': 'Instructor added successfully!'})
    except Exception as db_error:
        return jsonify({'success': False, 'message': str(db_error)}), requests.codes.internal_server_error

@app.route('/schedule', methods=['GET'])
def schedule():
    conn = connect_to_db()
    cur = conn.cursor()
    try:
        cur.execute("SELECT json_build_object('courses', json_agg(row_to_json(t))) FROM (SELECT course_name, course_day, course_start_time, course_end_time FROM course) t")
        courses_result = cur.fetchone()[0]
        cur.execute("SELECT json_build_object('instructors', json_agg(row_to_json(t))) FROM (SELECT instructor_name, inst_course_name, inst_day, inst_start_time, inst_end_time FROM instructor) t")
        instructors_result = cur.fetchone()[0]
        cur.execute("SELECT json_build_object('timeSlots', json_agg(row_to_json(t))) FROM (SELECT day, start_time, end_time FROM time_slot) t")
        time_slot_result = cur.fetchone()[0]
    except Exception as error:
        print("Error runing a query to create a json", error)
    combined_result = {
        "courses": courses_result["courses"],
        "instructors": instructors_result["instructors"],
        "timeSlots": time_slot_result["timeSlots"]
    }

    with open('output.json', 'w') as f:
        json.dump(combined_result, f, indent=4)

    fileToReplace = '""'
    fileToSearch = "null"

    with open('output.json', 'r') as file:
        filedata = file.read()
    filedata = filedata.replace(fileToSearch, fileToReplace)
    with open('output.json', 'w') as file:
        file.write(filedata)
    cur.close()
    conn.close()
    return jsonify({'success': True, 'message': 'Json uploaded successfully!'})


if __name__ == '__main__':
    app.run(debug=True)