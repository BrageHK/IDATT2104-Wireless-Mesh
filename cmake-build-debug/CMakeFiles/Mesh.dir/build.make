# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/Mesh.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Mesh.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Mesh.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Mesh.dir/flags.make

CMakeFiles/Mesh.dir/main.cpp.obj: CMakeFiles/Mesh.dir/flags.make
CMakeFiles/Mesh.dir/main.cpp.obj: D:/kode/wirelesh\ mesh/IDATT2104-Wireless-Mesh/main.cpp
CMakeFiles/Mesh.dir/main.cpp.obj: CMakeFiles/Mesh.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Mesh.dir/main.cpp.obj"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mesh.dir/main.cpp.obj -MF CMakeFiles\Mesh.dir\main.cpp.obj.d -o CMakeFiles\Mesh.dir\main.cpp.obj -c "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\main.cpp"

CMakeFiles/Mesh.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mesh.dir/main.cpp.i"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\main.cpp" > CMakeFiles\Mesh.dir\main.cpp.i

CMakeFiles/Mesh.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mesh.dir/main.cpp.s"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\main.cpp" -o CMakeFiles\Mesh.dir\main.cpp.s

CMakeFiles/Mesh.dir/node/Node.cpp.obj: CMakeFiles/Mesh.dir/flags.make
CMakeFiles/Mesh.dir/node/Node.cpp.obj: D:/kode/wirelesh\ mesh/IDATT2104-Wireless-Mesh/node/Node.cpp
CMakeFiles/Mesh.dir/node/Node.cpp.obj: CMakeFiles/Mesh.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Mesh.dir/node/Node.cpp.obj"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mesh.dir/node/Node.cpp.obj -MF CMakeFiles\Mesh.dir\node\Node.cpp.obj.d -o CMakeFiles\Mesh.dir\node\Node.cpp.obj -c "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\node\Node.cpp"

CMakeFiles/Mesh.dir/node/Node.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mesh.dir/node/Node.cpp.i"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\node\Node.cpp" > CMakeFiles\Mesh.dir\node\Node.cpp.i

CMakeFiles/Mesh.dir/node/Node.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mesh.dir/node/Node.cpp.s"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\node\Node.cpp" -o CMakeFiles\Mesh.dir\node\Node.cpp.s

CMakeFiles/Mesh.dir/worker/Workers.cpp.obj: CMakeFiles/Mesh.dir/flags.make
CMakeFiles/Mesh.dir/worker/Workers.cpp.obj: D:/kode/wirelesh\ mesh/IDATT2104-Wireless-Mesh/worker/Workers.cpp
CMakeFiles/Mesh.dir/worker/Workers.cpp.obj: CMakeFiles/Mesh.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Mesh.dir/worker/Workers.cpp.obj"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mesh.dir/worker/Workers.cpp.obj -MF CMakeFiles\Mesh.dir\worker\Workers.cpp.obj.d -o CMakeFiles\Mesh.dir\worker\Workers.cpp.obj -c "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\worker\Workers.cpp"

CMakeFiles/Mesh.dir/worker/Workers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mesh.dir/worker/Workers.cpp.i"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\worker\Workers.cpp" > CMakeFiles\Mesh.dir\worker\Workers.cpp.i

CMakeFiles/Mesh.dir/worker/Workers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mesh.dir/worker/Workers.cpp.s"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\worker\Workers.cpp" -o CMakeFiles\Mesh.dir\worker\Workers.cpp.s

CMakeFiles/Mesh.dir/topography/Topography.cpp.obj: CMakeFiles/Mesh.dir/flags.make
CMakeFiles/Mesh.dir/topography/Topography.cpp.obj: D:/kode/wirelesh\ mesh/IDATT2104-Wireless-Mesh/topography/Topography.cpp
CMakeFiles/Mesh.dir/topography/Topography.cpp.obj: CMakeFiles/Mesh.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Mesh.dir/topography/Topography.cpp.obj"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Mesh.dir/topography/Topography.cpp.obj -MF CMakeFiles\Mesh.dir\topography\Topography.cpp.obj.d -o CMakeFiles\Mesh.dir\topography\Topography.cpp.obj -c "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\topography\Topography.cpp"

CMakeFiles/Mesh.dir/topography/Topography.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mesh.dir/topography/Topography.cpp.i"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\topography\Topography.cpp" > CMakeFiles\Mesh.dir\topography\Topography.cpp.i

CMakeFiles/Mesh.dir/topography/Topography.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mesh.dir/topography/Topography.cpp.s"
	C:\Users\eiler\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\231.9011.31\bin\mingw\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\topography\Topography.cpp" -o CMakeFiles\Mesh.dir\topography\Topography.cpp.s

# Object files for target Mesh
Mesh_OBJECTS = \
"CMakeFiles/Mesh.dir/main.cpp.obj" \
"CMakeFiles/Mesh.dir/node/Node.cpp.obj" \
"CMakeFiles/Mesh.dir/worker/Workers.cpp.obj" \
"CMakeFiles/Mesh.dir/topography/Topography.cpp.obj"

# External object files for target Mesh
Mesh_EXTERNAL_OBJECTS =

Mesh.exe: CMakeFiles/Mesh.dir/main.cpp.obj
Mesh.exe: CMakeFiles/Mesh.dir/node/Node.cpp.obj
Mesh.exe: CMakeFiles/Mesh.dir/worker/Workers.cpp.obj
Mesh.exe: CMakeFiles/Mesh.dir/topography/Topography.cpp.obj
Mesh.exe: CMakeFiles/Mesh.dir/build.make
Mesh.exe: CMakeFiles/Mesh.dir/linkLibs.rsp
Mesh.exe: CMakeFiles/Mesh.dir/objects1
Mesh.exe: CMakeFiles/Mesh.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable Mesh.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Mesh.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Mesh.dir/build: Mesh.exe
.PHONY : CMakeFiles/Mesh.dir/build

CMakeFiles/Mesh.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Mesh.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Mesh.dir/clean

CMakeFiles/Mesh.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh" "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh" "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\cmake-build-debug" "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\cmake-build-debug" "D:\kode\wirelesh mesh\IDATT2104-Wireless-Mesh\cmake-build-debug\CMakeFiles\Mesh.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Mesh.dir/depend

