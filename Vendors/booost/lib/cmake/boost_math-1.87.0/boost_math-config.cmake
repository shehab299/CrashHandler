# Generated by Boost 1.87.0

if(TARGET Boost::math)
  return()
endif()

if(Boost_VERBOSE OR Boost_DEBUG)
  message(STATUS "Found boost_math ${boost_math_VERSION} at ${boost_math_DIR}")
endif()

mark_as_advanced(boost_math_DIR)

# Compute the include and library directories relative to this file.

get_filename_component(_BOOST_CMAKEDIR "${CMAKE_CURRENT_LIST_DIR}/../" REALPATH)

# If the computed and the original directories are symlink-equivalent, use original
if(EXISTS "/home/shehab/Desktop/CrashHandler/boost_new/lib/cmake")
  get_filename_component(_BOOST_CMAKEDIR_ORIGINAL "/home/shehab/Desktop/CrashHandler/boost_new/lib/cmake" REALPATH)
  if(_BOOST_CMAKEDIR STREQUAL _BOOST_CMAKEDIR_ORIGINAL)
    set(_BOOST_CMAKEDIR "/home/shehab/Desktop/CrashHandler/boost_new/lib/cmake")
  endif()
  unset(_BOOST_CMAKEDIR_ORIGINAL)
endif()

get_filename_component(_BOOST_INCLUDEDIR "${_BOOST_CMAKEDIR}/../../include/" ABSOLUTE)

add_library(Boost::math INTERFACE IMPORTED)

set_target_properties(Boost::math PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "${_BOOST_INCLUDEDIR}"
)

unset(_BOOST_INCLUDEDIR)
unset(_BOOST_CMAKEDIR)
