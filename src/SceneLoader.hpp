/// Borrowed from https://github.com/umdcs4212/JSONSceneLoader

#pragma once

#include "cwrender/Scenes/Scene.hpp"
#include "cwrender/Helpers.hpp"

#include "ISceneLoader.hpp"

#include <cstdio>
#include <string>

using namespace cwrender;
using Scene3D = cwrender::Scene<double, 3>;

class SceneLoader : public ISceneLoader {
    private:
    Scene3D &m_targetScene; // Reference to the external scene

    public:
    int numShaders = -1;
    int numTextures = -1;

    // The caller provides the scene to be filled
    SceneLoader(Scene3D &sceneToPopulate)
        : m_targetScene(sceneToPopulate), numShaders(0), numTextures(0) {}

    #pragma region NOOP Functions
    void reserveCameras(size_t count) override {
        // makes sure the vector has enough space
        // m_targetScene.cameras.reserve(count);
        return;
    }
    void reserveLights(size_t count) override {
        // m_targetScene.lights.reserve(count);
        return;
    }
    void reserveShapes(size_t count) override {
        // m_targetScene.shapes.reserve(count);
        return;
    }
    void reserveShaders(size_t count) override {return;}
    void reserveTextures(size_t count) override {return;}
    #pragma endregion

    void setSceneParams(const SceneParams &sparams) override {
        throw NotImplemented("TODO: Implement Scene Params");
        std::printf("Setting scene params.\n");
    }

    void addCamera(
        const std::string &name, const std::string &type,
        const ISceneLoader::vec &pos, const ISceneLoader::vec &viewDir,
        float focalLength, float imagePlaneWidth
    ) override {
        throw NotImplemented("TODO: Implement Cameras");
        std::printf("Creating camera: %s, type:%s\n", name.c_str(), type.c_str());
        // auto cam = std::make_shared<Camera>();
        // cam->type = type;
        // m_targetScene.cameras.push_back(cam);
    }

    void addPointLight(
        const ISceneLoader::vec &pos,
        const ISceneLoader::vec &intensity
    ) override {
        throw NotImplemented("TODO: Implement Point Lights");
        std::printf("Creating point light.\n");

        // auto light = std::make_shared<Light>();
        // light->type = "Point Light";
        // m_targetScene.lights.push_back(light);
    }

    void addAreaLight(
        const ISceneLoader::vec &pos,
        const ISceneLoader::vec &color,
        const ISceneLoader::vec &normal, float width,
        float length
    ) override {
        throw NotImplemented("TODO: Implement Area Lights");
        std::printf("Creating area light.\n");
    }

    void addShader(const ISceneLoader::ShaderDesc &shaderDesc) override {
        throw NotImplemented("TODO: Implement shaders");
        std::printf("Creating shader: type=%s\n", shaderDesc.type.c_str());
        // I place my shaders on an unordered map often
    }

    void addShape(const ISceneLoader::ShapeDesc &shapeDesc) override {
        throw NotImplemented("TODO: implement shapes");
        std::printf("Creating shape: type=%s\n", shapeDesc.type.c_str());
    }

    void addTexture(
        const std::string &type,
        const std::string &name,
        const std::string &sourceFile
    ) override {
        throw NotImplemented("TODO: implement textures");
        std::printf("Creating texture: type=%s, name=%s, sourceFile=%s\n", type.c_str(), name.c_str(), sourceFile.c_str());
        return;
    }


};
