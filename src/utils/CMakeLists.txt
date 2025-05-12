# Glob the source files together
file(GLOB SOURCE_FILES
    *.cc
)

# Make an object file
add_library(obj_utils STATIC ${SOURCE_FILES})

# Include directories
target_include_directories(obj_utils
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/utils
)