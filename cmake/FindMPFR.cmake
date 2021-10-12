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
  set(HOMEBREW_MPFR_BASE_PATH "/opt/homebrew/Cellar//mpfr/4.1.0")
#  if( ${CMAKE_OSX_ARCHITECTURES} MATCHES "(x86)|(X86)|(x86_64)|(amd64)|(AMD64)" )
#    set(HOMEBREW_MPFR_BASE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../MPFR_x64_64")
#    message(STATUS "Custom path for MPFR set for architecture:${CMAKE_OSX_ARCHITECTURES} at: ${HOMEBREW_MPFR_BASE_PATH}")
#  endif()
  set(HOMEBREW_DIRS "${HOMEBREW_MPFR_BASE_PATH}/include;${HOMEBREW_MPFR_BASE_PATH}/lib;/${HOMEBREW_MPFR_BASE_PATH}/share")
  find_path(MPFR_INCLUDES NAMES mpfr.h mpfr/mpfr.h PATHS ${HOMEBREW_DIRS} $ENV{MPFR_DIR} ${INCLUDE_INSTALL_DIR} NO_DEFAULT_PATH)
  else()
  find_path(MPFR_INCLUDES NAMES mpfr.h mpfr/mpfr.h PATHS ${HOMEBREW_DIRS} $ENV{MPFR_DIR} ${INCLUDE_INSTALL_DIR})
endif()



# Set MPFR_FIND_VERSION to 5.1.0 if no minimum version is specified
if(NOT MPFR_FIND_VERSION)
  if(NOT MPFR_FIND_VERSION_MAJOR)
    set(MPFR_FIND_VERSION_MAJOR 4)
  endif()
  if(NOT MPFR_FIND_VERSION_MINOR)
    set(MPFR_FIND_VERSION_MINOR 0)
  endif()
  if(NOT MPFR_FIND_VERSION_PATCH)
    set(MPFR_FIND_VERSION_PATCH 0)
  endif()
  set(MPFR_FIND_VERSION
    "${MPFR_FIND_VERSION_MAJOR}.${MPFR_FIND_VERSION_MINOR}.${MPFR_FIND_VERSION_PATCH}")
endif()

if(MPFR_INCLUDES)
  # Since the MPFR version macros may be in a file included by MPFR.h of the form
  # MPFR-.*[_]?.*.h (e.g., MPFR-x86_64.h), we search each of them.
  file(GLOB MPFR_HEADERS "${MPFR_INCLUDES}/mpfr.h" "${MPFR_INCLUDES}/*.h")
  foreach(MPFR_header_filename ${MPFR_HEADERS})
    file(READ "${MPFR_header_filename}" _MPFR_version_header)
    string(REGEX MATCH
      "define[ \t]+MPFR_VERSION_MAJOR[ \t]+([0-9]+)" _MPFR_major_version_match
      "${_MPFR_version_header}")
    if(_MPFR_major_version_match)
      set(MPFR_MAJOR_VERSION "${CMAKE_MATCH_1}")
      string(REGEX MATCH "define[ \t]+MPFR_VERSION_MINOR[ \t]+([0-9]+)"
        _MPFR_minor_version_match "${_MPFR_version_header}")
      set(MPFR_MINOR_VERSION "${CMAKE_MATCH_1}")
      string(REGEX MATCH "define[ \t]+MPFR_VERSION_PATCHLEVEL[ \t]+([0-9]+)"
        _MPFR_patchlevel_version_match "${_MPFR_version_header}")
      set(MPFR_PATCHLEVEL_VERSION "${CMAKE_MATCH_1}")
      set(MPFR_VERSION
        ${MPFR_MAJOR_VERSION}.${MPFR_MINOR_VERSION}.${MPFR_PATCHLEVEL_VERSION})
    endif()
  endforeach()

  # Check whether found version exists and exceeds the minimum requirement
  if(NOT MPFR_VERSION)
    set(MPFR_VERSION_OK FALSE)
    message(STATUS "MPFR version was not detected")
  elseif(${MPFR_VERSION} VERSION_LESS ${MPFR_FIND_VERSION})
    set(MPFR_VERSION_OK FALSE)
    message(STATUS "MPFR version ${MPFR_VERSION} found in ${MPFR_INCLUDES}, "
                   "but at least version ${MPFR_FIND_VERSION} is required")
  else()
    message(STATUS "MPFR version ${MPFR_VERSION} found in ${MPFR_INCLUDES}")
    set(MPFR_VERSION_OK TRUE)
  endif()
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin" OR IOS)
    find_library(MPFR_LIBRARIES MPFR PATHS ${HOMEBREW_DIRS} $ENV{MPFR_DIR} ${LIB_INSTALL_DIR} NO_DEFAULT_PATH)
else()
    find_path(MPFR_LIBRARIES NAMES MPFR libmpfr libmpfr.so PATHS $ENV{MPFR_DIR} ${LIB_INSTALL_DIR} /usr/lib/x86_64-linux-gnu/)
endif()

message(STATUS "MPFR_LIBRARIES ${MPFR_LIBRARIES}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MPFR DEFAULT_MSG
                                  MPFR_INCLUDES MPFR_LIBRARIES MPFR_VERSION_OK)
mark_as_advanced(MPFR_INCLUDES MPFR_LIBRARIES)
