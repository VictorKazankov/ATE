find_package(Git REQUIRED)

# Get commit hash
# GIT_COMMIT_HASH - output variable
# Example usage: git_get_commit_hash(GIT_COMMIT_HASH)
function(git_get_commit_hash GIT_COMMIT_HASH)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" log -1 --format=%h
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    set(${GIT_COMMIT_HASH} ${COMMIT_HASH} PARENT_SCOPE)

endfunction(git_get_commit_hash)

# Parse git tag and get git tag, major version and minor version
# If not tag on repo - set default tag 1.0.0
# Tag must contain version: v-1.0.1, release-1.8.1 ...
# GIT_TAG, GIT_TAG_VERSION_MAJOR, GIT_TAG_VERSION_MINOR - output variables
# Example usage: git_get_latest_tag(GIT_TAG GIT_TAG_VERSION_MAJOR GIT_TAG_VERSION_MINOR)
function(git_get_latest_tag GIT_TAG GIT_TAG_VERSION_MAJOR GIT_TAG_VERSION_MINOR)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" describe --tags --abbrev=0
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE TAG_FROM_GIT
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE EXEC_STATUS
    )

    ##
    ## Parse tag
    ##
    string(REGEX MATCHALL "[0-9]+" VERSION_LIST ${TAG_FROM_GIT})

    list(LENGTH VERSION_LIST VERSION_LIST_LENGTH)

    if (${VERSION_LIST_LENGTH} LESS 2)
        message(SEND_ERROR "Failed to parge git tag major and minor version.")
    endif ()

    list(GET VERSION_LIST 0 VERSION_MAJOR)
    list(GET VERSION_LIST 1 VERSION_MINOR)

    set(${GIT_TAG} ${TAG_FROM_GIT} PARENT_SCOPE)
    set(${GIT_TAG_VERSION_MAJOR} ${VERSION_MAJOR} PARENT_SCOPE)
    set(${GIT_TAG_VERSION_MINOR} ${VERSION_MINOR} PARENT_SCOPE)
endfunction(git_get_latest_tag)

# Get count commit from last tag
# GIT_NUMBER_OF_COMMITS_SINCE_TAG - output variable
# GIT_TAG - input variable contain git tag
# Example usage: git_get_number_of_commits_since_tag(GIT_COMMITS_SINCE_TAG ${GIT_TAG})
function(git_get_number_of_commits_since_tag GIT_NUMBER_OF_COMMITS_SINCE_TAG GIT_TAG)
    if(NOT ${GIT_TAG} STREQUAL "")
        set(COUNT_TO "...${GIT_TAG}")
    endif()

    execute_process(
        COMMAND "${GIT_EXECUTABLE}" rev-list --no-merges --count HEAD${COUNT_TO}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_VARIABLE NUMBER_OF_COMMITS
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE EXEC_STATUS
    )

    if (EXEC_STATUS EQUAL "0")
        set(${GIT_NUMBER_OF_COMMITS_SINCE_TAG} ${NUMBER_OF_COMMITS} PARENT_SCOPE)
    else ()
        set(${GIT_NUMBER_OF_COMMITS_SINCE_TAG} "0" PARENT_SCOPE)
    endif ()

endfunction(git_get_number_of_commits_since_tag)

# Sets GIT_TAG_EXISTS to TRUE if git tag command list some tags
# GIT_TAG_EXISTS - TRUE if git tag lists any tags
# Example usage: git_check_tags_exists(GIT_TAG_EXISTS)
function(git_check_tags_exists GIT_TAG_EXISTS)
    execute_process(
        COMMAND "${GIT_EXECUTABLE}" describe --tags
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
        OUTPUT_QUIET
        ERROR_QUIET
        RESULT_VARIABLE ERROR_CODE
    )

    if (${ERROR_CODE} EQUAL 0)
        set(${GIT_TAG_EXISTS} TRUE PARENT_SCOPE)
    else()
        set(${GIT_TAG_EXISTS} FALSE PARENT_SCOPE)
    endif()
endfunction(git_check_tags_exists)

# Get version components based on git tag.
# Please not that it works with annotated tags only.
# If no tags are visible, return version 1.0 and overall number of commits
# GIT_TAG_VERSION_MAJOR - major part of version
# GIT_TAG_VERSION_MINOR - minor part of version
# GIT_NUMBER_OF_COMMITS_SINCE_TAG - number commits since tag or overall
# GIT_COMMIT_HASH - latest commit hash
# Example usage: git_get_version(GIT_TAG_VERSION_MAJOR GIT_TAG_VERSION_MINOR GIT_NUMBER_OF_COMMITS_SINCE_TAG GIT_COMMIT_HASH)
function(git_get_version GIT_TAG_VERSION_MAJOR GIT_TAG_VERSION_MINOR GIT_NUMBER_OF_COMMITS_SINCE_TAG GIT_COMMIT_HASH)
    git_check_tags_exists(TAG_EXISTS)

    if (${TAG_EXISTS})
        git_get_latest_tag(TAG VERSION_MAJOR VERSION_MINOR)
        git_get_number_of_commits_since_tag(COMMITS_SINCE_TAG ${TAG})

        message(STATUS "git: Most recent visible tag: " ${TAG})
        message(STATUS "git: Number of commits: " ${COMMITS_SINCE_TAG})
    else()
        git_get_number_of_commits_since_tag(COMMITS_SINCE_TAG "")

        set(VERSION_MAJOR "1")
        set(VERSION_MINOR "0")

        message(STATUS "git: No git tag is visible. Will assume version 1.0")
        message(STATUS "git: Overall number of commits: " ${COMMITS_SINCE_TAG})
    endif()

    git_get_commit_hash(COMMIT_HASH)
    message(STATUS "git: Latest commit hash: " ${COMMIT_HASH})

    set(${GIT_TAG_VERSION_MAJOR} ${VERSION_MAJOR} PARENT_SCOPE)
    set(${GIT_TAG_VERSION_MINOR} ${VERSION_MINOR} PARENT_SCOPE)
    set(${GIT_NUMBER_OF_COMMITS_SINCE_TAG} ${COMMITS_SINCE_TAG} PARENT_SCOPE)
    set(${GIT_COMMIT_HASH} ${COMMIT_HASH} PARENT_SCOPE)
endfunction(git_get_version)
