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

CMakeFiles/libatp.dir/src/atp_socket.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_socket.cpp.o: src/atp_socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/libatp.dir/src/atp_socket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_socket.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_socket.cpp

CMakeFiles/libatp.dir/src/atp_socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_socket.cpp > CMakeFiles/libatp.dir/src/atp_socket.cpp.i

CMakeFiles/libatp.dir/src/atp_socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_socket.cpp -o CMakeFiles/libatp.dir/src/atp_socket.cpp.s

CMakeFiles/libatp.dir/src/atp_socket.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_socket.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_socket.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_socket.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_socket.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_socket.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_socket.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_socket.cpp.o


CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o: src/atp_dynamic_thread_pool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_dynamic_thread_pool.cpp

CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_dynamic_thread_pool.cpp > CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.i

CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_dynamic_thread_pool.cpp -o CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.s

CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o


CMakeFiles/libatp.dir/src/atp_libevent.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_libevent.cpp.o: src/atp_libevent.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/libatp.dir/src/atp_libevent.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_libevent.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_libevent.cpp

CMakeFiles/libatp.dir/src/atp_libevent.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_libevent.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_libevent.cpp > CMakeFiles/libatp.dir/src/atp_libevent.cpp.i

CMakeFiles/libatp.dir/src/atp_libevent.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_libevent.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_libevent.cpp -o CMakeFiles/libatp.dir/src/atp_libevent.cpp.s

CMakeFiles/libatp.dir/src/atp_libevent.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_libevent.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_libevent.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_libevent.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_libevent.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_libevent.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_libevent.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_libevent.cpp.o


CMakeFiles/libatp.dir/src/atp_channel.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_channel.cpp.o: src/atp_channel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/libatp.dir/src/atp_channel.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_channel.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_channel.cpp

CMakeFiles/libatp.dir/src/atp_channel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_channel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_channel.cpp > CMakeFiles/libatp.dir/src/atp_channel.cpp.i

CMakeFiles/libatp.dir/src/atp_channel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_channel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_channel.cpp -o CMakeFiles/libatp.dir/src/atp_channel.cpp.s

CMakeFiles/libatp.dir/src/atp_channel.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_channel.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_channel.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_channel.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_channel.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_channel.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_channel.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_channel.cpp.o


CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o: src/atp_tcp_conn.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_tcp_conn.cpp

CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_tcp_conn.cpp > CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.i

CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_tcp_conn.cpp -o CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.s

CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o


CMakeFiles/libatp.dir/src/atp_listener.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_listener.cpp.o: src/atp_listener.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/libatp.dir/src/atp_listener.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_listener.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_listener.cpp

CMakeFiles/libatp.dir/src/atp_listener.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_listener.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_listener.cpp > CMakeFiles/libatp.dir/src/atp_listener.cpp.i

CMakeFiles/libatp.dir/src/atp_listener.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_listener.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_listener.cpp -o CMakeFiles/libatp.dir/src/atp_listener.cpp.s

CMakeFiles/libatp.dir/src/atp_listener.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_listener.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_listener.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_listener.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_listener.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_listener.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_listener.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_listener.cpp.o


CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o: src/atp_memory_pool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o"
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
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o"
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


CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o: src/atp_event_watcher.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_event_watcher.cpp

CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_event_watcher.cpp > CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.i

CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_event_watcher.cpp -o CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.s

CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o


CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o: src/atp_event_loop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_event_loop.cpp

CMakeFiles/libatp.dir/src/atp_event_loop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_event_loop.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_event_loop.cpp > CMakeFiles/libatp.dir/src/atp_event_loop.cpp.i

CMakeFiles/libatp.dir/src/atp_event_loop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_event_loop.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_event_loop.cpp -o CMakeFiles/libatp.dir/src/atp_event_loop.cpp.s

CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o


CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o: src/atp_event_loop_thread_pool.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_event_loop_thread_pool.cpp

CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_event_loop_thread_pool.cpp > CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.i

CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_event_loop_thread_pool.cpp -o CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.s

CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o


CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o: src/atp_tcp_server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o -c /home/pengwang/cloud_platform/project/libatp/src/atp_tcp_server.cpp

CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/src/atp_tcp_server.cpp > CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.i

CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/src/atp_tcp_server.cpp -o CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.s

CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o.requires

CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o.provides: CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o.provides

CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o.provides.build: CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o


CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o: CMakeFiles/libatp.dir/flags.make
CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o: examples/atp_listener_benchmark.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o -c /home/pengwang/cloud_platform/project/libatp/examples/atp_listener_benchmark.cpp

CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pengwang/cloud_platform/project/libatp/examples/atp_listener_benchmark.cpp > CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.i

CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pengwang/cloud_platform/project/libatp/examples/atp_listener_benchmark.cpp -o CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.s

CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o.requires:

.PHONY : CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o.requires

CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o.provides: CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o.requires
	$(MAKE) -f CMakeFiles/libatp.dir/build.make CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o.provides.build
.PHONY : CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o.provides

CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o.provides.build: CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o


# Object files for target libatp
libatp_OBJECTS = \
"CMakeFiles/libatp.dir/src/atp_socket.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_libevent.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_channel.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_listener.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o" \
"CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o" \
"CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o"

# External object files for target libatp
libatp_EXTERNAL_OBJECTS =

libatp: CMakeFiles/libatp.dir/src/atp_socket.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_libevent.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_channel.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_listener.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o
libatp: CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o
libatp: CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o
libatp: CMakeFiles/libatp.dir/build.make
libatp: /home/pengwang/cloud_platform/target/3rd/jsoncpp/libjsoncpp.a
libatp: /home/pengwang/cloud_platform/target/3rd/libevent/libevent_core.a
libatp: /home/pengwang/cloud_platform/target/3rd/gflags/libgflags.so
libatp: /home/pengwang/cloud_platform/target/3rd/glog/libglog.a
libatp: CMakeFiles/libatp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pengwang/cloud_platform/project/libatp/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable libatp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libatp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/libatp.dir/build: libatp

.PHONY : CMakeFiles/libatp.dir/build

CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_socket.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_dynamic_thread_pool.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_libevent.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_channel.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_tcp_conn.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_listener.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_memory_pool.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_memory_policy.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_event_watcher.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_event_loop.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_event_loop_thread_pool.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/src/atp_tcp_server.cpp.o.requires
CMakeFiles/libatp.dir/requires: CMakeFiles/libatp.dir/examples/atp_listener_benchmark.cpp.o.requires

.PHONY : CMakeFiles/libatp.dir/requires

CMakeFiles/libatp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libatp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libatp.dir/clean

CMakeFiles/libatp.dir/depend:
	cd /home/pengwang/cloud_platform/project/libatp && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pengwang/cloud_platform/project/libatp /home/pengwang/cloud_platform/project/libatp /home/pengwang/cloud_platform/project/libatp /home/pengwang/cloud_platform/project/libatp /home/pengwang/cloud_platform/project/libatp/CMakeFiles/libatp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/libatp.dir/depend

