# Example for common_data_products - apply similar pattern to others

# Glob the source files together
file(GLOB SOURCE_FILES
    *.cc
)

# Make an object file
add_library(common_data_products SHARED ${SOURCE_FILES})
add_library(unpacker::common_data_products ALIAS common_data_products)

# Include directories - use generator expressions for build vs install
target_include_directories(common_data_products
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${ROOT_INCLUDE_DIRS}>
)

# Libraries
target_link_libraries(common_data_products
    PUBLIC
        ${ROOT_LIBRARIES}
)

# These are the data products for which we make root dictionaries
set(CommonDataProducts 
    ${CMAKE_SOURCE_DIR}/include/common/data_products/*.hh
)

root_generate_dictionary(CommonDataProductDictionary
    ${CommonDataProducts} 
    MODULE common_data_products
    LINKDEF ${CMAKE_SOURCE_DIR}/include/common/data_products/LinkDef.h
)

# Install the ROOT dictionary files generated for this library
install(
    FILES 
        ${CMAKE_CURRENT_BINARY_DIR}/libcommon_data_products_rdict.pcm
        ${CMAKE_CURRENT_BINARY_DIR}/libcommon_data_products.rootmap
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    OPTIONAL
)