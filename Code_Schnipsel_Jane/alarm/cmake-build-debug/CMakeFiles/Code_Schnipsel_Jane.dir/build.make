# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.UxLyZKHMGR/alarm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.UxLyZKHMGR/alarm/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Code_Schnipsel_Jane.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Code_Schnipsel_Jane.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Code_Schnipsel_Jane.dir/flags.make

CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.o: CMakeFiles/Code_Schnipsel_Jane.dir/flags.make
CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.o: /tmp/tmp.UxLyZKHMGR/alarm/alarm.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.UxLyZKHMGR/alarm/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.o -c /tmp/tmp.UxLyZKHMGR/alarm/alarm.c

CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.UxLyZKHMGR/alarm/alarm.c > CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.i

CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.UxLyZKHMGR/alarm/alarm.c -o CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.s

# Object files for target Code_Schnipsel_Jane
Code_Schnipsel_Jane_OBJECTS = \
"CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.o"

# External object files for target Code_Schnipsel_Jane
Code_Schnipsel_Jane_EXTERNAL_OBJECTS =

Code_Schnipsel_Jane: CMakeFiles/Code_Schnipsel_Jane.dir/alarm.c.o
Code_Schnipsel_Jane: CMakeFiles/Code_Schnipsel_Jane.dir/build.make
Code_Schnipsel_Jane: /usr/lib/x86_64-linux-gnu/librt.so
Code_Schnipsel_Jane: CMakeFiles/Code_Schnipsel_Jane.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.UxLyZKHMGR/alarm/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Code_Schnipsel_Jane"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Code_Schnipsel_Jane.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Code_Schnipsel_Jane.dir/build: Code_Schnipsel_Jane
.PHONY : CMakeFiles/Code_Schnipsel_Jane.dir/build

CMakeFiles/Code_Schnipsel_Jane.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Code_Schnipsel_Jane.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Code_Schnipsel_Jane.dir/clean

CMakeFiles/Code_Schnipsel_Jane.dir/depend:
	cd /tmp/tmp.UxLyZKHMGR/alarm/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.UxLyZKHMGR/alarm /tmp/tmp.UxLyZKHMGR/alarm /tmp/tmp.UxLyZKHMGR/alarm/cmake-build-debug /tmp/tmp.UxLyZKHMGR/alarm/cmake-build-debug /tmp/tmp.UxLyZKHMGR/alarm/cmake-build-debug/CMakeFiles/Code_Schnipsel_Jane.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Code_Schnipsel_Jane.dir/depend

