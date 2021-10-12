# Try to find the GMP library
# https://gmplib.org/
#
# This module supports requiring a minimum version, e.g. you can do
#   find_package(GMP 6.0.0)
# to require version 6.0.0 to newer of GMP.
#
# Once done this will define
#
#  GMP_FOUND - system has GMP lib with correct version
#  GMP_INCLUDES - the GMP include directory
#  GMP_LIBRARIES - the GMP library
#  GMP_VERSION - GMP version

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR IOS)
  message( STATUS "Architecture: ${CMAKE_OSX_ARCHITECTURES}" )
  set(HOMEBREW_FLINT_BASE_PATH "/opt/homebrew/Cellar/flint/2.8.0")
#  if( ${CMAKE_OSX_ARCHITECTURES} MATCHES "(x86)|(X86)|(x86_64)|(amd64)|(AMD64)" )
#    set(HOMEBREW_FLINT_BASE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../flint_x64_64")
#    message(STATUS "Custom path for FLINT set for architecture:${CMAKE_OSX_ARCHITECTURES} at: ${HOMEBREW_FLINT_BASE_PATH}")
#  endif()
  set(HOMEBREW_DIRS "${HOMEBREW_FLINT_BASE_PATH}/include;${HOMEBREW_FLINT_BASE_PATH}/lib;/${HOMEBREW_FLINT_BASE_PATH}/share")
  find_path(FLINT_INCLUDES NAMES flint.h flint/flint.h PATHS ${HOMEBREW_DIRS} $ENV{FLINT_DIR} ${INCLUDE_INSTALL_DIR} NO_DEFAULT_PATH)
  else()
  find_path(FLINT_INCLUDES NAMES flint.h flint/flint.h PATHS ${HOMEBREW_DIRS} $ENV{FLINT_DIR} ${INCLUDE_INSTALL_DIR})

endif()


# Set FLINT_FIND_VERSION to 5.1.0 if no minimum version is specified
if(NOT FLINT_FIND_VERSION)
  if(NOT FLINT_FIND_VERSION_MAJOR)
    set(FLINT_FIND_VERSION_MAJOR 2)
  endif()
  if(NOT FLINT_FIND_VERSION_MINOR)
    set(FLINT_FIND_VERSION_MINOR 5)
  endif()
  if(NOT FLINT_FIND_VERSION_PATCH)
    set(FLINT_FIND_VERSION_PATCH 0)
  endif()
  set(FLINT_FIND_VERSION
    "${FLINT_FIND_VERSION_MAJOR}.${FLINT_FIND_VERSION_MINOR}.${FLINT_FIND_VERSION_PATCH}")
endif()

if(FLINT_INCLUDES)
  # Since the FLINT version macros may be in a file included by FLINT.h of the form
  # FLINT-.*[_]?.*.h (e.g., FLINT-x86_64.h), we search each of them.
  file(GLOB FLINT_HEADERS "${FLINT_INCLUDES}/flint/flint.h" "${FLINT_INCLUDES}/flint/*.h")
  foreach(FLINT_header_filename ${FLINT_HEADERS})
    file(READ "${FLINT_header_filename}" _FLINT_version_header)
    string(REGEX MATCH
      "define[ \t]+__FLINT_VERSION[ \t]+([0-9]+)" _FLINT_major_version_match
      "${_FLINT_version_header}")
    if(_FLINT_major_version_match)
      set(FLINT_MAJOR_VERSION "${CMAKE_MATCH_1}")
      string(REGEX MATCH "define[ \t]+__FLINT_VERSION_MINOR[ \t]+([0-9]+)"
        _FLINT_minor_version_match "${_FLINT_version_header}")
      set(FLINT_MINOR_VERSION "${CMAKE_MATCH_1}")
      string(REGEX MATCH "define[ \t]+__FLINT_VERSION_PATCHLEVEL[ \t]+([0-9]+)"
        _FLINT_patchlevel_version_match "${_FLINT_version_header}")
      set(FLINT_PATCHLEVEL_VERSION "${CMAKE_MATCH_1}")
      set(FLINT_VERSION
        ${FLINT_MAJOR_VERSION}.${FLINT_MINOR_VERSION}.${FLINT_PATCHLEVEL_VERSION})
    endif()
  endforeach()

  # Check whether found version exists and exceeds the minimum requirement
  if(NOT FLINT_VERSION)
    set(FLINT_VERSION_OK FALSE)
    message(STATUS "FLINT version was not detected")
  elseif(${FLINT_VERSION} VERSION_LESS ${FLINT_FIND_VERSION})
    set(FLINT_VERSION_OK FALSE)
    message(STATUS "FLINT version ${FLINT_VERSION} found in ${FLINT_INCLUDES}, "
                   "but at least version ${FLINT_FIND_VERSION} is required")
  else()
    message(STATUS "FLINT version ${FLINT_VERSION} found in ${FLINT_INCLUDES}")
    set(FLINT_VERSION_OK TRUE)
  endif()
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR IOS)
    find_library(FLINT_LIBRARIES FLINT PATHS ${HOMEBREW_DIRS} $ENV{FLINT_DIR} ${LIB_INSTALL_DIR} NO_DEFAULT_PATH)
else()
    find_path(FLINT_LIBRARIES NAMES FLINT libflint libflint.so PATHS $ENV{FLINT_DIR} ${LIB_INSTALL_DIR} "/usr/lib/x86_64-linux-gnu/")
endif()

message(STATUS "FLINT_LIBRARIES ${FLINT_LIBRARIES}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FLINT DEFAULT_MSG
                                  FLINT_INCLUDES FLINT_LIBRARIES FLINT_VERSION_OK)
mark_as_advanced(FLINT_INCLUDES FLINT_LIBRARIES)
