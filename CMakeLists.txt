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

# find_package(Eigen3 REQUIRED)
# include_directories(${Eigen_INCLUDE_DIRS})
# message("$ENV{EIGEN3_INCLUDE_DIR}")
# include_directories(${EIGEN3_INCLUDE_DIR})
# include_directories($ENV{EIGEN3_INCLUDE_DIR})

# Check if the BOOST_1_70_0_ROOT environment variable is defined
# if(DEFINED ENV{BOOST_1_70_0_ROOT})
#     # Append the BOOST_1_70_0_ROOT to CMAKE_PREFIX_PATH
#     list(APPEND CMAKE_PREFIX_PATH "$ENV{BOOST_1_70_0_ROOT}")
# endif()
# find_package(Boost 1.70 REQUIRED COMPONENTS system filesystem)
# find_package(Boost REQUIRED COMPONENTS system filesystem)
# find_package(Boost REQUIRED)
# include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_OSX_ARCHITECTURES "arm64")

#Set the path for where to look for the libraries of the installed target
set(CMAKE_INSTALL_RPATH "${CMAKE_SOURCE_DIR}/lib")

#create installation directories
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#Add all the sub directories
add_subdirectory(external/midas)
add_subdirectory(src)

# add_subdirectory(external/midas)
# add_subdirectory(src/data_products)
# add_subdirectory(src/unpackers)
# add_subdirectory(src/serializer)

# Create the executable
add_executable(unpacker src/main.cc)

# # Enable debugging symbols
# target_compile_options(unpacker PRIVATE -g)

#link libraries
target_link_libraries(unpacker
    PRIVATE 
        data_products
        unpackers
        midas
        # ${Eigen_LIBRARIES}
        # ${ZLIB_LIBRARIES}
        ${ROOT_LIBRARIES}
        # ${Boost_LIBRARIES}
)

#link include directories
target_include_directories(unpacker 
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include/data_products
        ${CMAKE_SOURCE_DIR}/include/unpackers
        ${CMAKE_SOURCE_DIR}/include/parsers
        ${CMAKE_SOURCE_DIR}/include/utils
        ${CMAKE_SOURCE_DIR}/external/midas
        # ${ZLIB_INCLUDE_DIRS}
        ${ROOT_INCLUDE_DIRS}
        # ${Boost_INCLUDE_DIRS}
        # ${EIGEN3_INCLUDE_DIRS}

)

# Move the dictionaries so that they are next to the .so file
add_custom_command(
    TARGET unpacker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy 
    ${CMAKE_BINARY_DIR}/src/data_products/libdata_products_rdict.pcm 
    ${CMAKE_BINARY_DIR}/src/data_products/libdata_products.rootmap
    ${CMAKE_SOURCE_DIR}/lib
)

# Install the libraries
install(TARGETS unpackers
                data_products
                midas 
        LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/lib
)

#Install the executable
install(TARGETS unpacker
        DESTINATION ${CMAKE_SOURCE_DIR}/bin
)

# target_compile_options(unpacker PRIVATE -Wall -Wextra -Wno-undef)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")