# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pengwang/cloud_platform/project/libatp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pengwang/cloud_platform/project/libatp

# Include any dependencies generated for this target.
include CMakeFiles/libatp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/libatp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/libatp.dir/flags.make

CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o: src/atp_memory_pool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_memory_pool.cpp

CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_memory_pool.cpp > CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.i

CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_memory_pool.cpp -o CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.s

CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o


CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o: src/atp_memory_policy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_memory_policy.cpp

CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_memory_policy.cpp > CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.i

CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_memory_policy.cpp -o CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.s

CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o


CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o: examples/atp_memory_pool_benchmark.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o -c /home/pengwang/cloud_platform/project/libatp/examples/atp_memory_pool_benchmark.cpp

CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/examples/atp_memory_pool_benchmark.cpp > CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.i

CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/examples/atp_memory_pool_benchmark.cpp -o CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.s

CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o.requires

CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o.provides: CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o.provides

CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o.provides.build: CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o


# Object files for target libatp
libatp_OBJECTS = \
"CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o" \
"CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o"

# External object files for target libatp
libatp_EXTERNAL_OBJECTS =

libatp: CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o
libatp: CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o
libatp: CMakeFiles/libatp.dir/build.make
libatp: /home/pengwang/cloud_platform/target/3rd/libevent/libevent_core.a
libatp: /home/pengwang/cloud_platform/target/3rd/gflags/libgflags.so
libatp: /home/pengwang/cloud_platform/target/3rd/glog/libglog.a
libatp: CMakeFiles/libatp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable libatp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libatp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/libatp.dir/build: libatp

.PHONY : CMakeFiles/libatp.dir/build

CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/examples/atp_memory_pool_benchmark.cpp.o.requires

.PHONY : CMakeFiles/libatp.dir/requires

CMakeFiles/libatp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libatp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libatp.dir/clean

CMakeFiles/libatp.dir/depend:
	cd /home/pengwang/cloud_platform/project/libatp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pengwang/cloud_platform/project/libatp /home/pengwang/cloud_platform/project/libatp /home/pengwang/cloud_platform/project/libatp /home/pengwang/cloud_platform/project/libatp /home/pengwang/cloud_platform/project/libatp/CMakeFiles/libatp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/libatp.dir/depend
