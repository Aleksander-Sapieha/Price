# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/sourceguy/Desktop/test/Price/build/_deps/libpsl_src-src"
  "/home/sourceguy/Desktop/test/Price/build/_deps/libpsl_src-build"
  "/home/sourceguy/Desktop/test/Price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix"
  "/home/sourceguy/Desktop/test/Price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/tmp"
  "/home/sourceguy/Desktop/test/Price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp"
  "/home/sourceguy/Desktop/test/Price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src"
  "/home/sourceguy/Desktop/test/Price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/sourceguy/Desktop/test/Price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/sourceguy/Desktop/test/Price/build/_deps/libpsl_src-subbuild/libpsl_src-populate-prefix/src/libpsl_src-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
