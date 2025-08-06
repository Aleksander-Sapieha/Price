#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cpr::cpr" for configuration "Release"
set_property(TARGET cpr::cpr APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(cpr::cpr PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libcpr.so.1.12.0"
  IMPORTED_SONAME_RELEASE "libcpr.so.1"
  )

list(APPEND _cmake_import_check_targets cpr::cpr )
list(APPEND _cmake_import_check_files_for_cpr::cpr "${_IMPORT_PREFIX}/lib/libcpr.so.1.12.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
