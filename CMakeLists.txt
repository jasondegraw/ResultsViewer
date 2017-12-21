cmake_minimum_required(VERSION 3.8.2)

project(ResultsViewer)

set(VERSION_MAJOR 2)
set(VERSION_MINOR 2)
set(VERSION_PATCH 2)

if(CMAKE_COMPILER_IS_GNUCXX)
  # Apparently 4.6 is the earliest version supporting nullptr and range-based for
  # https://gcc.gnu.org/projects/cxx0x.html
  if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS "4.6.0")
    message(FATAL_ERROR "g++ versions earlier than 4.6.0 are not supported")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(MSVC)
  # http://en.wikipedia.org/wiki/Visual_C%2B%2B#32-bit_and_64-bit_versions
  if(${CMAKE_C_COMPILER_VERSION} VERSION_LESS "19.10.25017.0")
    message(FATAL_ERROR "Visual Studio earlier than VS2017 is not supported")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # This check for clang seems to work, at least on Ubuntu. Not that much 
  # else works on Ubuntu: had to add -I/usr/include/c++/4.8/ and 
  # -I/usr/include/x86_64-linux-gnu/c++/4.8/ to get it to compile.
  # Mac works as well!
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
  message(FATAL_ERROR, "Clang is untested")
else()
  message(FATAL_ERROR, "Uknown C++ compiler")
endif()

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# There may be a way to tell up front if Qt5 is going to be found, but I haven't found
# a foolproof way to do it yet, so settle for the default error message for now.
#if(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)
#    message(WARNING "CMAKE_PREFIX_PATH is not defined, so find_package may not work. Set the CMAKE_PREFIX_PATH "
#            "environment variable to the install prefix of Qt 5, either on the command line as "
#            "-DCMAKE_PREFIX_PATH=\"path/to/Qt5/lib/cmake\" or with set(CMAKE_PREFIX_PATH path/to/Qt5/lib/cmake)")
#endif(NOT CMAKE_PREFIX_PATH AND NOT Qt5Widgets_DIR)

# Add a compiler flag
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# Find the Qt libraries
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5PrintSupport REQUIRED)

# SQLite definitions, used in sqlite and litesql
add_definitions(-DSQLITE_THREADSAFE=1) # 1 is default, serial access
add_subdirectory(dependencies/sqlite3)

# qwt
add_subdirectory(dependencies/qwt-6.1.3)

include_directories("${CMAKE_HOME_DIRECTORY}/dependencies/")
include_directories("${CMAKE_BINARY_DIR}/qwt-include")

# Include the src
add_subdirectory(src)

# Add the testing
add_subdirectory(test)
