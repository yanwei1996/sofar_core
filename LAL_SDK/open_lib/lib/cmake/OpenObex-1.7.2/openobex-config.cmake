# - Config file for the OpenObex package
# It defines the following variables
#  OpenObex_INCLUDE_DIRS - include directories for FooBar
#  OpenObex_LIBRARIES    - libraries to link against

get_filename_component(OpenObex_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
if (EXISTS "${OpenObex_CMAKE_DIR}/lib/openobex-build.cmake")
  #in build tree
  include(${OpenObex_CMAKE_DIR}/lib/openobex-build.cmake)
  include(${OpenObex_CMAKE_DIR}/lib/openobex-build-settings.cmake)

  set(OpenObex_INCLUDE_DIRS
    ${OpenObex_CMAKE_DIR}/include
    ${OpenObex_SOURCE_DIR}/include
  )

else ()
  #in installed tree
  include(${OpenObex_CMAKE_DIR}/openobex-target.cmake)

  set(OpenObex_CMAKE_REL_INCLUDE_DIR "../../../include")
  get_filename_component(OpenObex_ABS_INCLUDE_DIR
    "${OpenObex_CMAKE_DIR}/${OpenObex_CMAKE_REL_INCLUDE_DIR}" ABSOLUTE)

  set(OpenObex_INCLUDE_DIRS "${OpenObex_ABS_INCLUDE_DIR}")
endif()

set(OpenObex_LIBRARIES openobex)
