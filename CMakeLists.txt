project(warface-fastzoom)

set(HEADERS
    logger.h
    appCore.h
    appSettings.h
)

set(LIBRARIES
    logger.cpp
    appSettings.cpp
    appCore.cpp
)

add_executable(warface-fastzoom main.cpp ${LIBRARIES} ${HEADERS})