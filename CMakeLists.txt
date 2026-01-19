cmake_minimum_required(VERSION 3.28)

set(BINARY_NAME eikon)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(WIN32)
    set(CMAKE_INSTALL_LIBDIR "lib")
    set(CMAKE_INSTALL_INCLUDEDIR "include")
elseif(APPLE)
    set(CMAKE_INSTALL_LIBDIR "/usr/local/lib")
    set(CMAKE_INSTALL_INCLUDEDIR "/usr/local/include")
else()
    set(CMAKE_INSTALL_LIBDIR "/usr/lib")
    set(CMAKE_INSTALL_INCLUDEDIR "/usr/include")
endif()


project(${BINARY_NAME})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(
    ${BINARY_NAME} SHARED
    src/bmp.cpp
    src/eikon.cpp
    src/font.cpp
    src/pixels.cpp
    src/png.cpp
    src/ppm.cpp
    src/shapes.cpp
    src/utils.cpp
)

install(
    TARGETS ${BINARY_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBRARY}
)

install(
    FILES
    include/bmp.hpp
    include/colors.hpp
    include/eikon.hpp
    include/files.hpp
    include/font.hpp
    include/formats.hpp
    include/matrix.hpp
    include/pixels.hpp
    include/png.hpp
    include/ppm.hpp
    include/shapes.hpp
    include/utils.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${BINARY_NAME}
)

