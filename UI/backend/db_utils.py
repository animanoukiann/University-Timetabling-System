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
