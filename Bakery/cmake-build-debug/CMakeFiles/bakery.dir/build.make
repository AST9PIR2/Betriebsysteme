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
CMAKE_SOURCE_DIR = /tmp/bakery

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/bakery/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/bakery.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/bakery.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bakery.dir/flags.make

CMakeFiles/bakery.dir/main.c.o: CMakeFiles/bakery.dir/flags.make
CMakeFiles/bakery.dir/main.c.o: /tmp/bakery/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/bakery/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/bakery.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bakery.dir/main.c.o -c /tmp/bakery/main.c

CMakeFiles/bakery.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bakery.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/bakery/main.c > CMakeFiles/bakery.dir/main.c.i

CMakeFiles/bakery.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bakery.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/bakery/main.c -o CMakeFiles/bakery.dir/main.c.s

# Object files for target bakery
bakery_OBJECTS = \
"CMakeFiles/bakery.dir/main.c.o"

# External object files for target bakery
bakery_EXTERNAL_OBJECTS =

bakery: CMakeFiles/bakery.dir/main.c.o
bakery: CMakeFiles/bakery.dir/build.make
bakery: /usr/lib/x86_64-linux-gnu/librt.so
bakery: CMakeFiles/bakery.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/bakery/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bakery"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bakery.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bakery.dir/build: bakery
.PHONY : CMakeFiles/bakery.dir/build

CMakeFiles/bakery.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bakery.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bakery.dir/clean

CMakeFiles/bakery.dir/depend:
	cd /tmp/bakery/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/bakery /tmp/bakery /tmp/bakery/cmake-build-debug /tmp/bakery/cmake-build-debug /tmp/bakery/cmake-build-debug/CMakeFiles/bakery.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bakery.dir/depend
