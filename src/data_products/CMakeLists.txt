# Glob the source files together
file(GLOB SOURCE_FILES
    *.cc
)

# Make an object file
add_library(data_products SHARED ${SOURCE_FILES})

# Include directories
target_include_directories(data_products
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/data_products
        ${ROOT_INCLUDE_DIRS}
)

# Libraries
target_link_libraries(data_products
    PUBLIC
        ${ROOT_LIBRARIES}
)

#These are the data products for which we make root dictionaries
set(DataProducts 
    ${CMAKE_SOURCE_DIR}/include/data_products/*.hh
)

root_generate_dictionary(DataProductDictionary
    ${DataProducts} 
    MODULE data_products
    LINKDEF ${CMAKE_SOURCE_DIR}/include/data_products/LinkDef.h
)