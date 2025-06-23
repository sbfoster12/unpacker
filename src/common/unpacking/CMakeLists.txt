cmake_minimum_required(VERSION 3.10)

# Glob source files
file(GLOB SOURCE_FILES
    *.cc
)

# Glob headers for ROOT dictionary generation
file(GLOB DICT_HEADERS
    ${PROJECT_SOURCE_DIR}/include/common/unpacking/*.hh
)

# Define include paths
set(INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/external/midas
    ${ROOT_INCLUDE_DIRS}
)

list(REMOVE_DUPLICATES INCLUDE_DIRS)

# Prepare include options for ROOT dictionary generation (add -I prefix)
set(ROOT_DICTIONARY_INCLUDE_OPTIONS "")
foreach(dir ${INCLUDE_DIRS})
    list(APPEND ROOT_DICTIONARY_INCLUDE_OPTIONS "-I${dir}")
endforeach()

# Generate ROOT dictionary
root_generate_dictionary(common_unpacking_dict
    ${DICT_HEADERS}
    MODULE common_unpacking
    LINKDEF ${PROJECT_SOURCE_DIR}/include/common/unpacking/LinkDef.h
    OPTIONS ${ROOT_DICTIONARY_INCLUDE_OPTIONS}
)

# Add library with ROOT dictionary source
add_library(common_unpacking SHARED
    ${SOURCE_FILES}
    common_unpacking_dict.cxx
)

# Set include paths for compilation
target_include_directories(common_unpacking
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/external/midas
        ${ROOT_INCLUDE_DIRS}
)

# Link dependencies
target_link_libraries(common_unpacking
    PUBLIC
        common_data_products
        midas
        nlohmann_json::nlohmann_json
        ${ROOT_LIBRARIES}
)
