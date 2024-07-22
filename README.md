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
