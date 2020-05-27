list(TRANSFORM JGE_EXAMPLES TOLOWER)

foreach(dir IN LISTS JGE_EXAMPLES)
    add_subdirectory(${dir})
endforeach()
