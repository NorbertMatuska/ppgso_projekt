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
CMAKE_SOURCE_DIR = D:\PROJEKT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\PROJEKT\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/raw3_raytrace.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/raw3_raytrace.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/raw3_raytrace.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/raw3_raytrace.dir/flags.make

CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.obj: CMakeFiles/raw3_raytrace.dir/flags.make
CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.obj: CMakeFiles/raw3_raytrace.dir/includes_CXX.rsp
CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.obj: D:/PROJEKT/src/raw3_raytrace/raw3_raytrace.cpp
CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.obj: CMakeFiles/raw3_raytrace.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\PROJEKT\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.obj"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.obj -MF CMakeFiles\raw3_raytrace.dir\src\raw3_raytrace\raw3_raytrace.cpp.obj.d -o CMakeFiles\raw3_raytrace.dir\src\raw3_raytrace\raw3_raytrace.cpp.obj -c D:\PROJEKT\src\raw3_raytrace\raw3_raytrace.cpp

CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.i"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\PROJEKT\src\raw3_raytrace\raw3_raytrace.cpp > CMakeFiles\raw3_raytrace.dir\src\raw3_raytrace\raw3_raytrace.cpp.i

CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.s"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\PROJEKT\src\raw3_raytrace\raw3_raytrace.cpp -o CMakeFiles\raw3_raytrace.dir\src\raw3_raytrace\raw3_raytrace.cpp.s

# Object files for target raw3_raytrace
raw3_raytrace_OBJECTS = \
"CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.obj"

# External object files for target raw3_raytrace
raw3_raytrace_EXTERNAL_OBJECTS =

raw3_raytrace.exe: CMakeFiles/raw3_raytrace.dir/src/raw3_raytrace/raw3_raytrace.cpp.obj
raw3_raytrace.exe: CMakeFiles/raw3_raytrace.dir/build.make
raw3_raytrace.exe: libppgso.a
raw3_raytrace.exe: D:/PROJEKT/dependencies/lib/mingw/libglfw3.a
raw3_raytrace.exe: D:/PROJEKT/dependencies/lib/mingw/libglew32.a
raw3_raytrace.exe: D:/PROJEKT/dependencies/lib/mingw/libassimp.a
raw3_raytrace.exe: CMakeFiles/raw3_raytrace.dir/linkLibs.rsp
raw3_raytrace.exe: CMakeFiles/raw3_raytrace.dir/objects1.rsp
raw3_raytrace.exe: CMakeFiles/raw3_raytrace.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\PROJEKT\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable raw3_raytrace.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\raw3_raytrace.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/raw3_raytrace.dir/build: raw3_raytrace.exe
.PHONY : CMakeFiles/raw3_raytrace.dir/build

CMakeFiles/raw3_raytrace.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\raw3_raytrace.dir\cmake_clean.cmake
.PHONY : CMakeFiles/raw3_raytrace.dir/clean

CMakeFiles/raw3_raytrace.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\PROJEKT D:\PROJEKT D:\PROJEKT\cmake-build-debug D:\PROJEKT\cmake-build-debug D:\PROJEKT\cmake-build-debug\CMakeFiles\raw3_raytrace.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/raw3_raytrace.dir/depend

