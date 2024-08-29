import pytest
import app
import datetime
from app import app, connection

@pytest.fixture
def client():
    app.config['TESTING'] = True
    with app.test_client() as client:
        yield client

def testAddTimeSlotSuccess(client):
    response = client.post('/addTimeslot', json={'timeSlot': 'Tuesday 14:00 16:00'})
    assert response.status_code == 200
    assert response.json['success'] is True
    assert response.json['message'] == 'Time slot added successfully!'

    cur = connection.cursor()
    cur.execute("SELECT * FROM time_slot WHERE day = 'Tuesday'")
    result = cur.fetchone()
    cur.close()
    assert result is not None
    assert result[1] == 'Tuesday'
    assert result[2] == datetime.time(14, 0)
    assert result[3] == datetime.time(16, 0)

def testAddTimeSlotFail(client):
    response = client.post('/addTimeslot', json={'timeSlot': 'InvalidFormat'})
    assert response.status_code == 400
    assert response.json['success'] is False
    assert response.json['message'] == 'Invalid time slot format'

def testAddCourseSuccess(client):
    response = client.post('/addCourse', json={'courseName': 'Math'})
    assert response.status_code == 200
    assert response.json['success'] is True
    assert response.json['message'] == 'Course added successfully!'

    cur = connection.cursor()
    cur.execute("SELECT * FROM course WHERE course_name = 'Math'")
    result = cur.fetchone()
    cur.close()
    assert result is not None
    assert result[1] == 'Math'

    response = client.post('/addCourse', json={
        'courseName': 'Physics',
        'timeSlotCourse': 'Monday 10:00 12:00'
    })
    assert response.status_code == 200
    assert response.json['success'] is True
    assert response.json['message'] == 'Course added successfully!'

    cur = connection.cursor()
    cur.execute("SELECT * FROM course WHERE course_name = 'Physics'")
    result = cur.fetchone()
    cur.close()
    assert result is not None
    assert result[1] == 'Physics'

def testAddCourseFail(client):
    response = client.post('/addCourse', json={
        'courseName': 'Math',
        'timeSlotCourse': 'InvalidFormat'
    })
    assert response.status_code == 400
    assert response.json['success'] is False
    assert response.json['message'] == 'Invalid time slot format' 

def testAddInstructorSuccess(client):
    response = client.post('/addInstructor', json={'instructor': 'Abovyan'})
    assert response.status_code == 200
    assert response.json['success'] is True
    assert response.json['message'] == 'Instructor added successfully!'

    cur = connection.cursor()
    cur.execute("SELECT * FROM instructor WHERE instructor_name = 'Abovyan'")
    result = cur.fetchone()
    cur.close()
    assert result is not None
    assert result[1] == 'Abovyan'

    response = client.post('/addInstructor', json={
        'instructor': 'Hakobyan',
        'courseNameInst': 'Math'
    })
    assert response.status_code == 200
    assert response.json['success'] is True
    assert response.json['message'] == 'Instructor added successfully!'

    cur = connection.cursor()
    cur.execute("SELECT * FROM instructor WHERE instructor_name = 'Hakobyan'")
    result = cur.fetchone()
    cur.close()
    assert result is not None
    assert result[1] == 'Hakobyan'

    response = client.post('/addInstructor', json={
        'instructor': 'Petrosyan',
        'courseNameInst': 'English',
        'timeSlotInst': 'Monday 10:00 12:00'
    })
    assert response.status_code == 200
    assert response.json['success'] is True
    assert response.json['message'] == 'Instructor added successfully!'

    cur = connection.cursor()
    cur.execute("SELECT * FROM instructor WHERE instructor_name = 'Petrosyan'")
    result = cur.fetchone()
    cur.close()
    assert result is not None
    assert result[1] == 'Petrosyan'

def testAddInstructorFail(client):
    response = client.post('/addInstructor', json={
        'instructor': 'Vardanyan',
        'courseNameInst': 'Math',
        'timeSlotInst': 'InvalidTimeFormat'
    })
    assert response.status_code == 400
    assert response.json['success'] is False
    assert response.json['message'] == 'Invalid time slot format' 

