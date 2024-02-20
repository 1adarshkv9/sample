# Define the library
add_library(DBusListener src/dbus_listener.cpp)

# Include directories
target_include_directories(DBusListener PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add dependencies if any
target_link_libraries(DBusListener YourDependencyLibraries)

# Install rules if needed
install(TARGETS DBusListener DESTINATION lib)

