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
include CMakeFiles/dawn.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dawn.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dawn.dir/flags.make

CMakeFiles/dawn.dir/src/buffer.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/buffer.o: src/buffer.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/buffer.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/buffer.o   -c /Users/jianjundeng/dawn/src/buffer.c

CMakeFiles/dawn.dir/src/buffer.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/buffer.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/buffer.c > CMakeFiles/dawn.dir/src/buffer.i

CMakeFiles/dawn.dir/src/buffer.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/buffer.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/buffer.c -o CMakeFiles/dawn.dir/src/buffer.s

CMakeFiles/dawn.dir/src/buffer.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/buffer.o.requires

CMakeFiles/dawn.dir/src/buffer.o.provides: CMakeFiles/dawn.dir/src/buffer.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/buffer.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/buffer.o.provides

CMakeFiles/dawn.dir/src/buffer.o.provides.build: CMakeFiles/dawn.dir/src/buffer.o

CMakeFiles/dawn.dir/src/connection.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/connection.o: src/connection.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/connection.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/connection.o   -c /Users/jianjundeng/dawn/src/connection.c

CMakeFiles/dawn.dir/src/connection.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/connection.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/connection.c > CMakeFiles/dawn.dir/src/connection.i

CMakeFiles/dawn.dir/src/connection.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/connection.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/connection.c -o CMakeFiles/dawn.dir/src/connection.s

CMakeFiles/dawn.dir/src/connection.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/connection.o.requires

CMakeFiles/dawn.dir/src/connection.o.provides: CMakeFiles/dawn.dir/src/connection.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/connection.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/connection.o.provides

CMakeFiles/dawn.dir/src/connection.o.provides.build: CMakeFiles/dawn.dir/src/connection.o

CMakeFiles/dawn.dir/src/dy_char.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/dy_char.o: src/dy_char.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/dy_char.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/dy_char.o   -c /Users/jianjundeng/dawn/src/dy_char.c

CMakeFiles/dawn.dir/src/dy_char.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/dy_char.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/dy_char.c > CMakeFiles/dawn.dir/src/dy_char.i

CMakeFiles/dawn.dir/src/dy_char.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/dy_char.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/dy_char.c -o CMakeFiles/dawn.dir/src/dy_char.s

CMakeFiles/dawn.dir/src/dy_char.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/dy_char.o.requires

CMakeFiles/dawn.dir/src/dy_char.o.provides: CMakeFiles/dawn.dir/src/dy_char.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/dy_char.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/dy_char.o.provides

CMakeFiles/dawn.dir/src/dy_char.o.provides.build: CMakeFiles/dawn.dir/src/dy_char.o

CMakeFiles/dawn.dir/src/fdop.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/fdop.o: src/fdop.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/fdop.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/fdop.o   -c /Users/jianjundeng/dawn/src/fdop.c

CMakeFiles/dawn.dir/src/fdop.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/fdop.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/fdop.c > CMakeFiles/dawn.dir/src/fdop.i

CMakeFiles/dawn.dir/src/fdop.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/fdop.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/fdop.c -o CMakeFiles/dawn.dir/src/fdop.s

CMakeFiles/dawn.dir/src/fdop.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/fdop.o.requires

CMakeFiles/dawn.dir/src/fdop.o.provides: CMakeFiles/dawn.dir/src/fdop.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/fdop.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/fdop.o.provides

CMakeFiles/dawn.dir/src/fdop.o.provides.build: CMakeFiles/dawn.dir/src/fdop.o

CMakeFiles/dawn.dir/src/hash.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/hash.o: src/hash.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/hash.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/hash.o   -c /Users/jianjundeng/dawn/src/hash.c

CMakeFiles/dawn.dir/src/hash.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/hash.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/hash.c > CMakeFiles/dawn.dir/src/hash.i

CMakeFiles/dawn.dir/src/hash.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/hash.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/hash.c -o CMakeFiles/dawn.dir/src/hash.s

CMakeFiles/dawn.dir/src/hash.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/hash.o.requires

CMakeFiles/dawn.dir/src/hash.o.provides: CMakeFiles/dawn.dir/src/hash.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/hash.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/hash.o.provides

CMakeFiles/dawn.dir/src/hash.o.provides.build: CMakeFiles/dawn.dir/src/hash.o

CMakeFiles/dawn.dir/src/item.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/item.o: src/item.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/item.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/item.o   -c /Users/jianjundeng/dawn/src/item.c

CMakeFiles/dawn.dir/src/item.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/item.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/item.c > CMakeFiles/dawn.dir/src/item.i

CMakeFiles/dawn.dir/src/item.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/item.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/item.c -o CMakeFiles/dawn.dir/src/item.s

CMakeFiles/dawn.dir/src/item.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/item.o.requires

CMakeFiles/dawn.dir/src/item.o.provides: CMakeFiles/dawn.dir/src/item.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/item.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/item.o.provides

CMakeFiles/dawn.dir/src/item.o.provides.build: CMakeFiles/dawn.dir/src/item.o

