add_subdirectory(data_products)
add_subdirectory(parsers)
add_subdirectory(unpackers)
add_subdirectory(utils)

# Make the unpackers library
add_library(unpackers INTERFACE)

# Link the relevant static libraries
target_link_libraries(unpackers
    INTERFACE
        obj_utils
        obj_parsers
        obj_unpackers
)
