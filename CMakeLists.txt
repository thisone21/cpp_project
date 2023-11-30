cmake_minimum_required(VERSION 3.0)
project(openaicpp)

include_directories("include/openai")
#ADD_EXECUTABLE(main.out source.cpp)
#add_subdirectory(examples)
cmake_minimum_required(VERSION 3.0)

project(examples)

# You can comment/Uncomment this line if you wish to hide/show verbose output
# add_definitions(-DOPENAI_VERBOSE_OUTPUT=1)

option(CURL_STATIC_LINKING "Set to ON to build libcurl with static linking."  OFF)
  
if(CURL_STATIC_LINKING)
    message("-DCURL_STATICLIB [added]")
    add_definitions(-DCURL_STATICLIB)
endif()

add_definitions(-DJSON_USE_IMPLICIT_CONVERSIONS=0)

# SET(CURL_MIN_VERSION "7.61.0")
find_package(CURL REQUIRED)

include_directories(${CURL_INCLUDE_DIRS})

message("Using compiler ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# if ("${CMAKE_MAJOR_VERSION}${CMAKE_MINOR_VERSION}" LESS 31)
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# endif()

set (SOURCES_EXAMPLES
    project
)

set (TARGETS_EXAMPLES
    project
)

foreach( name ${TARGETS_EXAMPLES} )
    add_executable(${name} ${name}.cpp)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${name} PROPERTY CXX_STANDARD_REQUIRED ON)
    target_compile_options(${name} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
    )
    target_link_libraries(${name} ${CURL_LIBRARIES})
 endforeach()

