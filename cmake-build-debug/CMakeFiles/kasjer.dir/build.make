# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_SOURCE_DIR = /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/kasjer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/kasjer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/kasjer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kasjer.dir/flags.make

CMakeFiles/kasjer.dir/codegen:
.PHONY : CMakeFiles/kasjer.dir/codegen

CMakeFiles/kasjer.dir/kasjer.cpp.o: CMakeFiles/kasjer.dir/flags.make
CMakeFiles/kasjer.dir/kasjer.cpp.o: /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/kasjer.cpp
CMakeFiles/kasjer.dir/kasjer.cpp.o: CMakeFiles/kasjer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/kasjer.dir/kasjer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/kasjer.dir/kasjer.cpp.o -MF CMakeFiles/kasjer.dir/kasjer.cpp.o.d -o CMakeFiles/kasjer.dir/kasjer.cpp.o -c /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/kasjer.cpp

CMakeFiles/kasjer.dir/kasjer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/kasjer.dir/kasjer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/kasjer.cpp > CMakeFiles/kasjer.dir/kasjer.cpp.i

CMakeFiles/kasjer.dir/kasjer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/kasjer.dir/kasjer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/kasjer.cpp -o CMakeFiles/kasjer.dir/kasjer.cpp.s

# Object files for target kasjer
kasjer_OBJECTS = \
"CMakeFiles/kasjer.dir/kasjer.cpp.o"

# External object files for target kasjer
kasjer_EXTERNAL_OBJECTS =

kasjer: CMakeFiles/kasjer.dir/kasjer.cpp.o
kasjer: CMakeFiles/kasjer.dir/build.make
kasjer: CMakeFiles/kasjer.dir/compiler_depend.ts
kasjer: CMakeFiles/kasjer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable kasjer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kasjer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kasjer.dir/build: kasjer
.PHONY : CMakeFiles/kasjer.dir/build

CMakeFiles/kasjer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kasjer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kasjer.dir/clean

CMakeFiles/kasjer.dir/depend:
	cd /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16 /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16 /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug/CMakeFiles/kasjer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/kasjer.dir/depend

