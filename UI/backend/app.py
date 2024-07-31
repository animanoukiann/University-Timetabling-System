from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess
import os

app = Flask(__name__)
CORS(app)

courses = []

@app.route('/addCourse', methods=['POST'])
def add_course():
    data = request.json
    course_name = data.get('courseName', '')

    if not course_name:
        return jsonify({'success': False, 'message': 'Course name is required'}), 400
    
    print(f"Received data: {data}")

    cpp_exec_path = os.path.join('..', '..', 'build', 'output', 'main')

    try:
        result = subprocess.run([cpp_exec_path, '--addCourse', course_name], capture_output=True, text=True)
        print(f"Subprocess result: {result.stdout}")
        print(f"Subprocess error: {result.stderr}")

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
        print(f"Subprocess result: {result.stdout}")
        print(f"Subprocess error: {result.stderr}")

        if result.returncode == 0:
            return jsonify({'success': True, 'message': 'Time slot added successfully!'})
        else:
            return jsonify({'success': False, 'message': 'Failed to add time slot', 'details': result.stderr}), 500
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), 500
    
@app.route('/addInstructor', methods=['POST'])
def add_instructor():
    data = request.json
    inst = data.get('instructor', '')

    if not inst:
        return jsonify({'success': False, 'message': 'Instructor is required'}), 400
    
    print(f"Received data: {data}")

    cpp_exec_path = os.path.join('..', '..', 'build', 'output', 'main')

    try:
        result = subprocess.run([cpp_exec_path, '--addInstructor', inst], capture_output=True, text=True)
        print(f"Subprocess result: {result.stdout}")
        print(f"Subprocess error: {result.stderr}")

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
        print(f"Subprocess result: {result.stdout}")
        print(f"Subprocess error: {result.stderr}")

        if result.returncode == 0:
            return jsonify({'success': True, 'message': 'Algorithm executed successfully!'})
        else:
            return jsonify({'success': False, 'message': 'Failed to execute algorithm', 'details': result.stderr}), 500
    except Exception as e:
        print(f"Exception occurred: {str(e)}")
        return jsonify({'success': False, 'message': str(e)}), 500


if __name__ == '__main__':
    app.run(debug=True)