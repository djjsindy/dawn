# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /opt/local/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jianjundeng/dawn

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jianjundeng/dawn

# Include any dependencies generated for this target.
include CMakeFiles/./bin/dawn.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/./bin/dawn.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/./bin/dawn.dir/flags.make

CMakeFiles/./bin/dawn.dir/src/buffer.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/buffer.c.o: src/buffer.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/buffer.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/buffer.c.o   -c /Users/jianjundeng/dawn/src/buffer.c

CMakeFiles/./bin/dawn.dir/src/buffer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/buffer.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/buffer.c > CMakeFiles/./bin/dawn.dir/src/buffer.c.i

CMakeFiles/./bin/dawn.dir/src/buffer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/buffer.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/buffer.c -o CMakeFiles/./bin/dawn.dir/src/buffer.c.s

CMakeFiles/./bin/dawn.dir/src/buffer.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/buffer.c.o.requires

CMakeFiles/./bin/dawn.dir/src/buffer.c.o.provides: CMakeFiles/./bin/dawn.dir/src/buffer.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/buffer.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/buffer.c.o.provides

CMakeFiles/./bin/dawn.dir/src/buffer.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/buffer.c.o

CMakeFiles/./bin/dawn.dir/src/connection.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/connection.c.o: src/connection.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/connection.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/connection.c.o   -c /Users/jianjundeng/dawn/src/connection.c

CMakeFiles/./bin/dawn.dir/src/connection.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/connection.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/connection.c > CMakeFiles/./bin/dawn.dir/src/connection.c.i

CMakeFiles/./bin/dawn.dir/src/connection.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/connection.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/connection.c -o CMakeFiles/./bin/dawn.dir/src/connection.c.s

CMakeFiles/./bin/dawn.dir/src/connection.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/connection.c.o.requires

CMakeFiles/./bin/dawn.dir/src/connection.c.o.provides: CMakeFiles/./bin/dawn.dir/src/connection.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/connection.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/connection.c.o.provides

CMakeFiles/./bin/dawn.dir/src/connection.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/connection.c.o

CMakeFiles/./bin/dawn.dir/src/daemon.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/daemon.c.o: src/daemon.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/daemon.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/daemon.c.o   -c /Users/jianjundeng/dawn/src/daemon.c

CMakeFiles/./bin/dawn.dir/src/daemon.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/daemon.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/daemon.c > CMakeFiles/./bin/dawn.dir/src/daemon.c.i

CMakeFiles/./bin/dawn.dir/src/daemon.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/daemon.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/daemon.c -o CMakeFiles/./bin/dawn.dir/src/daemon.c.s

CMakeFiles/./bin/dawn.dir/src/daemon.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/daemon.c.o.requires

CMakeFiles/./bin/dawn.dir/src/daemon.c.o.provides: CMakeFiles/./bin/dawn.dir/src/daemon.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/daemon.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/daemon.c.o.provides

CMakeFiles/./bin/dawn.dir/src/daemon.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/daemon.c.o

CMakeFiles/./bin/dawn.dir/src/dy_char.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/dy_char.c.o: src/dy_char.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/dy_char.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/dy_char.c.o   -c /Users/jianjundeng/dawn/src/dy_char.c

CMakeFiles/./bin/dawn.dir/src/dy_char.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/dy_char.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/dy_char.c > CMakeFiles/./bin/dawn.dir/src/dy_char.c.i

CMakeFiles/./bin/dawn.dir/src/dy_char.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/dy_char.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/dy_char.c -o CMakeFiles/./bin/dawn.dir/src/dy_char.c.s

CMakeFiles/./bin/dawn.dir/src/dy_char.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/dy_char.c.o.requires

CMakeFiles/./bin/dawn.dir/src/dy_char.c.o.provides: CMakeFiles/./bin/dawn.dir/src/dy_char.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/dy_char.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/dy_char.c.o.provides

CMakeFiles/./bin/dawn.dir/src/dy_char.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/dy_char.c.o

CMakeFiles/./bin/dawn.dir/src/epoll.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/epoll.c.o: src/epoll.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/epoll.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/epoll.c.o   -c /Users/jianjundeng/dawn/src/epoll.c

