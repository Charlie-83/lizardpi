include(FetchContent)

set(WiringPi_SOURCE_DIR ${CMAKE_BINARY_DIR}/wiringpi-src)
set(VERSION 3.14)
FetchContent_Declare(
    WiringPi
    GIT_REPOSITORY https://github.com/WiringPi/WiringPi
    GIT_TAG ${VERSION}
    SOURCE_DIR ${WiringPi_SOURCE_DIR}
)
FetchContent_MakeAvailable(WiringPi)

add_custom_target(buildWiringPi ALL
    COMMAND make -C ${WiringPi_SOURCE_DIR}/wiringPi
    BYPRODUCTS ${WiringPi_SOURCE_DIR}/wiringPi/libwiringPi.so.${VERSION}
)

add_library(wiringPi SHARED IMPORTED)

set_target_properties(wiringPi PROPERTIES
    IMPORTED_LOCATION ${WiringPi_SOURCE_DIR}/wiringPi/libwiringPi.so.${VERSION}
    INTERFACE_INCLUDE_DIRECTORIES ${WiringPi_SOURCE_DIR}/wiringPi
    DEPENDS ${WiringPi_SOURCE_DIR}/wiringPi/libwiringPi.so.${VERSION}
)
