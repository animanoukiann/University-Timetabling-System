import unittest
import json
from unittest.mock import MagicMock, patch
from app import app, add_course, add_instructor, add_time_slot, schedule
from dbUtils import insertTimeSlot, insertCourse, insertInstructor


class TestApp(unittest.TestCase):
    def setUp(self):
        self.app = app.test_client()
        self.app.testing = True

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

    # def testScheduleSuccess(self):
    #     mock = MagicMock()
    #     mock.cursor()
    #     mock.close()
    #     response = self.app.get('/schedule')
    #     mock.cursor.assert_called_once()
    #     mock.close.assert_called_once()
    #     self.assertEqual(response.status_code, 200)

    # @patch('app.connection.cursor.execute')
    # def testScheduleFail(self, mock):
    #     mock.side_effect = Exception("Query execution failed")
    #     response = self.app.get('/schedule')
    #     mock.assert_called_once()
    #     self.assertEqual(response.status_code, 500)
    #     response_data = json.loads(response.data.decode('utf-8'))
    #     self.assertFalse(response_data['success'])
    #     self.assertEqual(response_data['message'], "Query execution failed")

    def testInsertTimeSlotSuccess(self):
        mock = MagicMock()
        mock.close()
        mock.connection.commit()
        day = 'Monday'
        start_time = '09:00'
        end_time = '10:00'
        insertTimeSlot(day, start_time, end_time)
        mock.connection.cursor().execute(
            """
            INSERT INTO time_slot (day, start_time, end_time)
            VALUES (%s, %s, %s)
            """,
            (day, start_time, end_time)
        )
        mock.connection.commit.assert_called_once()
        mock.close.assert_called_once()
        mock.connection.rollback.assert_not_called()

if __name__ == '__main__':
    unittest.main()