CMakeFiles/./bin/dawn.dir/src/epoll.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/epoll.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/epoll.c > CMakeFiles/./bin/dawn.dir/src/epoll.c.i

CMakeFiles/./bin/dawn.dir/src/epoll.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/epoll.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/epoll.c -o CMakeFiles/./bin/dawn.dir/src/epoll.c.s

CMakeFiles/./bin/dawn.dir/src/epoll.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/epoll.c.o.requires

CMakeFiles/./bin/dawn.dir/src/epoll.c.o.provides: CMakeFiles/./bin/dawn.dir/src/epoll.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/epoll.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/epoll.c.o.provides

CMakeFiles/./bin/dawn.dir/src/epoll.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/epoll.c.o

CMakeFiles/./bin/dawn.dir/src/fdop.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/fdop.c.o: src/fdop.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/fdop.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/fdop.c.o   -c /Users/jianjundeng/dawn/src/fdop.c

CMakeFiles/./bin/dawn.dir/src/fdop.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/fdop.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/fdop.c > CMakeFiles/./bin/dawn.dir/src/fdop.c.i

CMakeFiles/./bin/dawn.dir/src/fdop.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/fdop.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/fdop.c -o CMakeFiles/./bin/dawn.dir/src/fdop.c.s

CMakeFiles/./bin/dawn.dir/src/fdop.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/fdop.c.o.requires

CMakeFiles/./bin/dawn.dir/src/fdop.c.o.provides: CMakeFiles/./bin/dawn.dir/src/fdop.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/fdop.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/fdop.c.o.provides

CMakeFiles/./bin/dawn.dir/src/fdop.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/fdop.c.o

CMakeFiles/./bin/dawn.dir/src/hash.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/hash.c.o: src/hash.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/hash.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/hash.c.o   -c /Users/jianjundeng/dawn/src/hash.c

CMakeFiles/./bin/dawn.dir/src/hash.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/hash.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/hash.c > CMakeFiles/./bin/dawn.dir/src/hash.c.i

CMakeFiles/./bin/dawn.dir/src/hash.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/hash.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/hash.c -o CMakeFiles/./bin/dawn.dir/src/hash.c.s

CMakeFiles/./bin/dawn.dir/src/hash.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/hash.c.o.requires

CMakeFiles/./bin/dawn.dir/src/hash.c.o.provides: CMakeFiles/./bin/dawn.dir/src/hash.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/hash.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/hash.c.o.provides

CMakeFiles/./bin/dawn.dir/src/hash.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/hash.c.o

CMakeFiles/./bin/dawn.dir/src/item.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/item.c.o: src/item.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/item.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/item.c.o   -c /Users/jianjundeng/dawn/src/item.c

CMakeFiles/./bin/dawn.dir/src/item.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/item.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/item.c > CMakeFiles/./bin/dawn.dir/src/item.c.i

CMakeFiles/./bin/dawn.dir/src/item.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/item.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/item.c -o CMakeFiles/./bin/dawn.dir/src/item.c.s

CMakeFiles/./bin/dawn.dir/src/item.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/item.c.o.requires

CMakeFiles/./bin/dawn.dir/src/item.c.o.provides: CMakeFiles/./bin/dawn.dir/src/item.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/item.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/item.c.o.provides

CMakeFiles/./bin/dawn.dir/src/item.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/item.c.o

CMakeFiles/./bin/dawn.dir/src/kqueue.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/kqueue.c.o: src/kqueue.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/kqueue.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/kqueue.c.o   -c /Users/jianjundeng/dawn/src/kqueue.c

CMakeFiles/./bin/dawn.dir/src/kqueue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/kqueue.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/kqueue.c > CMakeFiles/./bin/dawn.dir/src/kqueue.c.i

CMakeFiles/./bin/dawn.dir/src/kqueue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/kqueue.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/kqueue.c -o CMakeFiles/./bin/dawn.dir/src/kqueue.c.s

CMakeFiles/./bin/dawn.dir/src/kqueue.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/kqueue.c.o.requires

CMakeFiles/./bin/dawn.dir/src/kqueue.c.o.provides: CMakeFiles/./bin/dawn.dir/src/kqueue.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/kqueue.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/kqueue.c.o.provides

CMakeFiles/./bin/dawn.dir/src/kqueue.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/kqueue.c.o

