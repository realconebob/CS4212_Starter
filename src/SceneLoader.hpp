/// TODO: I should just rewrite this entire damn thing

#pragma once

#include "cwrender/Helpers.hpp"
#include "cwrender/BaseTypes/BaseTypes.hpp"
#include "cwrender/Hittables/Hittables.hpp"
#include "cwrender/Hittables/Plane.hpp"
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

class SceneLoader : public ISceneLoader {
public:
    using Vec3 = VecX<double, 3>;

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
        m_scene.add(PointLight<double, 3>{l.pos, l.color /* * 10e3 */});

    for (const auto &s : m_shapes)
        addHittable(s);

    m_scene.build(); // see Scene change below


    std::fprintf(stderr, "lights=%zu shapes=%zu shaders=%zu\n", m_scene.getlights().size(), m_shapes.size(), m_shaders.size());
    for (auto &[n, d] : m_shaders)
        std::fprintf(stderr, "shader %s type=%s diffuse=(%g %g %g) spec=(%g %g %g) phong=%g\n",
            n.c_str(), d.type.c_str(),
            d.diffuse.data.x, d.diffuse.data.y, d.diffuse.data.z,
            d.specular.data.x, d.specular.data.y, d.specular.data.z, d.phongExp);
    for (auto &l : m_pointLights)
        std::fprintf(stderr, "light pos=(%g %g %g) color=(%g %g %g)\n",
            l.pos[0], l.pos[1], l.pos[2], l.color[0], l.color[1], l.color[2]);
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

    static Vec3 toVec(const vec &v) { return Vec3(v.x, v.y, v.z); }

    void unsupported(const std::string &what) const {
        if (m_strict) throw NotImplemented(("SceneLoader: unsupported " + what).c_str());
        std::fprintf(stderr, "SceneLoader: unsupported %s (skipped/approximated)\n", what.c_str());
    }

    // ------------------------------------------------------------- materials
    const ShaderDesc &shader(const std::string &name) const {
        auto it = m_shaders.find(name);
        if (it == m_shaders.end())
            throw std::runtime_error("Shape references unknown shader '" + name + "'");
        return it->second;
    }

    void validateTextures(const ShaderDesc &d) const {
        // cwrender has no textured albedo yet; just validate the reference.
        for (const ShaderProperty *p : {&d.diffuse, &d.specular, &d.normal, &d.emission}) {
            if (!p->hasTexture) continue;
            if (!m_textures.count(p->textureName))
                throw std::runtime_error("Shader '" + d.name + "' references unknown texture '" + p->textureName + "'");
            unsupported("textured shader '" + d.name + "'");
        }
    }

    // Builds the concrete material for a shader and passes it BY VALUE to f.
    // f must be a generic lambda: Renderable needs the concrete type to own a copy.
    template <typename F>
    void withMaterial(const ShaderDesc &d, F &&f) const {
        validateTextures(d);
        const std::string &t = d.type;

        // TODO: confirm "Diffuse" should map to Lambertian rather than DiffuseShader
        if (t == "Lambertian" || t == "Diffuse") {
            f(LambertianShader<double, 3>{toVec(d.diffuse.data)});
        }
        else if (t == "BlinnPhong" || t == "Phong") {
            f(BlinnPhongShader<double, 3>{toVec(d.specular.data), double(d.phongExp), toVec(d.diffuse.data)});
        }
        else if (t == "Mirror" || t == "Metal") {
            // no albedo in the file: perfect white reflector, roughness -> fuzz
            f(MetalShader<double, 3>{Vec3::Ones(), double(d.roughness)});
        }
        else if (t == "Normal") {
            f(NormalMapShader<double, 3>{});
        }
        else {
            // Glaze, Dielectric, BlinnPhongMirrored, CoolToWarm, Emitter, unknown
            unsupported("shader type '" + t + "' (shader '" + d.name + "')");
            f(LambertianShader<double, 3>{Vec3(0.5, 0.5, 0.5)}); // non-strict fallback
        }
    }

    // Pairs a concrete shape with the concrete material for `sd` and adds it to the scene.
    template <typename H>
    void emit(H shape, const ShaderDesc &sd) {
        withMaterial(sd, [&](auto mat) {
            m_scene.add(Renderable<double, 3>{std::move(shape), std::move(mat)});
        });
    }

    // --------------------------------------------------------------- shapes
    // cwrender hittables have no transform of their own, so xforms are baked into the geometry.
    Vec3 xformPoint(const ShapeDesc &s, const vec &v) const {
        if (!s.hasXForm) return toVec(v);
        glm::vec4 p = s.localXForm * glm::vec4(v.x, v.y, v.z, 1.0f);
        return Vec3(p.x, p.y, p.z);
    }

    void addHittable(const ShapeDesc &s) {
        const ShaderDesc &sd = shader(s.shaderNameReference); // fail early on bad references

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
            emit(Sphere3<double>{center, radius}, sd);
        }
        else if (s.type == "triangle") {
            emit(Triangle3<double>{xformPoint(s, s.v0), xformPoint(s, s.v1), xformPoint(s, s.v2)}, sd);
        }
        else if (s.type == "plane") {
            emit(Plane3<double>{xformPoint(s, s.v0), xformPoint(s, s.v1)}, sd);
        }
        else {
            // "box" (CUBE not implemented), "mesh" (no mesh loader), "instance"
            unsupported("shape type '" + s.type + "' ('" + s.name + "')");
        }
    }
};

}