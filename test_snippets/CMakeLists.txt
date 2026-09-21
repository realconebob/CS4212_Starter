# 
# Adds the src directory to the include search
# path. ${CMAKE_SOURCE_DIR} is prefixed to allow the files in the
# utest directory to correctly include the relative reference to the
# src files.
#
include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(tsnip_PNGRenderer
  ts_PNGRenderer.cpp
)
target_link_libraries(tsnip_PNGRenderer 
  cs4212-util
  internallib
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
  PNG::PNG
  ZLIB::ZLIB
)

add_executable(tsnip_Camera3D
  ts_3DCameraTest.cpp
)
target_link_libraries(tsnip_Camera3D 
  cs4212-util
  internallib
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
  PNG::PNG
  ZLIB::ZLIB
)