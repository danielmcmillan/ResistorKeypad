cmake_minimum_required (VERSION 2.6)
project (DockKeypad)

add_library(DockKeypad src/ADS1x15.cpp src/Reader.cpp src/EventEmitter.cpp src/sonyRadioConfiguration.cpp)
target_include_directories(DockKeypad PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_include_directories(DockKeypad PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Link to system WiringPi
find_library(wiringPi_LIB wiringPi)
if(NOT wiringPi_LIB)
  message(FATAL_ERROR "wiringPi library not found")
endif()
target_link_libraries(DockKeypad ${wiringPi_LIB})
target_link_libraries(DockKeypad ${wiringPi_LIB})
# Link thread library
find_package(Threads REQUIRED)
target_link_libraries(DockKeypad Threads::Threads)
