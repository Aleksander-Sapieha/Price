# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/home/sourceguy/price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp/libpsl_src-populate-gitclone-lastrun.txt" AND EXISTS "/home/sourceguy/price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp/libpsl_src-populate-gitinfo.txt" AND
  "/home/sourceguy/price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp/libpsl_src-populate-gitclone-lastrun.txt" IS_NEWER_THAN "/home/sourceguy/price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp/libpsl_src-populate-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/home/sourceguy/price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp/libpsl_src-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/home/sourceguy/price/build/_deps/libpsl_src-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/home/sourceguy/price/build/_deps/libpsl_src-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/usr/bin/git"
            clone --no-checkout --config "advice.detachedHead=false" "https://github.com/rockdaboot/libpsl.git" "libpsl_src-src"
    WORKING_DIRECTORY "/home/sourceguy/price/build/_deps"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/rockdaboot/libpsl.git'")
endif()

execute_process(
  COMMAND "/usr/bin/git"
          checkout "0.21.5" --
  WORKING_DIRECTORY "/home/sourceguy/price/build/_deps/libpsl_src-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '0.21.5'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/home/sourceguy/price/build/_deps/libpsl_src-src"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/home/sourceguy/price/build/_deps/libpsl_src-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/home/sourceguy/price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp/libpsl_src-populate-gitinfo.txt" "/home/sourceguy/price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp/libpsl_src-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/home/sourceguy/price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp/libpsl_src-populate-gitclone-lastrun.txt'")
endif()
