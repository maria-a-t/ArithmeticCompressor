# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.2/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/maria/MSU/Compression/project/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/maria/MSU/Compression/project/build

# Include any dependencies generated for this target.
include CMakeFiles/compress.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/compress.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/compress.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/compress.dir/flags.make

CMakeFiles/compress.dir/main.cpp.o: CMakeFiles/compress.dir/flags.make
CMakeFiles/compress.dir/main.cpp.o: /Users/maria/MSU/Compression/project/src/main.cpp
CMakeFiles/compress.dir/main.cpp.o: CMakeFiles/compress.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/maria/MSU/Compression/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/compress.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/compress.dir/main.cpp.o -MF CMakeFiles/compress.dir/main.cpp.o.d -o CMakeFiles/compress.dir/main.cpp.o -c /Users/maria/MSU/Compression/project/src/main.cpp

CMakeFiles/compress.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/compress.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/maria/MSU/Compression/project/src/main.cpp > CMakeFiles/compress.dir/main.cpp.i

CMakeFiles/compress.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/compress.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/maria/MSU/Compression/project/src/main.cpp -o CMakeFiles/compress.dir/main.cpp.s

# Object files for target compress
compress_OBJECTS = \
"CMakeFiles/compress.dir/main.cpp.o"

# External object files for target compress
compress_EXTERNAL_OBJECTS =

compress: CMakeFiles/compress.dir/main.cpp.o
compress: CMakeFiles/compress.dir/build.make
compress: libutils.a
compress: CMakeFiles/compress.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/maria/MSU/Compression/project/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable compress"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compress.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/compress.dir/build: compress
.PHONY : CMakeFiles/compress.dir/build

CMakeFiles/compress.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/compress.dir/cmake_clean.cmake
.PHONY : CMakeFiles/compress.dir/clean

CMakeFiles/compress.dir/depend:
	cd /Users/maria/MSU/Compression/project/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/maria/MSU/Compression/project/src /Users/maria/MSU/Compression/project/src /Users/maria/MSU/Compression/project/build /Users/maria/MSU/Compression/project/build /Users/maria/MSU/Compression/project/build/CMakeFiles/compress.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/compress.dir/depend
