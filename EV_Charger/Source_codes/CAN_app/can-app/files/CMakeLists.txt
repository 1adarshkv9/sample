cmake_minimum_required(VERSION 3.5)
project(can-app)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/inc}
        )

link_directories(
        ${THISDIR}/../../../dbus-wrapper/0.1-r0/build/
        )

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas -g -D_GNU_SOURCE \
                   -D_USINGLIBC -lrt -Wall -lpthread -g -ljson-c \
                   -I${THISDIR}/../../dbus-wrapper/0.1-r0/inc" 
   )

set(DBUS_WRAPPER "-ldbus-wrapper" )
set(JSON_LIBRARY "-ljson-c" )

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS-1  REQUIRED IMPORTED_TARGET dbus-1)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBSOCKETCAN REQUIRED IMPORTED_TARGET libsocketcan)

add_executable (can_app
           ${CMAKE_CURRENT_SOURCE_DIR}/src/can_app.c
           ${CMAKE_CURRENT_SOURCE_DIR}/src/can.c
       )

include_directories(${JSON_INCLUDE_DIR})

target_link_libraries(can_app PUBLIC
                      ${DBUS_WRAPPER}
                      ${JSON_LIBRARY}
                      PkgConfig::LIBSOCKETCAN
                      PkgConfig::DBUS-1
)
