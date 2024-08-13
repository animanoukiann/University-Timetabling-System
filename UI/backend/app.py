from flask import Flask, request, jsonify
from flask_cors import CORS
from constants import UniJsonKeys
import subprocess
import requests
import os

app = Flask(__name__)
CORS(app)

@app.route('/addCourse', methods=['POST'])
def add_course():
    data = request.json
    course_name = data.get(UniJsonKeys.COURSE_NAME, '')
    time_slot = data.get(UniJsonKeys.TIME_SLOT_COURSE, '')
    if not course_name:
        return jsonify({'success': False, 'message': 'Course name is required'}), requests.codes.bad_request
    
    app_dir = os.path.dirname(os.path.abspath(__file__))
    grand_parent_dir = os.path.dirname(os.path.dirname(app_dir))
    cpp_exec_path = os.path.join(grand_parent_dir, 'run.sh')
    command_args = [cpp_exec_path, '--addCourse', course_name]
    try:
        if time_slot:
            command_args.append(time_slot)
        result = subprocess.run(command_args)
        if result.returncode != 0:
            return jsonify({'success': False, 'message': 'Failed to add course', 'details': result.stderr}), requests.codes.internal_server_error
        return jsonify({'success': True, 'message': 'Course added successfully!'})
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), requests.codes.internal_server_error


@app.route('/addTimeslot', methods=['POST'])
def add_time_slot():
    data = request.json
    ts = data.get(UniJsonKeys.TIME_SLOT, '')

    if not ts:
        return jsonify({'success': False, 'message': 'Time slot is required'}), requests.codes.bad_request

    app_dir = os.path.dirname(os.path.abspath(__file__))
    grand_parent_dir = os.path.dirname(os.path.dirname(app_dir))
    cpp_exec_path = os.path.join(grand_parent_dir, 'run.sh')
    try:
        result = subprocess.run([cpp_exec_path, '--addTimeslot', ts])
        if result.returncode != 0:
            return jsonify({'success': False, 'message': 'Failed to add time slot', 'details': result.stderr}), requests.codes.internal_server_error
        return jsonify({'success': True, 'message': 'Time slot added successfully!'})
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), requests.codes.internal_server_error
    
@app.route('/addInstructor', methods=['POST'])
def add_instructor():
    data = request.json
    inst = data.get(UniJsonKeys.INSTRUCTOR, '')
    course = data.get(UniJsonKeys.COURSE_NAME_INST, '')
    time  = data.get(UniJsonKeys.TIME_SLOT_INST, '')

    if not inst:
        return jsonify({'success': False, 'message': 'Instructor is required'}), requests.codes.bad_request

    app_dir = os.path.dirname(os.path.abspath(__file__))
    grand_parent_dir = os.path.dirname(os.path.dirname(app_dir))
    cpp_exec_path = os.path.join(grand_parent_dir, 'run.sh')
    command_args = [cpp_exec_path, '--addInstructor', inst]
    try:
        if course:
            command_args.append(course)
        if not course and time:
            return jsonify({'success': False, 'message': 'If time is inputed course is mandatory'}), requests.codes.bad_request
        if time:
            command_args.append(time)
        result = subprocess.run(command_args)
        if result.returncode != 0:
            return jsonify({'success': False, 'message': 'Failed to add instructor', 'details': result.stderr}), requests.codes.internal_server_error
        return jsonify({'success': True, 'message': 'Instructor added successfully!'})
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), requests.codes.internal_server_error

@app.route('/schedule', methods=['GET'])
def schedule():
    app_dir = os.path.dirname(os.path.abspath(__file__))
    grand_parent_dir = os.path.dirname(os.path.dirname(app_dir))
    cpp_exec_path = os.path.join(grand_parent_dir, 'run.sh')

    try:
        result = subprocess.run([cpp_exec_path, '--schedule'])
        if result.returncode != 0:
            return jsonify({'success': False, 'message': 'Failed to execute algorithm', 'details': result.stderr}), requests.codes.internal_server_error
        return jsonify({'success': True, 'message': 'Algorithm executed successfully!', 'output': result.stdout})
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), requests.codes.internal_server_error

if __name__ == '__main__':
    app.run(debug=True)