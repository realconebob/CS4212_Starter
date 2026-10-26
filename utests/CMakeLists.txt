#
# Adds the src directory to the include search
# path. ${CMAKE_SOURCE_DIR} is prefixed to allow the files in the
# utest directory to correctly include the relative reference to the
# src files.
#
include_directories(${CMAKE_SOURCE_DIR}/src)

enable_testing()
include(CTest)
include(Catch)

#
# This is a simple way to set the names of the executables used for
# the unit tests. Note that we are setting a variable, called
# ${UTESTS} to be the set of unit test executables. Then, in the next
# step, we iterate over all the elements within the ${UTESTS} set to
# compile into an executable.

#
# For each of the executables named in ${UTESTS}, compile them into a
# boost unit test for use by the CMake CTest system.
#
# foreach(uexec ${UTESTS})
#   add_executable(${uexec} ${uexec}.cpp)
#   target_link_libraries(${uexec}
# 	PRIVATE
# 	Catch2::Catch2WithMain
# 	cs4212-util
#   cwrender
#   OpenMP::OpenMP_CXX
#   )

#   # add_test(NAME ${uexec} COMMAND $<TARGET_FILE:${uexec}>)
#   catch_discover_tests(${uexec})
# endforeach(uexec)

file(GLOB_RECURSE UT_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ut_*.cpp)

foreach(src IN LISTS UT_SOURCES)
  get_filename_component(utexec ${src} NAME_WE)
  add_executable(${utexec} ${src})
  target_link_libraries(${utexec}
    PRIVATE
    Catch2::Catch2WithMain
    cs4212-util
    cwrender
    ${Boost_PROGRAM_OPTIONS_LIBRARIES}
    PNG::PNG
    ZLIB::ZLIB
    OpenMP::OpenMP_CXX
  )
  add_test(NAME ${utexec} COMMAND $<TARGET_FILE:${utexec}>)
  catch_discover_tests(${utexec})
endforeach()
