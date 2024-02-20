# Define the library
add_library(CANReader src/can_read_thread.cpp)

# Include directories
target_include_directories(CANReader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add dependencies if any
target_link_libraries(CANReader YourDependencyLibraries)

# Install rules if needed
install(TARGETS CANReader DESTINATION lib)

