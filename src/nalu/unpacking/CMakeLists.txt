# Glob the source files together
file(GLOB SOURCE_FILES
    *.cc
    
)

# Make an object file
add_library(nalu_unpacking SHARED ${SOURCE_FILES})

# Include directories
target_include_directories(nalu_unpacking
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(nalu_unpacking
    PUBLIC
        common_unpacking
        nalu_data_products
)
