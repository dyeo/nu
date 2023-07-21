if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

cmake_minimum_required(VERSION 3.10)
project(nu C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED YES)

set(GENOPS_PY "${CMAKE_CURRENT_SOURCE_DIR}/tools/genops.py")
set(NUVMOPS_INL "${CMAKE_CURRENT_SOURCE_DIR}/nu/nuvmops.inl")
add_custom_command(
    OUTPUT ${NUVMOPS_INL}
    COMMAND ${CMAKE_COMMAND} -E env python ${GENOPS_PY} > ${NUVMOPS_INL}
    DEPENDS ${GENOPS_PY}
    DEPENDS NU_GENOPS
    COMMENT "Generating code..."
    VERBATIM
)

add_custom_target(NU_GENOPS DEPENDS ${NUVMOPS_INL})

add_subdirectory(nu)