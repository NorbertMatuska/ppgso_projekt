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
include CMakeFiles/task6_bezier_surface.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/task6_bezier_surface.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/task6_bezier_surface.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/task6_bezier_surface.dir/flags.make

CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.obj: CMakeFiles/task6_bezier_surface.dir/flags.make
CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.obj: CMakeFiles/task6_bezier_surface.dir/includes_CXX.rsp
CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.obj: D:/ppgso_projekt/src/task6_bezier_surface/task6_bezier_surface.cpp
CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.obj: CMakeFiles/task6_bezier_surface.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=D:\ppgso_projekt\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.obj"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.obj -MF CMakeFiles\task6_bezier_surface.dir\src\task6_bezier_surface\task6_bezier_surface.cpp.obj.d -o CMakeFiles\task6_bezier_surface.dir\src\task6_bezier_surface\task6_bezier_surface.cpp.obj -c D:\ppgso_projekt\src\task6_bezier_surface\task6_bezier_surface.cpp

CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.i"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\ppgso_projekt\src\task6_bezier_surface\task6_bezier_surface.cpp > CMakeFiles\task6_bezier_surface.dir\src\task6_bezier_surface\task6_bezier_surface.cpp.i

CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.s"
	"D:\CLion\CLion 2024.2.1\bin\mingw\bin\g++.exe" $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\ppgso_projekt\src\task6_bezier_surface\task6_bezier_surface.cpp -o CMakeFiles\task6_bezier_surface.dir\src\task6_bezier_surface\task6_bezier_surface.cpp.s

# Object files for target task6_bezier_surface
task6_bezier_surface_OBJECTS = \
"CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.obj"

# External object files for target task6_bezier_surface
task6_bezier_surface_EXTERNAL_OBJECTS =

task6_bezier_surface.exe: CMakeFiles/task6_bezier_surface.dir/src/task6_bezier_surface/task6_bezier_surface.cpp.obj
task6_bezier_surface.exe: CMakeFiles/task6_bezier_surface.dir/build.make
task6_bezier_surface.exe: libppgso.a
task6_bezier_surface.exe: libshaders.a
task6_bezier_surface.exe: D:/ppgso_projekt/dependencies/lib/mingw/libglfw3.a
task6_bezier_surface.exe: D:/ppgso_projekt/dependencies/lib/mingw/libglew32.a
task6_bezier_surface.exe: D:/ppgso_projekt/dependencies/lib/mingw/libassimp.a
task6_bezier_surface.exe: CMakeFiles/task6_bezier_surface.dir/linkLibs.rsp
task6_bezier_surface.exe: CMakeFiles/task6_bezier_surface.dir/objects1.rsp
task6_bezier_surface.exe: CMakeFiles/task6_bezier_surface.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=D:\ppgso_projekt\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable task6_bezier_surface.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\task6_bezier_surface.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/task6_bezier_surface.dir/build: task6_bezier_surface.exe
.PHONY : CMakeFiles/task6_bezier_surface.dir/build

CMakeFiles/task6_bezier_surface.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\task6_bezier_surface.dir\cmake_clean.cmake
.PHONY : CMakeFiles/task6_bezier_surface.dir/clean

CMakeFiles/task6_bezier_surface.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\ppgso_projekt D:\ppgso_projekt D:\ppgso_projekt\cmake-build-debug D:\ppgso_projekt\cmake-build-debug D:\ppgso_projekt\cmake-build-debug\CMakeFiles\task6_bezier_surface.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/task6_bezier_surface.dir/depend

