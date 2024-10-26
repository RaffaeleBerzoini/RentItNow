# Update a Git submodule.
#
# Args:
#   SUBMODULE_PATH: Path to the submodule within the repository.
#   RECURSIVE_ARG: Optional argument for recursive update (e.g., "--recursive").
#
# Usage Example:
#   update_submodule("ThirdParty/Catch2" "--recursive")
#   update_submodule("ThirdParty/plog" "")
#
# Note: Make sure to call this function within a Git repository.
function(update_submodule SUBMODULE_PATH RECURSIVE_ARG)
    find_package(Git QUIET)

    if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        message(STATUS "Submodule update for ${SUBMODULE_PATH}")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init ${RECURSIVE_ARG} ${SUBMODULE_PATH}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT
        )
        
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init ${RECURSIVE_ARG} failed with ${GIT_SUBMOD_RESULT} for submodule
            ${SUBMODULE_PATH}, please checkout submodules")
        endif()
    endif()
endfunction()