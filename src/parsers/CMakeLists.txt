# Glob the source files together
file(GLOB SOURCE_FILES
    *.cc
)

# Make an object file
add_library(obj_parsers STATIC ${SOURCE_FILES})

# Include directories
target_include_directories(obj_parsers
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include/parsers
)

target_link_libraries(obj_parsers
    PUBLIC 
        data_products
)