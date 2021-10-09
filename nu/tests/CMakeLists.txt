add_executable(
    nutest 
    tests.h
    tests.c
)

target_link_libraries(nutest nu)