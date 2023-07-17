add_library(nu STATIC
    utils.h
    utf8.h
    utf8.c
	nurbt.h
	nurbt.c
    nu.c
    nubool.c
    nunum.c
    nustr.c
    nuarr.c
    nuobj.c
    nuvm.c
    nuvmops.c
)

if(UNIX)
    target_link_libraries(nu m)
endif()

add_subdirectory(tests)