# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_SOURCE_DIR = /home/chenyaxuan123/CYRBD

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chenyaxuan123/CYRBD/build

# Include any dependencies generated for this target.
include CMakeFiles/rbd.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/rbd.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/rbd.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rbd.dir/flags.make

CMakeFiles/rbd.dir/src/rbd.cpp.o: CMakeFiles/rbd.dir/flags.make
CMakeFiles/rbd.dir/src/rbd.cpp.o: /home/chenyaxuan123/CYRBD/src/rbd.cpp
CMakeFiles/rbd.dir/src/rbd.cpp.o: CMakeFiles/rbd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenyaxuan123/CYRBD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rbd.dir/src/rbd.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rbd.dir/src/rbd.cpp.o -MF CMakeFiles/rbd.dir/src/rbd.cpp.o.d -o CMakeFiles/rbd.dir/src/rbd.cpp.o -c /home/chenyaxuan123/CYRBD/src/rbd.cpp

CMakeFiles/rbd.dir/src/rbd.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rbd.dir/src/rbd.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenyaxuan123/CYRBD/src/rbd.cpp > CMakeFiles/rbd.dir/src/rbd.cpp.i

CMakeFiles/rbd.dir/src/rbd.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rbd.dir/src/rbd.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenyaxuan123/CYRBD/src/rbd.cpp -o CMakeFiles/rbd.dir/src/rbd.cpp.s

CMakeFiles/rbd.dir/src/utiliy.cpp.o: CMakeFiles/rbd.dir/flags.make
CMakeFiles/rbd.dir/src/utiliy.cpp.o: /home/chenyaxuan123/CYRBD/src/utiliy.cpp
CMakeFiles/rbd.dir/src/utiliy.cpp.o: CMakeFiles/rbd.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenyaxuan123/CYRBD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/rbd.dir/src/utiliy.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/rbd.dir/src/utiliy.cpp.o -MF CMakeFiles/rbd.dir/src/utiliy.cpp.o.d -o CMakeFiles/rbd.dir/src/utiliy.cpp.o -c /home/chenyaxuan123/CYRBD/src/utiliy.cpp

CMakeFiles/rbd.dir/src/utiliy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rbd.dir/src/utiliy.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenyaxuan123/CYRBD/src/utiliy.cpp > CMakeFiles/rbd.dir/src/utiliy.cpp.i

CMakeFiles/rbd.dir/src/utiliy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rbd.dir/src/utiliy.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenyaxuan123/CYRBD/src/utiliy.cpp -o CMakeFiles/rbd.dir/src/utiliy.cpp.s

# Object files for target rbd
rbd_OBJECTS = \
"CMakeFiles/rbd.dir/src/rbd.cpp.o" \
"CMakeFiles/rbd.dir/src/utiliy.cpp.o"

# External object files for target rbd
rbd_EXTERNAL_OBJECTS =

rbd: CMakeFiles/rbd.dir/src/rbd.cpp.o
rbd: CMakeFiles/rbd.dir/src/utiliy.cpp.o
rbd: CMakeFiles/rbd.dir/build.make
rbd: /usr/local/lib/libigraph.a
rbd: /usr/lib/x86_64-linux-gnu/libm.so
rbd: /usr/lib/gcc/x86_64-linux-gnu/9/libgomp.so
rbd: /usr/lib/x86_64-linux-gnu/libpthread.so
rbd: CMakeFiles/rbd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chenyaxuan123/CYRBD/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable rbd"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rbd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rbd.dir/build: rbd
.PHONY : CMakeFiles/rbd.dir/build

CMakeFiles/rbd.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rbd.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rbd.dir/clean

CMakeFiles/rbd.dir/depend:
	cd /home/chenyaxuan123/CYRBD/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chenyaxuan123/CYRBD /home/chenyaxuan123/CYRBD /home/chenyaxuan123/CYRBD/build /home/chenyaxuan123/CYRBD/build /home/chenyaxuan123/CYRBD/build/CMakeFiles/rbd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rbd.dir/depend

