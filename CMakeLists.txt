cmake_minimum_required(VERSION 3.5)

project(warface-fastzoom)

add_library(logger libs/logger/src/logger.cpp)
add_library(stringSplit libs/stringUtils/src/stringSplit.cpp)
add_library(stringTrim libs/stringUtils/src/stringTrim.cpp)

target_include_directories(logger PUBLIC ${PROJECT_SOURCE_DIR}/libs/logger/include)
target_include_directories(stringSplit PUBLIC ${PROJECT_SOURCE_DIR}/libs/stringUtils/include)
target_include_directories(stringTrim PUBLIC ${PROJECT_SOURCE_DIR}/libs/stringUtils/include)

set(SOURCES
    src/appSettings.cpp
    src/appCore.cpp
    src/main.cpp
)

add_executable(warface-fastzoom ${SOURCES})

target_include_directories(warface-fastzoom PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(warface-fastzoom 
    PRIVATE 
        -static
        logger
        stringSplit
        stringTrim
)