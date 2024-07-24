To install the project clone the repo
```
git clone url_of_repo
cd University-Timetabling-System
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
and then build the project
```
make
```
after building, the dynamic library (libUniversityTimetabling.so) and the executable (main) will be placed in the build/dynlib and build/output directories, respectively.
to run the project, use the following command
```
./output/main
```
I added argument parsers which are

--addInstructor

for this parser you can give one, two or three argumets respectively name of the instructor, name of the course, preffered time which should be like this "Monday 12:50 14:25"

Ex. 
```
./output/main --addInstructor "John B" "Intro to ML" "Monday 12:50 14:25"
./output/main --addInstructor "John B"
./output/main --addInstructor "John B" "Intro to ML"
```
--addCourse

for this parser you can give one or two argumemts respectively name of the course,  preffered time which should be like this "Monday 12:50 14:25"

Ex. 
```
./output/main --addCourse "Intro to ML" "Monday 12:50 14:25"
./output/main --addCourse "Intro to ML"
```

--addTimeslot

for this parser you need to give three arguments respectively day, start time and end time

Ex. 
```
./output/main --addTimeslot "Monday" "12:50" "14:25"
```

--scedule

which is for calling scheduling algorithm for inputed data
