#pragma once

#include "cwrender/Helpers.hpp"
#include "cwrender/BaseTypes/BaseTypes.hpp"
#include "cwrender/Hittables/Hittables.hpp"
#include "cwrender/Materials/Materials.hpp"
#include "cwrender/Scenes/Scenes.hpp"

#include "ISceneLoader.hpp"

#include <cmath>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace cwrender {
using Scene3D = Scene<double, 3>;

// Usage:
//   Scene3D scene;
//   auto loader = std::make_shared<SceneLoader>(scene);
//   SceneParser_JSON(loader).parseFileData(path);
//   loader->finalize();            // <-- nothing reaches `scene` until this
//
// The ISceneLoader callbacks only RECORD descriptors. The parser calls them in
// the order camera -> light -> shader -> shape -> texture, so shaders arrive
// before the textures they name. Building during the callbacks would hit
// unresolved references; finalize() builds everything once all names exist.
class SceneLoader : public ISceneLoader {
public:
    using Vec3 = VecX<double, 3>;
    using MatPtr = std::shared_ptr<MaterialAny<double, 3>>;

    struct CameraDesc {
        std::string name, type;
        Vec3 position, viewDir; // viewDir is NOT normalized, and there is no up vector
        double focalLength, imagePlaneWidth;

        // Horizontal FOV in radians, from the file's focal length / image plane width.
        double horizontalFov() const { return 2.0 * std::atan(imagePlaneWidth / (2.0 * focalLength)); }
    };

    // strict = true : unsupported features throw NotImplemented (recommended while developing)
    // strict = false: warn on stderr and approximate/skip, so a scene still renders
    explicit SceneLoader(Scene3D &sceneToPopulate, bool strict = true)
        : m_scene(sceneToPopulate), m_strict(strict) {}

    // Read these after finalize()
    const std::vector<CameraDesc> &cameras() const { return m_cameras; }
    // Build a PerspectiveCamera3 from camera #index in the scene file.
    // The file has no resolution or aspect ratio, so the caller supplies them.
    // Samples/grid, etc. (e.g. camera.samplegrid) are still set by the caller afterwards.
    PerspectiveCamera3<double> makeCamera(double aspect, int imageWidth, size_t index = 0) const {
        if (index >= m_cameras.size())
            throw std::runtime_error("Scene file has no camera #" + std::to_string(index));
        const CameraDesc &c = m_cameras[index];
        if (c.type != "perspective")
            unsupported("camera type '" + c.type + "' ('" + c.name + "')");

        // The file gives a focal length and an image-plane *width*; cwrender wants a vertical FOV in degrees.
        // tan(vfov/2) = (width / (2 * focal)) / aspect
        const double pi = std::acos(-1.0);
        double vfovDeg = 2.0 * std::atan(c.imagePlaneWidth / (2.0 * c.focalLength * aspect)) * 180.0 / pi;

        PerspectiveCamera3<double> cam{
            c.position, c.position + c.viewDir, Vec3(0.0, 1.0, 0.0), aspect, vfovDeg, imageWidth};
        cam.set_fov(vfovDeg); // same setter your main() uses, in case the ctor stores the raw value
        return cam;
    }

    Vec3 backgroundColor() const { return toVec(m_params.bgColor); }
    const std::string &envMapFile() const { return m_params.envMapFile; }

    // ---------------------------------------------------------------- callbacks
    void reserveCameras(size_t) override {}
    void reserveLights(size_t) override {}
    void reserveShapes(size_t) override {}
    void reserveShaders(size_t) override {}
    void reserveTextures(size_t) override {}

    void setSceneParams(const SceneParams &p) override { m_params = p; }

    void addCamera(const std::string &name, const std::string &type,
                   const vec &pos, const vec &viewDir,
                   float focalLength, float imagePlaneWidth) override {
        m_cameras.push_back({name, type, toVec(pos), toVec(viewDir), focalLength, imagePlaneWidth});
    }

    void addPointLight(const vec &pos, const vec &intensity) override {
        m_pointLights.push_back({toVec(pos), toVec(intensity)});
    }

    void addAreaLight(const vec &, const vec &, const vec &, float, float) override {
        ++m_areaLightCount; // no area light type in cwrender yet; reported in finalize()
    }

    void addShader(const ShaderDesc &d) override { m_shaders[d.name] = d; }

    void addTexture(const std::string &type, const std::string &name,
                    const std::string &sourceFile) override {
        m_textures[name] = {type, sourceFile};
    }

    void addShape(const ShapeDesc &d) override { m_shapes.push_back(d); }

    // ----------------------------------------------------------------- build
    void finalize() {
        if (m_finalized) throw std::logic_error("SceneLoader::finalize() called twice");
        m_finalized = true;

        if (m_areaLightCount > 0) unsupported("area lights (" + std::to_string(m_areaLightCount) + " in file)");
        if (!m_params.envMapFile.empty()) unsupported("environment maps");

        for (const auto &l : m_pointLights)
            m_scene.light_back(PointLight<double, 3>{l.pos, l.color}.any());

        for (const auto &s : m_shapes)
            addHittable(s);
    }

private:
    struct PointLightRec { Vec3 pos, color; };
    struct TextureRec { std::string type, file; };

