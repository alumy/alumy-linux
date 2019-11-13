set(target card)

FILE (GLOB ${target}_src
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ic/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ic/emv/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ic/mifare/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/msr/*.c
)

add_library(${target} OBJECT ${${target}_src})

