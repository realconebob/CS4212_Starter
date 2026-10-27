#include <catch2/catch_test_macros.hpp>

#include "SceneLoader.hpp"
#include "cwrender/Scenes/Scene.hpp"

TEST_CASE("SEE IF COMPILES") {
    auto scene = cwrender::Scene<double, 3>{};
    auto loader = SceneLoader{scene};
    loader.reserveCameras(5);
};