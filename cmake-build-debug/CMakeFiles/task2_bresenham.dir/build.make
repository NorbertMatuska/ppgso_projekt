# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.29

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\CLion\CLion 2024.2.1\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "D:\CLion\CLion 2024.2.1\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\ppgso_projekt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\ppgso_projekt\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/task2_bresenham.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/task2_bresenham.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/task2_bresenham.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/task2_bresenham.dir/flags.make

CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.obj: CMakeFiles/task2_bresenham.dir/flags.make
CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.obj: CMakeFiles/task2_bresenham.dir/includes_CXX.rsp
CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.obj: D:/ppgso_projekt/src/task2_bresenham/task2_bresenham.cpp
CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.obj: CMakeFiles/task2_bresenham.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\ppgso_projekt\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.obj"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.obj -MF CMakeFiles\task2_bresenham.dir\src\task2_bresenham\task2_bresenham.cpp.obj.d -o CMakeFiles\task2_bresenham.dir\src\task2_bresenham\task2_bresenham.cpp.obj -c D:\ppgso_projekt\src\task2_bresenham\task2_bresenham.cpp

CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.i"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\ppgso_projekt\src\task2_bresenham\task2_bresenham.cpp > CMakeFiles\task2_bresenham.dir\src\task2_bresenham\task2_bresenham.cpp.i

CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.s"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\ppgso_projekt\src\task2_bresenham\task2_bresenham.cpp -o CMakeFiles\task2_bresenham.dir\src\task2_bresenham\task2_bresenham.cpp.s

# Object files for target task2_bresenham
task2_bresenham_OBJECTS = \
"CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.obj"

# External object files for target task2_bresenham
task2_bresenham_EXTERNAL_OBJECTS =

task2_bresenham.exe: CMakeFiles/task2_bresenham.dir/src/task2_bresenham/task2_bresenham.cpp.obj
task2_bresenham.exe: CMakeFiles/task2_bresenham.dir/build.make
task2_bresenham.exe: libppgso.a
task2_bresenham.exe: libshaders.a
task2_bresenham.exe: D:/ppgso_projekt/dependencies/lib/mingw/libglfw3.a
task2_bresenham.exe: D:/ppgso_projekt/dependencies/lib/mingw/libglew32.a
task2_bresenham.exe: D:/ppgso_projekt/dependencies/lib/mingw/libassimp.a
task2_bresenham.exe: CMakeFiles/task2_bresenham.dir/linkLibs.rsp
task2_bresenham.exe: CMakeFiles/task2_bresenham.dir/objects1.rsp
task2_bresenham.exe: CMakeFiles/task2_bresenham.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\ppgso_projekt\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable task2_bresenham.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\task2_bresenham.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/task2_bresenham.dir/build: task2_bresenham.exe
.PHONY : CMakeFiles/task2_bresenham.dir/build

CMakeFiles/task2_bresenham.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\task2_bresenham.dir\cmake_clean.cmake
.PHONY : CMakeFiles/task2_bresenham.dir/clean

CMakeFiles/task2_bresenham.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\ppgso_projekt D:\ppgso_projekt D:\ppgso_projekt\cmake-build-debug D:\ppgso_projekt\cmake-build-debug D:\ppgso_projekt\cmake-build-debug\CMakeFiles\task2_bresenham.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/task2_bresenham.dir/depend