    Scene3D &m_scene;
    bool m_strict;
    bool m_finalized = false;

    SceneParams m_params;
    std::vector<CameraDesc> m_cameras;
    std::vector<PointLightRec> m_pointLights;
    int m_areaLightCount = 0;
    std::unordered_map<std::string, ShaderDesc> m_shaders;
    std::unordered_map<std::string, TextureRec> m_textures;
    std::vector<ShapeDesc> m_shapes;
    std::unordered_map<std::string, MatPtr> m_materials; // built lazily, shared between shapes

    static Vec3 toVec(const vec &v) { return Vec3(v.x, v.y, v.z); }

    void unsupported(const std::string &what) const {
        if (m_strict) throw NotImplemented(("SceneLoader: unsupported " + what).c_str());
        std::fprintf(stderr, "SceneLoader: unsupported %s (skipped/approximated)\n", what.c_str());
    }

    // ------------------------------------------------------------- materials
    MatPtr material(const std::string &shaderName) {
        if (auto it = m_materials.find(shaderName); it != m_materials.end())
            return it->second;

        auto sd = m_shaders.find(shaderName);
        if (sd == m_shaders.end())
            throw std::runtime_error("Shape references unknown shader '" + shaderName + "'");

        auto mat = std::make_shared<MaterialAny<double, 3>>(buildMaterial(sd->second));
        m_materials[shaderName] = mat;
        return mat;
    }

    MaterialAny<double, 3> buildMaterial(const ShaderDesc &d) {
        // Textures: validate the reference, but cwrender has no textured albedo yet.
        for (const ShaderProperty *p : {&d.diffuse, &d.specular, &d.normal, &d.emission}) {
            if (!p->hasTexture) continue;
            if (!m_textures.count(p->textureName))
                throw std::runtime_error("Shader '" + d.name + "' references unknown texture '" + p->textureName + "'");
            unsupported("textured shader '" + d.name + "'");
        }

        const std::string &t = d.type;

        // TODO: confirm "Diffuse" should map to Lambertian rather than DiffuseShader
        if (t == "Lambertian" || t == "Diffuse")
            return LambertianShader<double, 3>{toVec(d.diffuse.data)}.any();

        // Phong and BlinnPhong both land on BlinnPhongShader
        if (t == "BlinnPhong" || t == "Phong")
            return BlinnPhongShader<double, 3>{toVec(d.specular.data), double(d.phongExp), toVec(d.diffuse.data)}.any();

        // "Mirror" has no albedo field in the file: perfect white reflector, roughness -> fuzz
        if (t == "Mirror")
            return MetalShader<double, 3>{Vec3::Ones(), double(d.roughness)}.any();

        // Glaze, Dielectric, BlinnPhongMirrored, CoolToWarm, Emitter, unknown
        unsupported("shader type '" + t + "' (shader '" + d.name + "')");
        return LambertianShader<double, 3>{Vec3(0.5, 0.5, 0.5)}.any(); // non-strict fallback
    }

    // --------------------------------------------------------------- shapes
    // cwrender hittables have no transform of their own, so xforms are baked into the geometry.
    Vec3 xformPoint(const ShapeDesc &s, const vec &v) const {
        if (!s.hasXForm) return toVec(v);
        glm::vec4 p = s.localXForm * glm::vec4(v.x, v.y, v.z, 1.0f);
        return Vec3(p.x, p.y, p.z);
    }

    void addHittable(const ShapeDesc &s) {
        MatPtr mat = material(s.shaderNameReference);

        if (s.type == "sphere") {
            Vec3 center = xformPoint(s, s.center);
            double radius = s.radius;

            if (s.hasXForm) {
                // Only uniform scale can be baked into a sphere (rotation is a no-op).
                const glm::mat4 &M = s.localXForm;
                float sx = glm::length(glm::vec3(M[0]));
                float sy = glm::length(glm::vec3(M[1]));
                float sz = glm::length(glm::vec3(M[2]));
                if (std::abs(sx - sy) > 1e-4f * sx || std::abs(sx - sz) > 1e-4f * sx)
                    unsupported("non-uniform scale on sphere '" + s.name + "'");
                radius *= sx;
            }
            m_scene.hittable_back(Sphere3D<double>{center, radius, mat}.any());
        }
        else if (s.type == "triangle") {
            m_scene.hittable_back(Triangle3<double>{
                xformPoint(s, s.v0), xformPoint(s, s.v1), xformPoint(s, s.v2), mat}.any());
        }
        else {
            // "box" (CUBE not implemented), "mesh" (no mesh loader), "instance"
            unsupported("shape type '" + s.type + "' ('" + s.name + "')");
        }
    }
};

}