CMakeFiles/dawn.dir/src/kqueue.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/kqueue.o: src/kqueue.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/kqueue.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/kqueue.o   -c /Users/jianjundeng/dawn/src/kqueue.c

CMakeFiles/dawn.dir/src/kqueue.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/kqueue.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/kqueue.c > CMakeFiles/dawn.dir/src/kqueue.i

CMakeFiles/dawn.dir/src/kqueue.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/kqueue.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/kqueue.c -o CMakeFiles/dawn.dir/src/kqueue.s

CMakeFiles/dawn.dir/src/kqueue.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/kqueue.o.requires

CMakeFiles/dawn.dir/src/kqueue.o.provides: CMakeFiles/dawn.dir/src/kqueue.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/kqueue.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/kqueue.o.provides

CMakeFiles/dawn.dir/src/kqueue.o.provides.build: CMakeFiles/dawn.dir/src/kqueue.o

CMakeFiles/dawn.dir/src/list.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/list.o: src/list.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/list.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/list.o   -c /Users/jianjundeng/dawn/src/list.c

CMakeFiles/dawn.dir/src/list.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/list.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/list.c > CMakeFiles/dawn.dir/src/list.i

CMakeFiles/dawn.dir/src/list.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/list.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/list.c -o CMakeFiles/dawn.dir/src/list.s

CMakeFiles/dawn.dir/src/list.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/list.o.requires

CMakeFiles/dawn.dir/src/list.o.provides: CMakeFiles/dawn.dir/src/list.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/list.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/list.o.provides

CMakeFiles/dawn.dir/src/list.o.provides.build: CMakeFiles/dawn.dir/src/list.o

CMakeFiles/dawn.dir/src/main.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/main.o: src/main.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/main.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/main.o   -c /Users/jianjundeng/dawn/src/main.c

CMakeFiles/dawn.dir/src/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/main.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/main.c > CMakeFiles/dawn.dir/src/main.i

CMakeFiles/dawn.dir/src/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/main.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/main.c -o CMakeFiles/dawn.dir/src/main.s

CMakeFiles/dawn.dir/src/main.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/main.o.requires

CMakeFiles/dawn.dir/src/main.o.provides: CMakeFiles/dawn.dir/src/main.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/main.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/main.o.provides

CMakeFiles/dawn.dir/src/main.o.provides.build: CMakeFiles/dawn.dir/src/main.o

CMakeFiles/dawn.dir/src/malloc.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/malloc.o: src/malloc.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/malloc.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/malloc.o   -c /Users/jianjundeng/dawn/src/malloc.c

CMakeFiles/dawn.dir/src/malloc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/malloc.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/malloc.c > CMakeFiles/dawn.dir/src/malloc.i

CMakeFiles/dawn.dir/src/malloc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/malloc.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/malloc.c -o CMakeFiles/dawn.dir/src/malloc.s

CMakeFiles/dawn.dir/src/malloc.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/malloc.o.requires

CMakeFiles/dawn.dir/src/malloc.o.provides: CMakeFiles/dawn.dir/src/malloc.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/malloc.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/malloc.o.provides

CMakeFiles/dawn.dir/src/malloc.o.provides.build: CMakeFiles/dawn.dir/src/malloc.o

CMakeFiles/dawn.dir/src/memory.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/memory.o: src/memory.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/memory.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/memory.o   -c /Users/jianjundeng/dawn/src/memory.c

CMakeFiles/dawn.dir/src/memory.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/memory.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/memory.c > CMakeFiles/dawn.dir/src/memory.i

CMakeFiles/dawn.dir/src/memory.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/memory.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/memory.c -o CMakeFiles/dawn.dir/src/memory.s

CMakeFiles/dawn.dir/src/memory.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/memory.o.requires

CMakeFiles/dawn.dir/src/memory.o.provides: CMakeFiles/dawn.dir/src/memory.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/memory.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/memory.o.provides

CMakeFiles/dawn.dir/src/memory.o.provides.build: CMakeFiles/dawn.dir/src/memory.o

CMakeFiles/dawn.dir/src/protocol.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/protocol.o: src/protocol.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/protocol.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/protocol.o   -c /Users/jianjundeng/dawn/src/protocol.c

CMakeFiles/dawn.dir/src/protocol.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/protocol.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/protocol.c > CMakeFiles/dawn.dir/src/protocol.i

CMakeFiles/dawn.dir/src/protocol.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/protocol.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/protocol.c -o CMakeFiles/dawn.dir/src/protocol.s

CMakeFiles/dawn.dir/src/protocol.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/protocol.o.requires

CMakeFiles/dawn.dir/src/protocol.o.provides: CMakeFiles/dawn.dir/src/protocol.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/protocol.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/protocol.o.provides

CMakeFiles/dawn.dir/src/protocol.o.provides.build: CMakeFiles/dawn.dir/src/protocol.o

CMakeFiles/dawn.dir/src/queue.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/queue.o: src/queue.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_13)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/queue.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/queue.o   -c /Users/jianjundeng/dawn/src/queue.c

