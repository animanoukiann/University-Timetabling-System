To install the project clone the repo then change the branch
```
git clone url_of_repo
cd University-Timetabling-System
git submodule update --init --recursive
```

after that you need to create a build directory
```
mkdir build
cd build
```
then generate build files with CMake
```
cmake ..
```
then build the project
```
make
```
and then install the project 
```
make install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/the/project
```

after installing, the dynamic library (libUniversityTimetabling.so) and the executable (main) will be placed in the root directory of the project, to run the project, use the this command from root directory with which you can add following arguments
```
./main
```

--addInstructor

for this argument you can give one, two or three argumets respectively name of the instructor, name of the course, preffered time which should be like this "Monday 12:50 14:25"

Ex. 
```
./main --addInstructor "John B" "Intro to ML" "Monday 12:50 14:25"
./main --addInstructor "John B"
./main --addInstructor "John B" "Intro to ML"
```
--addCourse

for this argument you can give one or two parametrs respectively name of the course,  preffered time which should be like this "Monday 12:50 14:25"

Ex. 
```
./main --addCourse "Intro to ML" "Monday 12:50 14:25"
./main --addCourse "Intro to ML"
```

--addTimeslot

for this argument you need to give only one argument seperated by space respectively day, start time and end time

Ex. 
```
./main --addTimeslot "Monday 12:50 14:25"
```

--schedule

for this argument you don't need to give any parametrs and it's calling the scheduling algorithm for inputed data

if you are not comfortable working with console there is a GUI for you here is how to work with it first of all you need to do the same things mentioned above, after that go to University-Timetabling-System/UI/backend and run these commands first create a virtual environment and activate it, then install Python dependencies and run the Flask application

```
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python3 app.py
```
after that open a new terminal and go to University-Timetabling-System/UI/frontend/university-timetable and run these commands first install Node.js dependencies then start the development server:

```
npm install
npm run serve
```

now if you open your browser then go to this link http://127.0.0.1:8080/ you will see all options mentiones bellow(addCourse, addInstructor, addTimeslot, schedule) and after inputing some data into one of them you can see result.json file updated and filled with that new inputed data