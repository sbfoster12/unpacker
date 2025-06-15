project(unpacker)

cmake_minimum_required(VERSION 3.13)

# for debug
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

# ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include("${ROOT_DIR}/RootMacros.cmake")

# ZLIB
find_package(ZLIB REQUIRED)

# JSON
find_package(nlohmann_json REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_ARCHITECTURES "arm64")

# Set the path for where to look for the libraries of the installed target
set(CMAKE_INSTALL_RPATH "${CMAKE_SOURCE_DIR}/lib")

# Create installation directories
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Add all the subdirectories
add_subdirectory(external/midas)
add_subdirectory(src)

# Create the executable
add_executable(unpacker src/main.cc)

# Link libraries
target_link_libraries(unpacker
    PRIVATE 
        common_data_products
        common_unpacking 
        nalu_data_products
        nalu_unpacking
        midas
        nlohmann_json::nlohmann_json
        ${ROOT_LIBRARIES}
)

# Move the dictionaries so that they are next to the .so files
add_custom_command(
    TARGET unpacker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_BINARY_DIR}/src/common/data_products/libcommon_data_products_rdict.pcm
        ${CMAKE_BINARY_DIR}/src/common/data_products/libcommon_data_products.rootmap
        ${CMAKE_BINARY_DIR}/src/nalu/data_products/libnalu_data_products_rdict.pcm
        ${CMAKE_BINARY_DIR}/src/nalu/data_products/libnalu_data_products.rootmap
        ${CMAKE_BINARY_DIR}/src/common/unpacking/libcommon_unpacking_rdict.pcm
        ${CMAKE_BINARY_DIR}/src/common/unpacking/libcommon_unpacking.rootmap
        ${CMAKE_BINARY_DIR}/src/nalu/unpacking/libnalu_unpacking_rdict.pcm
        ${CMAKE_BINARY_DIR}/src/nalu/unpacking/libnalu_unpacking.rootmap
        ${CMAKE_SOURCE_DIR}/lib
)

# Install the libraries (including unpacking libs)
install(TARGETS 
            common_unpacking
            common_data_products
            midas
            nalu_unpacking
            nalu_data_products
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/lib
)

# Install the executable
install(TARGETS unpacker
        DESTINATION ${CMAKE_SOURCE_DIR}/bin
)

# # Enable debugging symbols
# target_compile_options(unpacker PRIVATE -g -v)

# target_compile_options(unpacker PRIVATE -Wall -Wextra -Wno-undef)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")
