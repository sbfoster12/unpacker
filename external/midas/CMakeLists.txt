file(GLOB SOURCE_FILES
    *.cxx
)
    
add_library(midas SHARED    
    ${SOURCE_FILES}
)

target_include_directories(midas
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/external/midas
)

target_link_libraries(midas
    PRIVATE
        ${ZLIB_LIBRARIES}
)