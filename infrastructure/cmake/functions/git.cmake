find_package(Git REQUIRED)

function(git_get_commit_hash GIT_COMMIT_HASH)
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" log -1 --format=%h
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  set(${GIT_COMMIT_HASH} ${COMMIT_HASH} PARENT_SCOPE)
endfunction(git_get_commit_hash)

function(git_get_latest_tag GIT_TAG GIT_TAG_VERSION_MAJOR GIT_TAG_VERSION_MINOR)
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" describe --abbrev=0
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  string(REGEX MATCHALL "[0-9]+" VERSION_LIST ${TAG})

  list(LENGTH VERSION_LIST VERSION_LIST_LENGTH)

  if (${VERSION_LIST_LENGTH} LESS 2)
    message(SEND_ERROR "Failed to parge git tag major and minor version.")
  endif()

  list(GET VERSION_LIST 0 VERSION_MAJOR)
  list(GET VERSION_LIST 1 VERSION_MINOR)

  set(${GIT_TAG} ${TAG} PARENT_SCOPE)
  set(${GIT_TAG_VERSION_MAJOR} ${VERSION_MAJOR} PARENT_SCOPE)
  set(${GIT_TAG_VERSION_MINOR} ${VERSION_MINOR} PARENT_SCOPE)
endfunction(git_get_latest_tag)

function(git_get_number_of_commits_since_tag GIT_NUMBER_OF_COMMITS_SINCE_TAG GIT_TAG)
  execute_process(
    COMMAND "${GIT_EXECUTABLE}" rev-list --no-merges --count HEAD...${GIT_TAG}
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE NUMBER_OF_COMMITS
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )

  set(${GIT_NUMBER_OF_COMMITS_SINCE_TAG} ${NUMBER_OF_COMMITS} PARENT_SCOPE)
endfunction(git_get_number_of_commits_since_tag)
