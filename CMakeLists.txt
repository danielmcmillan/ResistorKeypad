cmake_minimum_required (VERSION 2.6)
project (DockKeypad)

add_executable(DockKeypad src/keypad_io.cpp src/ADS1x15.cpp src/Keypad.cpp)
target_include_directories(DockKeypad PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_include_directories(DockKeypad PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Link to system WiringPi
find_library(wiringPi_LIB wiringPi)
if(NOT wiringPi_LIB)
  message(FATAL_ERROR "wiringPi library not found")
endif()
target_link_libraries(DockKeypad ${wiringPi_LIB})
