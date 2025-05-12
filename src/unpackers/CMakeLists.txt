# Glob the source files together
file(GLOB SOURCE_FILES
    *.cc
)

# Make an object file
add_library(obj_unpackers STATIC ${SOURCE_FILES})

# Include directories
target_include_directories(obj_unpackers
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include/unpackers

)

target_link_libraries(obj_unpackers
    PUBLIC
        data_products
        obj_parsers
        obj_utils
        midas
)
