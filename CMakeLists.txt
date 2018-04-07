cmake_minimum_required (VERSION 2.6)
project (ResistorKeypad)

add_library(ResistorKeypad src/ADS1x15.cpp src/Reader.cpp src/EventEmitter.cpp src/sonyRadioConfiguration.cpp)
target_include_directories(ResistorKeypad PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_include_directories(ResistorKeypad PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Link to system WiringPi
find_library(wiringPi_LIB wiringPi)
if(NOT wiringPi_LIB)
  message(FATAL_ERROR "wiringPi library not found")
endif()
target_link_libraries(ResistorKeypad ${wiringPi_LIB})
# Link thread library
find_package(Threads REQUIRED)
target_link_libraries(ResistorKeypad Threads::Threads)