CMakeFiles/./bin/dawn.dir/src/list.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/list.c.o: src/list.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/list.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/list.c.o   -c /Users/jianjundeng/dawn/src/list.c

CMakeFiles/./bin/dawn.dir/src/list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/list.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/list.c > CMakeFiles/./bin/dawn.dir/src/list.c.i

CMakeFiles/./bin/dawn.dir/src/list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/list.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/list.c -o CMakeFiles/./bin/dawn.dir/src/list.c.s

CMakeFiles/./bin/dawn.dir/src/list.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/list.c.o.requires

CMakeFiles/./bin/dawn.dir/src/list.c.o.provides: CMakeFiles/./bin/dawn.dir/src/list.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/list.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/list.c.o.provides

CMakeFiles/./bin/dawn.dir/src/list.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/list.c.o

CMakeFiles/./bin/dawn.dir/src/main.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/main.c.o: src/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/main.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/main.c.o   -c /Users/jianjundeng/dawn/src/main.c

CMakeFiles/./bin/dawn.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/main.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/main.c > CMakeFiles/./bin/dawn.dir/src/main.c.i

CMakeFiles/./bin/dawn.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/main.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/main.c -o CMakeFiles/./bin/dawn.dir/src/main.c.s

CMakeFiles/./bin/dawn.dir/src/main.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/main.c.o.requires

CMakeFiles/./bin/dawn.dir/src/main.c.o.provides: CMakeFiles/./bin/dawn.dir/src/main.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/main.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/main.c.o.provides

CMakeFiles/./bin/dawn.dir/src/main.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/main.c.o

CMakeFiles/./bin/dawn.dir/src/memory.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/memory.c.o: src/memory.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/memory.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/memory.c.o   -c /Users/jianjundeng/dawn/src/memory.c

CMakeFiles/./bin/dawn.dir/src/memory.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/memory.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/memory.c > CMakeFiles/./bin/dawn.dir/src/memory.c.i

CMakeFiles/./bin/dawn.dir/src/memory.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/memory.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/memory.c -o CMakeFiles/./bin/dawn.dir/src/memory.c.s

CMakeFiles/./bin/dawn.dir/src/memory.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/memory.c.o.requires

CMakeFiles/./bin/dawn.dir/src/memory.c.o.provides: CMakeFiles/./bin/dawn.dir/src/memory.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/memory.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/memory.c.o.provides

CMakeFiles/./bin/dawn.dir/src/memory.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/memory.c.o

CMakeFiles/./bin/dawn.dir/src/my_log.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/my_log.c.o: src/my_log.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_13)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/my_log.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/my_log.c.o   -c /Users/jianjundeng/dawn/src/my_log.c

CMakeFiles/./bin/dawn.dir/src/my_log.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/my_log.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/my_log.c > CMakeFiles/./bin/dawn.dir/src/my_log.c.i

CMakeFiles/./bin/dawn.dir/src/my_log.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/my_log.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/my_log.c -o CMakeFiles/./bin/dawn.dir/src/my_log.c.s

CMakeFiles/./bin/dawn.dir/src/my_log.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/my_log.c.o.requires

CMakeFiles/./bin/dawn.dir/src/my_log.c.o.provides: CMakeFiles/./bin/dawn.dir/src/my_log.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/my_log.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/my_log.c.o.provides

CMakeFiles/./bin/dawn.dir/src/my_log.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/my_log.c.o

CMakeFiles/./bin/dawn.dir/src/persistence.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/persistence.c.o: src/persistence.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_14)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/persistence.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/persistence.c.o   -c /Users/jianjundeng/dawn/src/persistence.c

CMakeFiles/./bin/dawn.dir/src/persistence.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/persistence.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/persistence.c > CMakeFiles/./bin/dawn.dir/src/persistence.c.i

CMakeFiles/./bin/dawn.dir/src/persistence.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/persistence.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/persistence.c -o CMakeFiles/./bin/dawn.dir/src/persistence.c.s

CMakeFiles/./bin/dawn.dir/src/persistence.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/persistence.c.o.requires

CMakeFiles/./bin/dawn.dir/src/persistence.c.o.provides: CMakeFiles/./bin/dawn.dir/src/persistence.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/persistence.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/persistence.c.o.provides

