#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include "gl.hpp"
#include "grid.hpp"
#include "input_event.hpp"
#include "util/wander_math.hpp"

namespace wander_csm_test {
class Actor;
class Mesh;
class Shader;
class TextureShadowMap;
class MeshComponent;
class Texture;

class Game
{
public:
    Game();
    ~Game();
    void init();
    void run();
    int  getScreenWidth() const { return m_screenWidth; }
    int  getScreenHeight() const { return m_screenHeight; }
    bool isChangedScreenSize() const { return m_isChangedScreenSize; }

    const Shader& getShader(const std::string& name);

    void addActor(std::unique_ptr<Actor>&& actor);
    template <class T>
    T& getActor(const std::string& id)
    {
        auto iter = m_actors.find(id);
        if (iter == m_actors.end()) {
            throw std::runtime_error("actor " + id + " is not found");
        }
        return static_cast<T&>(*iter->second);
    }
    const Mesh&    loadMesh(const std::string& meshPath);
    const Texture& loadTexture(const std::string& meshPath);

    double             getDeltaT() const { return m_deltaT; }
    const std::string& getCameraId() const { return m_cameraId; }
    MeshComponent&     createMeshComponent(const std::string& ownerId, const std::string& meshFilePath, const std::string& shaderName);
    MeshComponent&     getMeshComponent(const std::string& id);

private:
    void input();
    void update();
    void draw();

    void loadGameObjects();

    void updateDeltaT();
    void updateMousePos();

    int         m_screenWidth;
    int         m_screenHeight;
    bool        m_isChangedScreenSize;
    bool        m_isRunning;
    GLFWwindow* m_glfwWindow;
    std::string m_cameraId;
    wu::Mat4    m_lightOrtho;
    wu::Mat4    m_lightView;

    double m_deltaT;
    double m_fps;
    double m_lastTime;

    InputEvent m_inputEvent;

    std::unique_ptr<Grid>                                     m_grid;
    std::unordered_map<std::string, std::unique_ptr<Mesh>>    m_meshes;
    std::unordered_map<std::string, std::unique_ptr<Texture>> m_textures;
    std::unordered_map<std::string, std::unique_ptr<Shader>>  m_shaders;
    std::unique_ptr<TextureShadowMap>                         m_textureShadowMap;

    std::unordered_map<std::string, std::unique_ptr<MeshComponent>> m_meshComponents;

    std::unordered_map<std::string, std::unique_ptr<Actor>> m_actors;
};
}  // namespace wander_csm_test
