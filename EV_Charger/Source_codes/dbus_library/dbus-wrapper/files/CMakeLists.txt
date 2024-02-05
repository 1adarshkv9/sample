cmake_minimum_required(VERSION 3.5)
project(dbus-wrapper)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc}
    )

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -D_GNU_SOURCE -D_USINGLIBC -lrt -lpthread" )

set(JSON_LIBRARY "-ljson-c" )
find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS_GLIB-1  REQUIRED IMPORTED_TARGET dbus-glib-1)
pkg_check_modules(GLIB-2.0  REQUIRED IMPORTED_TARGET glib-2.0)
pkg_check_modules(DBUS-1  REQUIRED IMPORTED_TARGET dbus-1)

ADD_LIBRARY(dbus-wrapper SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dbus_wrapper.c
    )

include_directories(${JSON_INCLUDE_DIR})
target_link_libraries(dbus-wrapper PUBLIC
                      ${JSON_LIBRARY} 
                      PkgConfig::DBUS_GLIB-1
                      PkgConfig::GLIB-2.0
                      PkgConfig::DBUS-1
)

