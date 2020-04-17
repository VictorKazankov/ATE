# Find gmock libraries
#   GMOCK_BOTH_LIBRARIES - Both libgmock & libgmock_main
#   GMOCK_LIBRARIES - libgmock
#   GMOCK_MAIN_LIBRARIES - libgmock-main
# Example Usage:
#
#    FindGmock()
#    add_executable(foo foo.cc)
#    target_link_libraries(foo ${GMOCK_BOTH_LIBRARIES})
#
function(FindGmock)
    if (NOT MSVC)
        find_library(GMOCK_LIBRARY
            NAMES gmock
            libgmock
            PATH_SUFFIXES lib)

        if (${GMOCK_LIBRARY} STREQUAL "GMOCK_LIBRARY-NOTFOUND")
            message(FATAL_ERROR "Can't find gmock library")
        endif()

        find_library(GMOCK_LIBRARY_MAIN
            NAMES gmock_main
            libgmock_main
            PATH_SUFFIXES lib)

        if (${GMOCK_LIBRARY_MAIN} STREQUAL "GMOCK_LIBRARY_MAIN-NOTFOUND")
            message(FATAL_ERROR "Can't find gmock_main library")
        endif()

        set(GMOCK_LIBRARIES ${GMOCK_LIBRARY} CACHE INTERNAL "GMOCK_LIBRARIES")
        set(GMOCK_MAIN_LIBRARIES ${GMOCK_LIBRARY_MAIN} CACHE INTERNAL "GMOCK_MAIN_LIBRARIES")
    else()
        find_library(GMOCK_LIB_D NAMES gmockd PATH_SUFFIXES lib)
        find_library(GMOCK_LIB NAMES gmock PATH_SUFFIXES lib)
        find_library(GMOCK_LIB_MAIN_D NAMES gmock_maind PATH_SUFFIXES lib)
        find_library(GMOCK_LIB_MAIN NAMES gmock_main PATH_SUFFIXES lib)

        if (${GMOCK_LIB_D} MATCHES ".*-NOTFOUND" OR ${GMOCK_LIB} MATCHES ".*-NOTFOUND")
            message(FATAL_ERROR "Can't find gmock library")
        endif()

        if (${GMOCK_LIB_MAIN_D} MATCHES ".*-NOTFOUND" OR ${GMOCK_LIB_MAIN} MATCHES ".*-NOTFOUND")
            message(FATAL_ERROR "Can't find gmock_main library")
        endif()

        set(GMOCK_LIBRARIES debug ${GMOCK_LIB_D} optimized ${GMOCK_LIB} CACHE INTERNAL "GMOCK_LIBRARIES")
        set(GMOCK_MAIN_LIBRARIES debug ${GMOCK_LIB_MAIN_D} optimized ${GMOCK_LIB_MAIN} CACHE INTERNAL "GMOCK_MAIN_LIBRARIES")
    endif()

    set(GMOCK_BOTH_LIBRARIES ${GMOCK_LIBRARIES} ${GMOCK_MAIN_LIBRARIES} CACHE INTERNAL "GMOCK_BOTH_LIBRARIES")
endfunction(FindGmock)
