# Glob the source files together
file(GLOB SOURCE_FILES
    *.cc
)

# Make an object file
add_library(common_data_products SHARED ${SOURCE_FILES})

# Include directories
target_include_directories(common_data_products
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${ROOT_INCLUDE_DIRS}

)

# Libraries
target_link_libraries(common_data_products
    PUBLIC
        ${ROOT_LIBRARIES}
)

#These are the data products for which we make root dictionaries
set(CommonDataProducts 
    ${CMAKE_SOURCE_DIR}/include/common/data_products/*.hh
)

root_generate_dictionary(CommonDataProductDictionary
    ${CommonDataProducts} 
    MODULE common_data_products
    LINKDEF ${CMAKE_SOURCE_DIR}/include/common/data_products/LinkDef.h
)