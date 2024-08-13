To install the project clone the repo then change the branch
```
git clone url_of_repo
cd University-Timetabling-System
git submodule update --init --recursive
```

After that you need to create a build directory
```
mkdir build
cd build
```
Then generate build files with CMake
```
cmake ..
```
Then build the project
```
make
```
After that install the project
```
make install
```

After installing, the dynamic library (libUniversityTimetabling.so) and the executable (main) will be placed in the root directory of the project. To run the project, use the this command from root directory, with which you can add following arguments
```
chmod +x ./run.sh
/run.sh
```

--addInstructor

For this argument you can give one, two or three argumets respectively name of the instructor, name of the course, preffered time which should be like this "Monday 12:50 14:25"

Ex. 
```
/run.sh --addInstructor "John B" "Intro to ML" "Monday 12:50 14:25"
/run.sh --addInstructor "John B"
/run.sh --addInstructor "John B" "Intro to ML"
```
--addCourse

For this argument you can give one or two parametrs respectively name of the course, preffered time which should be like this "Monday 12:50 14:25"

Ex. 
```
/run.sh --addCourse "Intro to ML" "Monday 12:50 14:25"
/run.sh --addCourse "Intro to ML"
```

--addTimeslot

For this argument you need to give only one argument, seperated by space respectively day, start time and end time

Ex. 
```
/run.sh --addTimeslot "Monday 12:50 14:25"
```

--schedule

For this argument you don't need to give any parametrs cause it's for calling the scheduling algorithm for inputed data

If you are not comfortable working with console, there is a GUI for you and here is how to work with it. First of all you need to do the same things mentioned above, after that go to University-Timetabling-System/UI/backend and run these commands, which are doing the following things, first create a virtual environment and activate it, then install Python dependencies and run the Flask application

```
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python3 app.py
```
After that open a new terminal and go to University-Timetabling-System/UI/frontend/university-timetable and run these commands, which are doing the following things, first install Node.js dependencies then start the development server:

```
npm install
npm install @vue/cli-service --save-dev
npm run serve
```

Now if you open your browser and follow this link http://127.0.0.1:8080/ you will see all options mentioned above(addCourse, addInstructor, addTimeslot, schedule) and after inputing some data into one of them, you can see result.json file updated and filled with the new inputed data