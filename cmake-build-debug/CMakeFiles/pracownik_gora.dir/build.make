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
include CMakeFiles/pracownik_gora.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/pracownik_gora.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/pracownik_gora.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pracownik_gora.dir/flags.make

CMakeFiles/pracownik_gora.dir/codegen:
.PHONY : CMakeFiles/pracownik_gora.dir/codegen

CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.o: CMakeFiles/pracownik_gora.dir/flags.make
CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.o: /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/pracownik_gora.cpp
CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.o: CMakeFiles/pracownik_gora.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.o -MF CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.o.d -o CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.o -c /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/pracownik_gora.cpp

CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/pracownik_gora.cpp > CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.i

CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/pracownik_gora.cpp -o CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.s

# Object files for target pracownik_gora
pracownik_gora_OBJECTS = \
"CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.o"

# External object files for target pracownik_gora
pracownik_gora_EXTERNAL_OBJECTS =

pracownik_gora: CMakeFiles/pracownik_gora.dir/pracownik_gora.cpp.o
pracownik_gora: CMakeFiles/pracownik_gora.dir/build.make
pracownik_gora: CMakeFiles/pracownik_gora.dir/compiler_depend.ts
pracownik_gora: CMakeFiles/pracownik_gora.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable pracownik_gora"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pracownik_gora.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pracownik_gora.dir/build: pracownik_gora
.PHONY : CMakeFiles/pracownik_gora.dir/build

CMakeFiles/pracownik_gora.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pracownik_gora.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pracownik_gora.dir/clean

CMakeFiles/pracownik_gora.dir/depend:
	cd /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16 /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16 /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug /mnt/c/Users/xxmis/Desktop/Kamil_Gebala_SO_Projekt_16/cmake-build-debug/CMakeFiles/pracownik_gora.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/pracownik_gora.dir/depend

