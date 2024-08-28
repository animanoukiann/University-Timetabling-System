import requests
import json
import psycopg2
import pyuniversity
from flask import Flask, request, jsonify
from flask_cors import CORS
from constants import UniJsonKeys
from constants import jsonFieldKeys
from dbUtils import insertTimeSlot, insertCourse, insertInstructor
from dbUtils import connection

app = Flask(__name__)
CORS(app)

def tsProvided(timeSlot, insertFuncArgs=None):
    tsMembers = timeSlot.split()
    if len(tsMembers) != 3:
        return False
    day, startTime, endTime = tsMembers
    insertFuncArgs.extend([day, startTime, endTime])
    insertTimeSlot(day, startTime, endTime)
    return True

@app.route('/addCourse', methods=['POST'])
def add_course():
    data = request.json
    courseName = data.get(UniJsonKeys.COURSE_NAME, '')
    timeSlot = data.get(UniJsonKeys.TIME_SLOT_COURSE, '')
    if not courseName:
        return jsonify({'success': False, 'message': 'Course name is required'}), \
            requests.codes.bad_request
    insertFuncArgs = [courseName]
    if timeSlot:
        success = tsProvided(timeSlot, insertFuncArgs)
        if not success:
            return jsonify({'success': False, 'message': 'Invalid time slot format'}), \
                requests.codes.bad_request
    try:
        insertCourse(*insertFuncArgs)
        return jsonify({'success': True, 'message': 'Course added successfully!'})
    except Exception as dbError:
        return jsonify({'success': False, 'message': str(dbError)}), \
            requests.codes.internal_server_error

@app.route('/addTimeslot', methods=['POST'])
def add_time_slot():
    data = request.json
    ts = data.get(UniJsonKeys.TIME_SLOT, '')
    if not ts:
        return jsonify({'success': False, 'message': 'Time slot is required'}), \
            requests.codes.bad_request
    
    tsMembers = ts.split()
    if len(tsMembers) != 3:
        return jsonify({'success': False, 'message': 'Invalid time slot format'}), \
            requests.codes.bad_request
    day, startTime, endTime = tsMembers
    try:
        insertTimeSlot(day, startTime, endTime)
        return jsonify({'success': True, 'message': 'Time slot added successfully!'})
    except Exception as dbError:
        return jsonify({'success': False, 'message': str(dbError)}), \
            requests.codes.internal_server_error
    
@app.route('/addInstructor', methods=['POST'])
def add_instructor():
    data = request.json
    inst = data.get(UniJsonKeys.INSTRUCTOR, '')
    course = data.get(UniJsonKeys.COURSE_NAME_INST, '')
    time  = data.get(UniJsonKeys.TIME_SLOT_INST, '')
    if not inst:
        return jsonify({'success': False, 'message': 'Instructor is required'}), \
            requests.codes.bad_request
    insertFuncArgs = [inst]
    if course:
        insertFuncArgs.append(course)
        insertCourseArgs = [course]
        insertCourse(*insertCourseArgs)
    if not course and time:
        return jsonify({'success': False, 'message': 'If time is inputed course is mandatory'}), \
            requests.codes.bad_request
    if time:
        success = tsProvided(time, insertFuncArgs)
        if not success:
            return jsonify({'success': False, 'message': 'Invalid time slot format'}), \
                requests.codes.bad_request
    try:
        insertInstructor(*insertFuncArgs)
        return jsonify({'success': True, 'message': 'Instructor added successfully!'})
    except Exception as dbError:
        return jsonify({'success': False, 'message': str(dbError)}), \
            requests.codes.internal_server_error
    
def replaceNoneWithEmptyString(d):
    if isinstance(d, dict):
        return {k: replaceNoneWithEmptyString(v) for k, v in d.items()}
    elif isinstance(d, list):
        return [replaceNoneWithEmptyString(item) for item in d]
    else:
        return '' if d is None else d

@app.route('/schedule', methods=['GET'])
def schedule():
    cur = connection.cursor()
    courseFieldName = "courses"
    instructorFieldName = "instructors"
    timeSlotFieldName = "timeSlots"
    try:
        cur.execute("SELECT json_build_object('courses', json_agg(row_to_json(t))) FROM"\
            "(SELECT course_name, course_day, course_start_time, course_end_time FROM course) t")
        coursesResult = cur.fetchone()[0]
        
        cur.execute("SELECT json_build_object('instructors', json_agg(row_to_json(t))) FROM" \
                    "(SELECT instructor_name, inst_course_name, inst_day, "\
                    "inst_start_time, inst_end_time FROM instructor) t")
        instructorsResult = cur.fetchone()[0]
        cur.execute("SELECT json_build_object('timeSlots', json_agg(row_to_json(t))) FROM "\
                    "(SELECT day, start_time, end_time FROM time_slot) t")
        timeSlotResult = cur.fetchone()[0]
    except Exception as error:
        return jsonify({'success': False, 'message': str(error)}), \
            requests.codes.internal_server_error
    combinedResult = {
        jsonFieldKeys.course : coursesResult[courseFieldName],
        jsonFieldKeys.instructor : instructorsResult[instructorFieldName],
        jsonFieldKeys.timeSlot : timeSlotResult[timeSlotFieldName]
    }
    combinedResult = replaceNoneWithEmptyString(combinedResult)
    x = json.dumps(combinedResult)
    uni = pyuniversity.University()
    result = uni.schedule(x)
    cur.close()
    return jsonify({'success': True, 'message': 'Schedule generated successfully!',
                    'result': result})


if __name__ == '__main__':
    app.run(debug=True)