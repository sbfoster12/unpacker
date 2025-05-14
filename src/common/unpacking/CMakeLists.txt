# Glob the source files together
file(GLOB SOURCE_FILES
    *.cc
)

# Make an object file
add_library(common_unpacking SHARED ${SOURCE_FILES})

# Include directories
target_include_directories(common_unpacking
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/include

)

target_link_libraries(common_unpacking
    PUBLIC
        common_data_products
        midas
)
