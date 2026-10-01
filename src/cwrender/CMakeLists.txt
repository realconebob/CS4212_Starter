add_library(cwrender
    Helpers.cpp Helpers.hpp
)

target_include_directories(cwrender PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(cwrender PUBLIC glm::glm PNG::PNG)

add_subdirectory(BaseTypes)
add_subdirectory(Hittables)
add_subdirectory(Materials)
add_subdirectory(Scenes)