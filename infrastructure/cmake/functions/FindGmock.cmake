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
        # Search for Debug libs (have 'd' suffix)
        unset(GMOCK_LIB_D CACHE)
        unset(GMOCK_LIB_MAIN_D CACHE)

        find_library(GMOCK_LIB_D NAMES gmockd PATH_SUFFIXES lib)
        find_library(GMOCK_LIB_MAIN_D NAMES gmock_maind PATH_SUFFIXES lib)

        if (GMOCK_LIB_D AND GMOCK_LIB_MAIN_D)
            message(STATUS "Found GMock Debug: ${GMOCK_LIB_D} ${GMOCK_LIB_MAIN_D}")
            set(FOUND_GMOCK_DEBUG TRUE)
            set(LIBS_D debug ${GMOCK_LIB_D})
            set(MAIN_D debug ${GMOCK_LIB_MAIN_D})
        endif()

        # Search for Release libs (don't have 'd' suffix)
        unset(GMOCK_LIB CACHE)
        unset(GMOCK_LIB_MAIN CACHE)

        find_library(GMOCK_LIB NAMES gmock PATH_SUFFIXES lib)
        find_library(GMOCK_LIB_MAIN NAMES gmock_main PATH_SUFFIXES lib)

        if (GMOCK_LIB AND GMOCK_LIB_MAIN)
            message(STATUS "Found GMock Release: ${GMOCK_LIB} ${GMOCK_LIB_MAIN}")
            set(FOUND_GMOCK_RELEASE TRUE)
            set(LIBS optimized ${GMOCK_LIB})
            set(MAIN optimized ${GMOCK_LIB_MAIN})
        endif()

        # If there are neither Debug nor Release it is an error
        if ((NOT FOUND_GMOCK_DEBUG) AND (NOT FOUND_GMOCK_RELEASE))
            message(FATAL_ERROR "Can't find gmock or gmock_main library")
        endif()

        set(GMOCK_LIBRARIES ${LIBS_D} ${LIBS} CACHE INTERNAL "GMOCK_LIBRARIES" FORCE)
        set(GMOCK_MAIN_LIBRARIES ${MAIN_D} ${MAIN} CACHE INTERNAL "GMOCK_MAIN_LIBRARIES" FORCE)
    endif()

    set(GMOCK_BOTH_LIBRARIES ${GMOCK_LIBRARIES} ${GMOCK_MAIN_LIBRARIES} CACHE INTERNAL "GMOCK_BOTH_LIBRARIES")
endfunction(FindGmock)