CMakeFiles/./bin/dawn.dir/src/persistence.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/persistence.c.o

CMakeFiles/./bin/dawn.dir/src/poll.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/poll.c.o: src/poll.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_15)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/poll.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/poll.c.o   -c /Users/jianjundeng/dawn/src/poll.c

CMakeFiles/./bin/dawn.dir/src/poll.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/poll.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/poll.c > CMakeFiles/./bin/dawn.dir/src/poll.c.i

CMakeFiles/./bin/dawn.dir/src/poll.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/poll.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/poll.c -o CMakeFiles/./bin/dawn.dir/src/poll.c.s

CMakeFiles/./bin/dawn.dir/src/poll.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/poll.c.o.requires

CMakeFiles/./bin/dawn.dir/src/poll.c.o.provides: CMakeFiles/./bin/dawn.dir/src/poll.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/poll.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/poll.c.o.provides

CMakeFiles/./bin/dawn.dir/src/poll.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/poll.c.o

CMakeFiles/./bin/dawn.dir/src/protocol.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/protocol.c.o: src/protocol.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_16)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/protocol.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/protocol.c.o   -c /Users/jianjundeng/dawn/src/protocol.c

CMakeFiles/./bin/dawn.dir/src/protocol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/protocol.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/protocol.c > CMakeFiles/./bin/dawn.dir/src/protocol.c.i

CMakeFiles/./bin/dawn.dir/src/protocol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/protocol.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/protocol.c -o CMakeFiles/./bin/dawn.dir/src/protocol.c.s

CMakeFiles/./bin/dawn.dir/src/protocol.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/protocol.c.o.requires

CMakeFiles/./bin/dawn.dir/src/protocol.c.o.provides: CMakeFiles/./bin/dawn.dir/src/protocol.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/protocol.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/protocol.c.o.provides

CMakeFiles/./bin/dawn.dir/src/protocol.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/protocol.c.o

CMakeFiles/./bin/dawn.dir/src/queue.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/queue.c.o: src/queue.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_17)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/queue.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/queue.c.o   -c /Users/jianjundeng/dawn/src/queue.c

CMakeFiles/./bin/dawn.dir/src/queue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/queue.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/queue.c > CMakeFiles/./bin/dawn.dir/src/queue.c.i

CMakeFiles/./bin/dawn.dir/src/queue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/queue.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/queue.c -o CMakeFiles/./bin/dawn.dir/src/queue.c.s

CMakeFiles/./bin/dawn.dir/src/queue.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/queue.c.o.requires

CMakeFiles/./bin/dawn.dir/src/queue.c.o.provides: CMakeFiles/./bin/dawn.dir/src/queue.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/queue.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/queue.c.o.provides

CMakeFiles/./bin/dawn.dir/src/queue.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/queue.c.o

CMakeFiles/./bin/dawn.dir/src/select.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/select.c.o: src/select.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_18)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/select.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/select.c.o   -c /Users/jianjundeng/dawn/src/select.c

CMakeFiles/./bin/dawn.dir/src/select.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/select.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/select.c > CMakeFiles/./bin/dawn.dir/src/select.c.i

CMakeFiles/./bin/dawn.dir/src/select.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/select.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/select.c -o CMakeFiles/./bin/dawn.dir/src/select.c.s

CMakeFiles/./bin/dawn.dir/src/select.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/select.c.o.requires

CMakeFiles/./bin/dawn.dir/src/select.c.o.provides: CMakeFiles/./bin/dawn.dir/src/select.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/select.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/select.c.o.provides

CMakeFiles/./bin/dawn.dir/src/select.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/select.c.o

CMakeFiles/./bin/dawn.dir/src/thread.c.o: CMakeFiles/./bin/dawn.dir/flags.make
CMakeFiles/./bin/dawn.dir/src/thread.c.o: src/thread.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_19)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/./bin/dawn.dir/src/thread.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/./bin/dawn.dir/src/thread.c.o   -c /Users/jianjundeng/dawn/src/thread.c

CMakeFiles/./bin/dawn.dir/src/thread.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/./bin/dawn.dir/src/thread.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/thread.c > CMakeFiles/./bin/dawn.dir/src/thread.c.i

