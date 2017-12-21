project(tests)
cmake_minimum_required(VERSION 2.8)
set(SRC_LIST TimeSeries_tests.cpp Utilities_tests.cpp TimeDelta_tests.cpp SqlFile_tests.cpp catch.hpp)
include_directories(../src)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} sqlite3)
IF(WIN32) # Check if we are on Windows
  if(MSVC) # Check if we are using the Visual Studio compiler
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:CONSOLE")
	set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
  elseif(CMAKE_COMPILER_IS_GNUCXX)
    # SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
  endif()
elseif(UNIX)
  # Nothing special required
else()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
ENDIF()

add_custom_command(TARGET tests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${CMAKE_SOURCE_DIR}/test/resources/RefBldgMediumOfficeNew2004_v1.4_8.8_5A_USA_IL_CHICAGO-OHARE.sql $<TARGET_FILE_DIR:tests>)

