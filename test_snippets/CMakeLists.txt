#
# Adds the src directory to the include search
# path. ${CMAKE_SOURCE_DIR} is prefixed to allow the files in the
# utest directory to correctly include the relative reference to the
# src files.
#
# include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE TS_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/ts_*.cpp)

foreach(src IN LISTS TS_SOURCES)
  get_filename_component(tsexec ${src} NAME_WE)
  add_executable(${tsexec} ${src})
  target_link_libraries(${tsexec}
    PRIVATE
    cs4212-util
    cwrender
    ${Boost_PROGRAM_OPTIONS_LIBRARIES}
    PNG::PNG
    ZLIB::ZLIB
    OpenMP::OpenMP_CXX
  )
  add_test(NAME ${tsexec} COMMAND ${tsexec})
endforeach()
