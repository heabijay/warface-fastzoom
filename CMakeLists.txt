project(warface-fastzoom)

set(HEADERS
    logger.h
)

set(LIBRARIES
    logger.cpp
)

add_executable(warface-fastzoom main.cpp ${LIBRARIES} ${HEADERS})