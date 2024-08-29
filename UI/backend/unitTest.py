import unittest
import json
from http.client import responses
from unittest.mock import MagicMock, patch
import app
import dbUtils
from dbUtils import insertTimeSlot, insertCourse, insertInstructor, connectDB


class UnitTestApp(unittest.TestCase):
    def setUp(self):
        self.app = app.app.test_client()
        self.app.testing = True

    def tearDown(self):
        app.connection = connectDB()
        dbUtils.connection = connectDB()

    def testAddCourseSuccess(self):
        mock = MagicMock()
        mock.insertCourse()
        mock.tsProvided()
        response = self.app.post('/addCourse', data = json.dumps(dict(courseName = 'Math', \
            timeSlotCourse = 'Monday 12:00 13:00')), content_type='application/json')
        mock.insertCourse.assert_called_once()
        mock.tsProvided.assert_called_once()
        self.assertEqual(response.status_code, 200)

    @patch('app.insertCourse')
    def testAddCourseFail(self, mock):
        mock.side_effect = Exception("Database insertion failed")
        response = self.app.post('/addCourse', data=json.dumps(dict(courseName='Math', \
            timeSlotCourse='Monday 12:00 13:00')), content_type='application/json')
        mock.assert_called_once()
        self.assertEqual(response.status_code, 500)
        response_data = json.loads(response.data.decode('utf-8'))
        self.assertFalse(response_data['success'])
        self.assertEqual(response_data['message'], "Database insertion failed")

    def testAddTimeSlotSuccess(self):
        mock = MagicMock()
        mock.insertTimeSlot()
        response = self.app.post('/addTimeslot', data = json.dumps(dict(timeSlot = 'Monday 12:00 13:00')),\
            content_type='application/json')
        mock.insertTimeSlot.assert_called_once()
        self.assertEqual(response.status_code, 200)

    @patch('app.insertTimeSlot')
    def testTimeSlotFail(self, mock):
        mock.side_effect = Exception("Database insertion failed")
        response = self.app.post('/addTimeslot', data = json.dumps(dict(timeSlot = 'Monday 12:00 13:00')),\
            content_type='application/json')
        mock.assert_called_once()
        self.assertEqual(response.status_code, 500)
        response_data = json.loads(response.data.decode('utf-8'))
        self.assertFalse(response_data['success'])
        self.assertEqual(response_data['message'], "Database insertion failed")
    
    def testAddInstructorSuccess(self):
        mock = MagicMock()
        mock.insertCourse()
        mock.tsProvided()
        mock.insertInstructor()
        response = self.app.post('/addInstructor', data = json.dumps(dict(instructor = 'Margaryan', \
            courseNameInst = 'Math', timeSlotInst = 'Monday 12:00 13:00')), content_type='application/json')
        mock.insertCourse.assert_called_once()
        mock.tsProvided.assert_called_once()
        mock.insertInstructor.assert_called_once()
        self.assertEqual(response.status_code, 200)
    
    @patch('app.insertInstructor')
    def testAddInstructorFail(self, mock):
        mock.side_effect = Exception("Database insertion failed")
        response = self.app.post('/addInstructor', data = json.dumps(dict(instructor = 'Margaryan', \
            courseNameInst = 'Math', timeSlotInst = 'Monday 12:00 13:00')), content_type='application/json')
        mock.assert_called_once()
        self.assertEqual(response.status_code, 500)
        response_data = json.loads(response.data.decode('utf-8'))
        self.assertFalse(response_data['success'])
        self.assertEqual(response_data['message'], "Database insertion failed")

    def testScheduleSuccess(self):
        mock = MagicMock()
        mock.cursor()
        mock.close()
        response = self.app.get('/schedule')
        mock.cursor.assert_called_once()
        mock.close.assert_called_once()
        self.assertEqual(response.status_code, 200)

    def testScheduleFail(self):
        mockExec = MagicMock()
        mockExec.execute.side_effect=Exception("Query execution failed")
        mockCursor = MagicMock()
        mockCursor.cursor.return_value = mockExec
        app.connection = mockCursor
        response = self.app.get('/schedule')
        self.assertEqual(response.status_code, 500)
        response_data = json.loads(response.data.decode('utf-8'))
        self.assertFalse(response_data['success'])
        self.assertEqual(response_data['message'], "Query execution failed")

    def testInsertTimeSlotSuccess(self):
        mock = MagicMock()
        mock.close()
        mock.connection.commit()
        day = 'Monday'
        startTime = '09:00'
        endTime = '10:00'
        insertTimeSlot(day, startTime, endTime)
        mock.connection.cursor().execute(
            """
            INSERT INTO time_slot (day, startTime, endTime)
            VALUES (%s, %s, %s)
            """,
            (day, startTime, endTime)
        )
        mock.connection.commit.assert_called_once()
        mock.close.assert_called_once()
        mock.connection.rollback.assert_not_called()

    def testInsertTimeSlotFail(self):
        mock_cursor = MagicMock()
        mock_connection = MagicMock()
        mock_cursor.execute.side_effect = Exception("Database execution failed")
        mock_connection.cursor.return_value = mock_cursor
        dbUtils.connection = mock_connection
        day = 'Monday'
        startTime = '09:00'
        endTime = '10:00'
        with self.assertRaises(Exception) as context:
            dbUtils.insertTimeSlot(day, startTime, endTime)
        mock_connection.rollback.assert_called_once()
        self.assertEqual(str(context.exception), "Database execution failed")
        mock_cursor.close.assert_called_once()

    def testInsertCourseSuccess(self):
        mock = MagicMock()
        mock.close()
        mock.connection.commit()
        day = 'Monday'
        startTime = '12:00'
        endTime = '13:00'
        course = 'Math'
        insertCourse(course, day, startTime, endTime)
        mock.connection.cursor().execute(
            """
            INSERT INTO course (course_name, course_day, course_start_time, course_end_time) 
            VALUES (%s, %s, %s, %s)
            """,
            (course, day, startTime, endTime)
        )
        mock.connection.commit.assert_called_once()
        mock.close.assert_called_once()
        mock.connection.rollback.assert_not_called()

    def testInsertCourseFail(self):
        mock_cursor = MagicMock()
        mock_connection = MagicMock()
        mock_cursor.execute.side_effect = Exception("Database execution failed")
        mock_connection.cursor.return_value = mock_cursor
        dbUtils.connection = mock_connection
        day = 'Monday'
        startTime = '09:00'
        endTime = '10:00'
        courseName = 'Math'
        with self.assertRaises(Exception) as context:
            dbUtils.insertCourse(courseName, day, startTime, endTime)
        mock_connection.rollback.assert_called_once()
        self.assertEqual(str(context.exception), "Database execution failed")
        mock_cursor.close.assert_called_once()

    def testInsertInstructorSuccess(self):
        mock = MagicMock()
        mock.close()
        mock.connection.commit()
        day = 'Monday'
        startTime = '12:00'
        endTime = '13:00'
        course = 'Math'
        instructor = 'Margaryan'
        insertInstructor(instructor, course, day, startTime, endTime)
        mock.connection.cursor().execute(
             """
            INSERT INTO instructor (instructor_name, inst_course_name,
            inst_day, inst_start_time, inst_end_time) VALUES (%s, %s, %s, %s, %s)
            """,
            (instructor, course, day, startTime, endTime)
        )
        mock.connection.commit.assert_called_once()
        mock.close.assert_called_once()
        mock.connection.rollback.assert_not_called()

    def testInsertInstructorFail(self):
        mock_cursor = MagicMock()
        mock_connection = MagicMock()
        mock_cursor.execute.side_effect = Exception("Database execution failed")
        mock_connection.cursor.return_value = mock_cursor
        dbUtils.connection = mock_connection
        day = 'Monday'
        startTime = '09:00'
        endTime = '10:00'
        courseName = 'Math'
        instructor = 'Margaryan'
        with self.assertRaises(Exception) as context:
            dbUtils.insertInstructor(instructor, courseName, day, startTime, endTime)
        mock_connection.rollback.assert_called_once()
        self.assertEqual(str(context.exception), "Database execution failed")
        mock_cursor.close.assert_called_once()


if __name__ == '__main__':
    unittest.main()
