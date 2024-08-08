from flask import Flask, request, jsonify
from flask_cors import CORS
from db_utils import connect_to_db, insert_time_slot
import subprocess
import os

app = Flask(__name__)
CORS(app)

courses = []

@app.route('/addCourse', methods=['POST'])
def add_course():
    data = request.json
    course_name = data.get('courseName', '')
    time_slot = data.get('timeSlotCourse', '')
    if not course_name:
        return jsonify({'success': False, 'message': 'Course name is required'}), 400
    
    print(f"Received data: {data}")

    cpp_exec_path = os.path.join('..', '..', 'build', 'output', 'main')

    try:
        if not time_slot:
            result = subprocess.run([cpp_exec_path, '--addCourse', course_name], capture_output=True, text=True)
        else:
            result = subprocess.run([cpp_exec_path, '--addCourse', course_name, time_slot], capture_output=True, text=True)
        if result.returncode == 0:
            return jsonify({'success': True, 'message': 'Course added successfully!'})
        else:
            return jsonify({'success': False, 'message': 'Failed to add course', 'details': result.stderr}), 500
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), 500


@app.route('/addTimeslot', methods=['POST'])
def add_time_slot():
    data = request.json
    ts = data.get('timeSlot', '')

    if not ts:
        return jsonify({'success': False, 'message': 'Time slot is required'}), 400
    
    print(f"Received data: {data}")

    cpp_exec_path = os.path.join('..', '..', 'build', 'output', 'main')

    try:
        result = subprocess.run([cpp_exec_path, '--addTimeslot', ts], capture_output=True, text=True)
        if result.returncode == 0:
            ts_members = ts.split()
            if len(ts_members) != 3:
                return jsonify({'success': False, 'message': 'Invalid time slot format'}), 400
            day, start_time, end_time = ts_members
            
            try:
                insert_time_slot(day, start_time, end_time)
                return jsonify({'success': True, 'message': 'Time slot added successfully!'})
            except Exception as db_error:
                return jsonify({'success': False, 'message': str(db_error)}), 500
        else:
            return jsonify({'success': False, 'message': 'Failed to add time slot', 'details': result.stderr}), 500
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), 500
    
@app.route('/addInstructor', methods=['POST'])
def add_instructor():
    data = request.json
    inst = data.get('instructor', '')
    course = data.get('courseNameInst', '')
    time  = data.get('timeSlotInst', '')


    if not inst:
        return jsonify({'success': False, 'message': 'Instructor is required'}), 400
    print(f"Received data: {data}")

    cpp_exec_path = os.path.join('..', '..', 'build', 'output', 'main')

    try:
        if not course and not time:
            result = subprocess.run([cpp_exec_path, '--addInstructor', inst], capture_output=True, text=True)
        elif not time and course:
            result = subprocess.run([cpp_exec_path, '--addInstructor', inst, course], capture_output=True, text=True)
        elif time and course:
            result = subprocess.run([cpp_exec_path, '--addInstructor', inst, course, time], capture_output=True, text=True)
        else:
            return jsonify({'success': False, 'message': 'If time is inputed course is mandatory'}), 400
        if result.returncode == 0:
            return jsonify({'success': True, 'message': 'Instructor added successfully!'})
        else:
            return jsonify({'success': False, 'message': 'Failed to add instructor', 'details': result.stderr}), 500
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), 500

@app.route('/schedule', methods=['GET'])
def schedule():
    cpp_exec_path = os.path.join('..', '..', 'build', 'output', 'main')

    try:
        result = subprocess.run([cpp_exec_path, '--schedule'], capture_output=True, text=True)

        if result.returncode == 0:
            return jsonify({'success': True, 'message': 'Algorithm executed successfully!', 'output': result.stdout})
        else:
            return jsonify({'success': False, 'message': 'Failed to execute algorithm', 'details': result.stderr}), 500
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)