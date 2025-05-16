# Glob the source files together
file(GLOB SOURCE_FILES
    *.cc
)

# Make an object file
add_library(nalu_data_products SHARED ${SOURCE_FILES})

# Include directories
target_include_directories(nalu_data_products
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# Libraries
target_link_libraries(nalu_data_products
    PUBLIC
        common_data_products
)

#These are the data products for which we make root dictionaries
set(NaluDataProducts 
    ${CMAKE_SOURCE_DIR}/include/nalu/data_products/*.hh
)

root_generate_dictionary(NaluDataProductDictionary
    ${NaluDataProducts} 
    MODULE nalu_data_products
    LINKDEF ${CMAKE_SOURCE_DIR}/include/nalu/data_products/LinkDef.h
)