cmake_minimum_required(VERSION 3.10)
project(PRT7Decoder)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
    src/ListaDeCarga.cpp
    src/RotorDeMapeo.cpp
    src/SerialPort.cpp
    src/TramaLoad.cpp
    src/TramaMap.cpp
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Create executable
add_executable(PRT7Decoder ${SOURCES})

# Enable Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT_DIR ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/docs/mainpage.md)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/docs/output)
    set(DOXYGEN_CONFIG_FILE ${CMAKE_SOURCE_DIR}/docs/Doxyfile)
    
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen..."
    )
endif()

# Optional: Link with pthread for serial port operations if needed
find_package(Threads REQUIRED)
target_link_libraries(PRT7Decoder Threads::Threads)
