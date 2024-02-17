cmake_minimum_required(VERSION 3.10)

project(OpenGLSetup)

# Add the source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

# Add executable
add_executable(myProgram ${SOURCE_FILES})

# Include directories
target_include_directories(myProgram PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link with external dependencies
target_link_directories(myProgram PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dep/lib)

# Add additional libraries if needed
target_link_libraries(myProgram your_additional_libraries)

# Set properties and compiler features as needed
set_property(TARGET myProgram PROPERTY CXX_STANDARD 11)

# Other configurations...