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
CMAKE_SOURCE_DIR = D:\STU\4-1\PPGSO\ppgso_projekt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\STU\4-1\PPGSO\ppgso_projekt\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/raw2_raycast.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/raw2_raycast.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/raw2_raycast.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/raw2_raycast.dir/flags.make

CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.obj: CMakeFiles/raw2_raycast.dir/flags.make
CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.obj: CMakeFiles/raw2_raycast.dir/includes_CXX.rsp
CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.obj: D:/STU/4-1/PPGSO/ppgso_projekt/src/raw2_raycast/raw2_raycast.cpp
CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.obj: CMakeFiles/raw2_raycast.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\STU\4-1\PPGSO\ppgso_projekt\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.obj"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.obj -MF CMakeFiles\raw2_raycast.dir\src\raw2_raycast\raw2_raycast.cpp.obj.d -o CMakeFiles\raw2_raycast.dir\src\raw2_raycast\raw2_raycast.cpp.obj -c D:\STU\4-1\PPGSO\ppgso_projekt\src\raw2_raycast\raw2_raycast.cpp

CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.i"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\STU\4-1\PPGSO\ppgso_projekt\src\raw2_raycast\raw2_raycast.cpp > CMakeFiles\raw2_raycast.dir\src\raw2_raycast\raw2_raycast.cpp.i

CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.s"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\STU\4-1\PPGSO\ppgso_projekt\src\raw2_raycast\raw2_raycast.cpp -o CMakeFiles\raw2_raycast.dir\src\raw2_raycast\raw2_raycast.cpp.s

# Object files for target raw2_raycast
raw2_raycast_OBJECTS = \
"CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.obj"

# External object files for target raw2_raycast
raw2_raycast_EXTERNAL_OBJECTS =

raw2_raycast.exe: CMakeFiles/raw2_raycast.dir/src/raw2_raycast/raw2_raycast.cpp.obj
raw2_raycast.exe: CMakeFiles/raw2_raycast.dir/build.make
raw2_raycast.exe: libppgso.a
raw2_raycast.exe: D:/STU/4-1/PPGSO/ppgso_projekt/dependencies/lib/mingw/libglfw3.a
raw2_raycast.exe: D:/STU/4-1/PPGSO/ppgso_projekt/dependencies/lib/mingw/libglew32.a
raw2_raycast.exe: D:/STU/4-1/PPGSO/ppgso_projekt/dependencies/lib/mingw/libassimp.a
raw2_raycast.exe: CMakeFiles/raw2_raycast.dir/linkLibs.rsp
raw2_raycast.exe: CMakeFiles/raw2_raycast.dir/objects1.rsp
raw2_raycast.exe: CMakeFiles/raw2_raycast.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\STU\4-1\PPGSO\ppgso_projekt\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable raw2_raycast.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\raw2_raycast.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/raw2_raycast.dir/build: raw2_raycast.exe
.PHONY : CMakeFiles/raw2_raycast.dir/build

CMakeFiles/raw2_raycast.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\raw2_raycast.dir\cmake_clean.cmake
.PHONY : CMakeFiles/raw2_raycast.dir/clean

CMakeFiles/raw2_raycast.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\STU\4-1\PPGSO\ppgso_projekt D:\STU\4-1\PPGSO\ppgso_projekt D:\STU\4-1\PPGSO\ppgso_projekt\cmake-build-debug D:\STU\4-1\PPGSO\ppgso_projekt\cmake-build-debug D:\STU\4-1\PPGSO\ppgso_projekt\cmake-build-debug\CMakeFiles\raw2_raycast.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/raw2_raycast.dir/depend

