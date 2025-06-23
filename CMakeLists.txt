cmake_minimum_required(VERSION 3.13)
project(unpacker VERSION 1.0 LANGUAGES CXX)

# --- Compiler & build settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# --- Dependencies ---
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
include("${ROOT_DIR}/RootMacros.cmake")

find_package(ZLIB REQUIRED)
find_package(nlohmann_json 3.11.2 REQUIRED CONFIG)

# Optional macOS arch override
set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Mac architecture")

# --- Subdirectories ---
add_subdirectory(external/midas)
add_subdirectory(src)

# --- Executable ---
add_executable(unpacker src/main.cc)
target_link_libraries(unpacker
    PUBLIC 
        common_data_products
        common_unpacking
        nalu_data_products
        nalu_unpacking
        midas
        nlohmann_json::nlohmann_json
        ${ROOT_LIBRARIES}
)

# --- Install targets ---
install(TARGETS
    common_unpacking
    common_data_products
    midas
    nalu_unpacking
    nalu_data_products
    EXPORT unpackerTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS unpacker
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# --- Install headers ---
install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# --- Export targets for find_package ---
install(EXPORT unpackerTargets
    FILE unpackerTargets.cmake
    NAMESPACE unpacker::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/unpacker
)

# --- Package config and version files ---
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/unpackerConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/unpackerConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/unpacker
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/unpackerConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/unpackerConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/unpacker
)

# --- RPATH for local shared lib resolution ---
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib")