CMakeFiles/./bin/dawn.dir/src/thread.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/./bin/dawn.dir/src/thread.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/thread.c -o CMakeFiles/./bin/dawn.dir/src/thread.c.s

CMakeFiles/./bin/dawn.dir/src/thread.c.o.requires:
.PHONY : CMakeFiles/./bin/dawn.dir/src/thread.c.o.requires

CMakeFiles/./bin/dawn.dir/src/thread.c.o.provides: CMakeFiles/./bin/dawn.dir/src/thread.c.o.requires
	$(MAKE) -f CMakeFiles/./bin/dawn.dir/build.make CMakeFiles/./bin/dawn.dir/src/thread.c.o.provides.build
.PHONY : CMakeFiles/./bin/dawn.dir/src/thread.c.o.provides

CMakeFiles/./bin/dawn.dir/src/thread.c.o.provides.build: CMakeFiles/./bin/dawn.dir/src/thread.c.o

# Object files for target ./bin/dawn
_/bin/dawn_OBJECTS = \
"CMakeFiles/./bin/dawn.dir/src/buffer.c.o" \
"CMakeFiles/./bin/dawn.dir/src/connection.c.o" \
"CMakeFiles/./bin/dawn.dir/src/daemon.c.o" \
"CMakeFiles/./bin/dawn.dir/src/dy_char.c.o" \
"CMakeFiles/./bin/dawn.dir/src/epoll.c.o" \
"CMakeFiles/./bin/dawn.dir/src/fdop.c.o" \
"CMakeFiles/./bin/dawn.dir/src/hash.c.o" \
"CMakeFiles/./bin/dawn.dir/src/item.c.o" \
"CMakeFiles/./bin/dawn.dir/src/kqueue.c.o" \
"CMakeFiles/./bin/dawn.dir/src/list.c.o" \
"CMakeFiles/./bin/dawn.dir/src/main.c.o" \
"CMakeFiles/./bin/dawn.dir/src/memory.c.o" \
"CMakeFiles/./bin/dawn.dir/src/my_log.c.o" \
"CMakeFiles/./bin/dawn.dir/src/persistence.c.o" \
"CMakeFiles/./bin/dawn.dir/src/poll.c.o" \
"CMakeFiles/./bin/dawn.dir/src/protocol.c.o" \
"CMakeFiles/./bin/dawn.dir/src/queue.c.o" \
"CMakeFiles/./bin/dawn.dir/src/select.c.o" \
"CMakeFiles/./bin/dawn.dir/src/thread.c.o"

# External object files for target ./bin/dawn
_/bin/dawn_EXTERNAL_OBJECTS =

./bin/dawn: CMakeFiles/./bin/dawn.dir/src/buffer.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/connection.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/daemon.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/dy_char.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/epoll.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/fdop.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/hash.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/item.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/kqueue.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/list.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/main.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/memory.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/my_log.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/persistence.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/poll.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/protocol.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/queue.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/select.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/src/thread.c.o
./bin/dawn: CMakeFiles/./bin/dawn.dir/build.make
./bin/dawn: CMakeFiles/./bin/dawn.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable ./bin/dawn"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/./bin/dawn.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/./bin/dawn.dir/build: ./bin/dawn
.PHONY : CMakeFiles/./bin/dawn.dir/build

CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/buffer.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/connection.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/daemon.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/dy_char.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/epoll.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/fdop.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/hash.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/item.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/kqueue.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/list.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/main.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/memory.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/my_log.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/persistence.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/poll.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/protocol.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/queue.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/select.c.o.requires
CMakeFiles/./bin/dawn.dir/requires: CMakeFiles/./bin/dawn.dir/src/thread.c.o.requires
.PHONY : CMakeFiles/./bin/dawn.dir/requires

CMakeFiles/./bin/dawn.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/./bin/dawn.dir/cmake_clean.cmake
.PHONY : CMakeFiles/./bin/dawn.dir/clean

CMakeFiles/./bin/dawn.dir/depend:
	cd /Users/jianjundeng/dawn && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jianjundeng/dawn /Users/jianjundeng/dawn /Users/jianjundeng/dawn /Users/jianjundeng/dawn /Users/jianjundeng/dawn/CMakeFiles/bin/dawn.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/./bin/dawn.dir/depend

