import psycopg2
import traceback

def connect_to_db():
    try:
        connection = psycopg2.connect(
            host="localhost",
            database="university_db",
            user="postgres",
            password="pwd123"
        )
        return connection
    except Exception as error:
        print("Error connecting to database:", error)
        return None
    
def insert_time_slot(day, start_time, end_time):
    connection = connect_to_db()
    if connection is None:
        raise Exception("Failed to connect to database")
    
    try:
        cursor = connection.cursor()
        query = """
            INSERT INTO time_slot (day, start_time, end_time)
            VALUES (%s, %s, %s)
        """
        print(f"Executing SQL: {query} with values: {day}, {start_time}, {end_time}")
        cursor.execute(query, (day, start_time, end_time))
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
        connection.close()

def insert_course(name, day=None, start_time=None, end_time=None):
    connection = connect_to_db()
    if connection is None:
        raise Exception ("Failed to connect to db")
    
    try:
        cursor = connection.cursor()
        if day==None and start_time==None and end_time==None:
            query_only_by_name = """
                INSERT INTO course (course_name) 
                VALUES (%s)
            """
            print(f"Executing SQL: {query_only_by_name} with value: {name}")
            cursor.execute(query_only_by_name, (name,))
        else:
            query_provided_with_time = """
                INSERT INTO course (course_name, day, start_time, end_time) 
                VALUES (%s, %s, %s, %s)
            """
            print(f"Executing SQL: {query_provided_with_time} with values: {name}, {day}, {start_time}, {end_time}")
            cursor.execute(query_provided_with_time, (name, day, start_time, end_time))
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
        connection.close()

def insert_instructor(instructor_name, course_name=None, day=None, start_time=None, end_time=None):
    connection = connect_to_db()
    if connection is None:
        raise Exception ("Failed to connect to db")

    try:
        cursor = connection.cursor()
        if day==None and start_time==None and end_time==None:
            if course_name==None:
                query_by_instructor_name = """
                    INSERT INTO instructor (name)
                    VALUES (%s) 
                """
                print(f"Executing SQL: {query_by_instructor_name} with value: {instructor_name}")
                cursor.execute(query_by_instructor_name, (instructor_name,))
            else:
                query_by_instructor_name_and_course = """
                    INSERT INTO instructor (name, course_name)
                    VALUES (%s, %s)
                """
                print(f"Executing SQL: {query_by_instructor_name_and_course} with value: {instructor_name}, {course_name}")
                cursor.execute(query_by_instructor_name_and_course, (instructor_name, course_name))
        else:
            query_by_instructor_course_timeslot = """
                INSERT INTO instructor (name, course_name, day, start_time, end_time)
                VALUES (%s, %s, %s, %s, %s)
            """
            print(f"Executing SQL: {query_by_instructor_course_timeslot} with value: {instructor_name}, {course_name}, {day}, {start_time}, {end_time}")
            cursor.execute(query_by_instructor_course_timeslot, (instructor_name, course_name, day, start_time, end_time))
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
        connection.close()
