project(warface-fastzoom)

set(HEADERS
    logger.h
    stringSplit.h
    stringTrim.h
    appCore.h
    appSettings.h
)

set(LIBRARIES
    logger.cpp
    stringSplit.cpp
    stringTrim.cpp
    appSettings.cpp
    appCore.cpp
)

add_executable(warface-fastzoom main.cpp ${LIBRARIES} ${HEADERS})