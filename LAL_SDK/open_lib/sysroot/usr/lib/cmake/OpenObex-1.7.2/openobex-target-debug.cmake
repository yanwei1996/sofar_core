#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "openobex" for configuration "Debug"
set_property(TARGET openobex APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(openobex PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libopenobex.so.1.7.2"
  IMPORTED_SONAME_DEBUG "libopenobex.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS openobex )
list(APPEND _IMPORT_CHECK_FILES_FOR_openobex "${_IMPORT_PREFIX}/lib/libopenobex.so.1.7.2" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
