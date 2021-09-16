add_library(
    nu STATIC
    utils.h
    utf8.h
    utf8.c
	nurbt.h
	nurbt.c
    nu.h
    nu.c
    nubase.c
    nubool.c
    nunum.c
    nustr.c
    nuobj.c
)

target_link_libraries(nu m)