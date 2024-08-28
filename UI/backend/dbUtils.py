import psycopg2
import traceback
import json
import os
from constants import ConnectParametrs

def connectDB():
    appDir = os.path.dirname(os.path.abspath(__file__))
    grandParentDir = os.path.dirname(os.path.dirname(appDir))
    configPath = os.path.join(grandParentDir, 'config.json')
    with open(configPath, 'r') as jsonFile:
      data = json.load(jsonFile)
    try:
        connection = psycopg2.connect(
            host= data[ConnectParametrs.host],
            database= data[ConnectParametrs.db],
            user= data[ConnectParametrs.user],
            password= data[ConnectParametrs.password]
        )
        return connection
    except Exception as error:
        print("Error connecting to database:", error)
        return None

connection = connectDB()
    
def insertTimeSlot(day, startTime, endTime):
    try:
        cursor = connection.cursor()
        query = """
            INSERT INTO time_slot (day, start_time, end_time)
            VALUES (%s, %s, %s)
        """
        print(f"Executing SQL: {query} with values: {day}, {startTime}, {endTime}")
        cursor.execute(query, (day, startTime, endTime))
        connection.commit()
        cursor.close()
        print("Time slot inserted successfully.")
    except Exception as e:
        connection.rollback()
        print("Error occurred during database insertion:")
        print(f"Exception: {str(e)}")
        traceback.print_exc()
        raise e
    finally:
        if cursor:
            cursor.close()

def insertCourse(name, day=None, startTime=None, endTime=None):
    try:
        cursor = connection.cursor()
        if day==None and startTime==None and endTime==None:
            queryOnlyByName = """
                INSERT INTO course (course_name) 
                VALUES (%s)
            """
            print(f"Executing SQL: {queryOnlyByName} with value: {name}")
            cursor.execute(queryOnlyByName, (name,))
        else:
            queryProvidedWithTime = """
                INSERT INTO course (course_name, course_day, course_start_time, course_end_time) 
                VALUES (%s, %s, %s, %s)
            """
            print(f"Executing SQL: {queryProvidedWithTime} with values: "
                  f"{name}, {day}, {startTime}, {endTime}")
            cursor.execute(queryProvidedWithTime, (name, day, startTime, endTime))
        connection.commit()
        cursor.close()
        print("Course inserted successfully.")
    except Exception as e:
        connection.rollback()
        print("Error occurred during database insertion:")
        print(f"Exception: {str(e)}")
        traceback.print_exc()
        raise e
    finally:
        if cursor:
            cursor.close()

def insertInstructor(instructorName, courseName=None, day=None, startTime=None, endTime=None):
    try:
        cursor = connection.cursor()
        if day==None and startTime==None and endTime==None:
            if courseName==None:
                queryByInstructorName = """
                    INSERT INTO instructor (instructor_name)
                    VALUES (%s) 
                """
                print(f"Executing SQL: {queryByInstructorName} with value: {instructorName}")
                cursor.execute(queryByInstructorName, (instructorName,))
            else:
                queryByInstructorNameAndCourse = """
                    INSERT INTO instructor (instructor_name, inst_course_name)
                    VALUES (%s, %s)
                """
                print(f"Executing SQL: {queryByInstructorNameAndCourse} with value: "
                      f"{instructorName}, {courseName}")
                cursor.execute(queryByInstructorNameAndCourse, (instructorName, courseName))
        else:
            queryByInstructorCourseTimeslot = """
                INSERT INTO instructor (instructor_name, inst_course_name,
                inst_day, inst_start_time, inst_end_time) VALUES (%s, %s, %s, %s, %s)
            """
            print(f"Executing SQL: {queryByInstructorCourseTimeslot} with value: "
                  f"{instructorName}, {courseName}, {day}, {startTime}, {endTime}")
            cursor.execute(queryByInstructorCourseTimeslot, (instructorName, courseName, day,
                                                                  startTime, endTime))
        connection.commit()
        cursor.close()
        print("Instructor inserted successfully.")
    except Exception as e:
        connection.rollback()
        print("Error occurred during database insertion:")
        print(f"Exception: {str(e)}")
        traceback.print_exc()
        raise e
    finally:
        if cursor:
            cursor.close()