CMakeFiles/dawn.dir/src/queue.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/queue.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/queue.c > CMakeFiles/dawn.dir/src/queue.i

CMakeFiles/dawn.dir/src/queue.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/queue.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/queue.c -o CMakeFiles/dawn.dir/src/queue.s

CMakeFiles/dawn.dir/src/queue.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/queue.o.requires

CMakeFiles/dawn.dir/src/queue.o.provides: CMakeFiles/dawn.dir/src/queue.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/queue.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/queue.o.provides

CMakeFiles/dawn.dir/src/queue.o.provides.build: CMakeFiles/dawn.dir/src/queue.o

CMakeFiles/dawn.dir/src/thread.o: CMakeFiles/dawn.dir/flags.make
CMakeFiles/dawn.dir/src/thread.o: src/thread.c
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/jianjundeng/dawn/CMakeFiles $(CMAKE_PROGRESS_14)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object CMakeFiles/dawn.dir/src/thread.o"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/dawn.dir/src/thread.o   -c /Users/jianjundeng/dawn/src/thread.c

CMakeFiles/dawn.dir/src/thread.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/dawn.dir/src/thread.i"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /Users/jianjundeng/dawn/src/thread.c > CMakeFiles/dawn.dir/src/thread.i

CMakeFiles/dawn.dir/src/thread.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/dawn.dir/src/thread.s"
	/usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /Users/jianjundeng/dawn/src/thread.c -o CMakeFiles/dawn.dir/src/thread.s

CMakeFiles/dawn.dir/src/thread.o.requires:
.PHONY : CMakeFiles/dawn.dir/src/thread.o.requires

CMakeFiles/dawn.dir/src/thread.o.provides: CMakeFiles/dawn.dir/src/thread.o.requires
	$(MAKE) -f CMakeFiles/dawn.dir/build.make CMakeFiles/dawn.dir/src/thread.o.provides.build
.PHONY : CMakeFiles/dawn.dir/src/thread.o.provides

CMakeFiles/dawn.dir/src/thread.o.provides.build: CMakeFiles/dawn.dir/src/thread.o

# Object files for target dawn
dawn_OBJECTS = \
"CMakeFiles/dawn.dir/src/buffer.o" \
"CMakeFiles/dawn.dir/src/connection.o" \
"CMakeFiles/dawn.dir/src/dy_char.o" \
"CMakeFiles/dawn.dir/src/fdop.o" \
"CMakeFiles/dawn.dir/src/hash.o" \
"CMakeFiles/dawn.dir/src/item.o" \
"CMakeFiles/dawn.dir/src/kqueue.o" \
"CMakeFiles/dawn.dir/src/list.o" \
"CMakeFiles/dawn.dir/src/main.o" \
"CMakeFiles/dawn.dir/src/malloc.o" \
"CMakeFiles/dawn.dir/src/memory.o" \
"CMakeFiles/dawn.dir/src/protocol.o" \
"CMakeFiles/dawn.dir/src/queue.o" \
"CMakeFiles/dawn.dir/src/thread.o"

# External object files for target dawn
dawn_EXTERNAL_OBJECTS =

dawn: CMakeFiles/dawn.dir/src/buffer.o
dawn: CMakeFiles/dawn.dir/src/connection.o
dawn: CMakeFiles/dawn.dir/src/dy_char.o
dawn: CMakeFiles/dawn.dir/src/fdop.o
dawn: CMakeFiles/dawn.dir/src/hash.o
dawn: CMakeFiles/dawn.dir/src/item.o
dawn: CMakeFiles/dawn.dir/src/kqueue.o
dawn: CMakeFiles/dawn.dir/src/list.o
dawn: CMakeFiles/dawn.dir/src/main.o
dawn: CMakeFiles/dawn.dir/src/malloc.o
dawn: CMakeFiles/dawn.dir/src/memory.o
dawn: CMakeFiles/dawn.dir/src/protocol.o
dawn: CMakeFiles/dawn.dir/src/queue.o
dawn: CMakeFiles/dawn.dir/src/thread.o
dawn: CMakeFiles/dawn.dir/build.make
dawn: CMakeFiles/dawn.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable dawn"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dawn.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dawn.dir/build: dawn
.PHONY : CMakeFiles/dawn.dir/build

CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/buffer.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/connection.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/dy_char.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/fdop.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/hash.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/item.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/kqueue.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/list.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/main.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/malloc.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/memory.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/protocol.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/queue.o.requires
CMakeFiles/dawn.dir/requires: CMakeFiles/dawn.dir/src/thread.o.requires
.PHONY : CMakeFiles/dawn.dir/requires

CMakeFiles/dawn.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dawn.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dawn.dir/clean

CMakeFiles/dawn.dir/depend:
	cd /Users/jianjundeng/dawn && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jianjundeng/dawn /Users/jianjundeng/dawn /Users/jianjundeng/dawn /Users/jianjundeng/dawn /Users/jianjundeng/dawn/CMakeFiles/dawn.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dawn.dir/depend

