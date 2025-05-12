file(GLOB SOURCE_FILES
    *.cc
)

add_library(serializer SHARED  
    ${SOURCE_FILES}
)

target_include_directories(serializer
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(serializer
    PUBLIC
        dataProducts
        ${Boost_LIBRARIES}
)