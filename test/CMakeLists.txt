cmake_minimum_required(VERSION 3.28)

set(PROJECT_NAME eikon_test)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
set(CMAKE_CXX_STANDARD 20)

project(${PROJECT_NAME})

if(WIN32)
    set(LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
    set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
elseif(APPLE)
    set(LIBRARY_DIR "/usr/local/lib")
    set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
else()
    set(LIBRARY_DIR "/usr/lib")
    set(BIN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin")
endif()

file(MAKE_DIRECTORY ${BIN_DIR})

file(
    GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${PROJECT_NAME} ${SOURCES})
add_library(eikon SHARED IMPORTED)

set_target_properties(eikon PROPERTIES IMPORTED_LOCATION "${LIBRARY_DIR}/libeikon.so")

target_link_libraries(${PROJECT_NAME} eikon)

add_definitions("-g")

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${BIN_DIR}  # L'eseguibile andrà sotto bin/
)

message(STATUS "Bin directory: ${BIN_DIR}")
message(STATUS "Library directory: ${LIBRARY_DIR}")
