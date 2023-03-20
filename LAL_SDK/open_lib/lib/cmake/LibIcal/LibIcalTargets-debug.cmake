#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ical" for configuration "Debug"
set_property(TARGET ical APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(ical PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "-lpthread"
  IMPORTED_LOCATION_DEBUG "/usr/lib/libical.so.1.0.1"
  IMPORTED_SONAME_DEBUG "libical.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS ical )
list(APPEND _IMPORT_CHECK_FILES_FOR_ical "/usr/lib/libical.so.1.0.1" )

# Import target "icalss" for configuration "Debug"
set_property(TARGET icalss APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(icalss PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "ical"
  IMPORTED_LOCATION_DEBUG "/usr/lib/libicalss.so.1.0.1"
  IMPORTED_SONAME_DEBUG "libicalss.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS icalss )
list(APPEND _IMPORT_CHECK_FILES_FOR_icalss "/usr/lib/libicalss.so.1.0.1" )

# Import target "icalvcal" for configuration "Debug"
set_property(TARGET icalvcal APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(icalvcal PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "ical"
  IMPORTED_LOCATION_DEBUG "/usr/lib/libicalvcal.so.1.0.1"
  IMPORTED_SONAME_DEBUG "libicalvcal.so.1"
  )

list(APPEND _IMPORT_CHECK_TARGETS icalvcal )
list(APPEND _IMPORT_CHECK_FILES_FOR_icalvcal "/usr/lib/libicalvcal.so.1.0.1" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
