set(NU_SRC
    utils.h
    utf8.h
    utf8.c
    nu.c
    nubool.c
    nunum.c
    nustr.c
    nuarr.c
    nuobj.c
    nuvm.c
    nuvmops.c
)

file(REMOVE ${NUVMOPS_INL})
add_library(nu STATIC ${NU_SRC})
add_dependencies(nu NU_GENOPS)

if(UNIX)
    target_link_libraries(nu m)
endif()

target_include_directories(nu PUBLIC "${PROJECT_SOURCE_DIR}/include")

add_subdirectory(tests)