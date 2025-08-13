#include "game.hpp"
#include <iostream>
#include <stdexcept>
#include "actor/camera_actor.hpp"
#include "actor/feature_map/feature_map_actor.hpp"
#include "actor/localization_map_actor.hpp"
#include "actor/robot_actor.hpp"
#include "component/instanced_mesh_component.hpp"
#include "rrt.hpp"
#define _USE_MATH_DEFINES  // for C++
#include <cassert>
#include <chrono>
#include <thread>
#include "component/mesh_component.hpp"
#include "definitions.hpp"
#include "math.h"
#include "mesh.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "texture_shadow_map.hpp"
#include "util/wander_math.hpp"
#include "vertex_array.hpp"

namespace wander_csm_test {
Game::Game()
    : m_isRunning(true),
      m_glfwWindow(nullptr),
      m_screenWidth(800),
      m_screenHeight(500),
      m_isChangedScreenSize(true),
      m_textureShadowMap(nullptr),
      m_fps(64),
      m_lastTime(0),
      m_deltaT(0),
      m_inputEvent(*this)
{
}

Game::~Game()
{
    if (m_glfwWindow) {
        glfwDestroyWindow(m_glfwWindow);
    }
    glfwTerminate();  // Terminate GLFW
}

void Game::init()
{
    if (!glfwInit()) {
        // Initialization failed
        throw std::runtime_error("failed to initialize glfw");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    m_glfwWindow = glfwCreateWindow(m_screenWidth, m_screenHeight, "wander robot viewer", NULL, NULL);
    if (!m_glfwWindow) {
        throw std::runtime_error("failed to crate glfw window");
    }
    glfwMakeContextCurrent(m_glfwWindow);

    // initialize renderer
    gladLoadGL();

    // load shaders
    auto compileShader = [this](const std::string& name, const std::string& vertFilePath, const std::string& fragFilePath) -> void {
        std::unique_ptr<Shader> shader(new Shader());
        if (!shader->createShaderProgram(vertFilePath, fragFilePath)) {
            throw std::runtime_error("failed to compile shader: " + vertFilePath + "and " + fragFilePath);
        }
        m_shaders.emplace(name, std::move(shader));
    };
    compileShader("testMeshShader", std::string(SHADER_PATH) + "test_mesh.vert", std::string(SHADER_PATH) + "test_mesh.frag");
    compileShader("spriteShader", std::string(SHADER_PATH) + "test_mesh.vert", std::string(SHADER_PATH) + "sprite.frag");
    compileShader("localizationMapShader", std::string(SHADER_PATH) + "test_mesh.vert", std::string(SHADER_PATH) + "localization_map.frag");
    compileShader("gridShader", std::string(SHADER_PATH) + "grid.vert", std::string(SHADER_PATH) + "grid.frag");
    compileShader("instancedMeshShader", std::string(SHADER_PATH) + "instanced_mesh.vert", std::string(SHADER_PATH) + "instanced_mesh.frag");

    m_textureShadowMap = std::move(std::make_unique<TextureShadowMap>(1024, 1024));

    loadGameObjects();

    m_inputEvent.init(m_glfwWindow);

    // initialize light view and ortho projection matrix for shadow
    m_lightView  = std::move(wu::Mat4::view(wu::Vec3(100, 100, 100), wu::Vec3(), wu::Vec3(0, 0, 1)));
    m_lightOrtho = std::move(wu::Mat4::orthogonal(50, -50, 50, -50, 0.1, 200));
}

void Game::loadGameObjects()
{
    addActor(std::move(std::make_unique<FeatureMapActor>(*this)));
    auto              localizationMapActor = std::make_unique<LocalizationMapActor>(*this);
    const std::string localizatoinActorId  = localizationMapActor->getId();
    addActor(std::move(localizationMapActor));
    auto camera = std::make_unique<CameraActor>(*this);
    m_cameraId  = camera->getId();
    addActor(std::move(camera));

    m_grid = std::move(std::make_unique<Grid>());
    m_rrt  = std::move(std::make_unique<RRT>(*this, localizatoinActorId));

    std::unique_ptr<RobotActor> robotActor;
    robotActor = std::make_unique<RobotActor>(*this, "no1", localizatoinActorId, wu::Vec2(0, -2), wu::Vec3(65. / 255., 105. / 255., 225. / 255.));
    robotActor->setGoal(wu::Vec2(8, 3.5));
    robotActor->setGoal(wu::Vec2(8, 7));
    addActor(std::move(robotActor));

    robotActor = std::make_unique<RobotActor>(*this, "no2", localizatoinActorId, wu::Vec2(-9.5, 0), wu::Vec3(102. / 255., 205. / 255., 170. / 255.));
    robotActor->setGoal(wu::Vec2(0, 3.5));
    addActor(std::move(robotActor));
}

void Game::updateDeltaT()
{
    double currentTime = glfwGetTime();
    double diffTime    = currentTime - m_lastTime;

    if (diffTime <= 1.0 / m_fps) {
        int diffNanoseconds = ((1.0 / m_fps) - diffTime) * 1e9;
        std::this_thread::sleep_for(std::chrono::nanoseconds(diffNanoseconds));
    }
    double newTime = glfwGetTime();
    m_deltaT       = newTime - m_lastTime;
    m_lastTime     = newTime;
}

const Shader& Game::getShader(const std::string& name)
{
    auto iter = m_shaders.find(name);
    if (iter != m_shaders.end()) {
        return *iter->second;
        // } else if (name.length() == 0) {
        //     return m_shaders["TestMeshShader"];
    } else {
        std::cout << "error: Shader " << name << " has not been loaded yet" << std::endl;
        ;
        throw std::runtime_error("Shader has not been loaded yet");
    }
}

void Game::addActor(std::unique_ptr<Actor>&& actor)
{
    const std::string& actorId = actor->getId();
    auto               iter    = m_actors.find(actorId);
    if (iter != m_actors.end()) {
        throw std::runtime_error("actor " + actor->getName() + "(" + actorId + ")" + " is already added");
    }
    m_actors.emplace(actorId, std::move(actor));
}

const Mesh& Game::loadMesh(const std::string& meshPath)
{
    auto iter = m_meshes.find(meshPath);
    if (iter == m_meshes.end()) {
        auto        meshPtr = std::make_unique<Mesh>(meshPath);
        const auto& meshRef = *meshPtr;
        m_meshes.emplace(meshPath, std::move(meshPtr));
        return meshRef;
    }
    return *iter->second;
}

const Texture& Game::loadTexture(const std::string& path)
{
    auto iter = m_textures.find(path);
    if (iter == m_textures.end()) {
        auto        ptr = std::make_unique<Texture>(path);
        const auto& ref = *ptr;
        m_textures.emplace(path, std::move(ptr));
        return ref;
    }
    return *iter->second;
}

void Game::run()
{
    while (m_isRunning) {
        input();
        update();
        draw();
    }
}

void Game::input()
{
    m_inputEvent.input();

    for (auto& actor : m_actors) {
        actor.second->input(m_inputEvent);
    }
}

void Game::update()
{
    updateDeltaT();

    if ((glfwWindowShouldClose(m_glfwWindow)) ||
        (glfwGetKey(m_glfwWindow, 'Q') == GLFW_PRESS) ||
        (glfwGetKey(m_glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)) {
        m_isRunning = false;
    }

    int screenWidth, screenHeight;
    glfwGetWindowSize(m_glfwWindow, &screenWidth, &screenHeight);
    m_isChangedScreenSize = (m_screenWidth == screenWidth) && (m_screenHeight == screenHeight);
    m_screenWidth         = screenWidth;
    m_screenHeight        = screenHeight;

    for (auto& actor : m_actors) {
        actor.second->update();
    }
}

void Game::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Frame Buffer object
    // glDisable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);

    // glEnable(GL_DEPTH_TEST);
    // m_textureShadowMap->writeBind();
    // glClear(GL_DEPTH_BUFFER_BIT);  // これも必要

    // {
    //     // draw skinMeshComp
    // }

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);  // unbind
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // glCullFace(GL_BACK);
    // glDisable(GL_CULL_FACE);

    glClearColor(253.f / 255.f, 246.f / 255.f, 257.f / 255.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, m_screenWidth, m_screenHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // m_textureShadowMap->bind(GL_TEXTURE10);

    // draw mesh
    // render testMesh
    for (const auto& meshComp : m_meshComponents) {
        meshComp.second->draw();
    }

    // render grid
    {
        const auto& shader = m_shaders["gridShader"];
        shader->useProgram();

        // set test cube model transforms
        shader->setMatrix4Uniform("modelTransform", wu::Mat4::identity());

        // set camera parameters
        const auto& cameraActor = getActor<CameraActor>(m_cameraId);
        shader->setMatrix4Uniform("cameraViewProj", cameraActor.getProjMat() * cameraActor.getViewMat());

        m_grid->getVAO().setActive();
        glDrawElements(GL_LINES, m_grid->getNumIndexSize(), GL_UNSIGNED_INT, 0);
    }

    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    // glDisable(GL_CULL_FACE);
    // mSpriteShader->UseProgram();
    // for (const auto& sprite : m_spriteComponents) {
    //     sprite.second->draw();
    // }

    glfwSwapBuffers(m_glfwWindow);
}

MeshComponent& Game::createMeshComponent(const std::string& ownerId, const std::string& meshFilePath, const std::string& shaderName)
{
    const auto&                    mesh    = loadMesh(meshFilePath);
    std::unique_ptr<MeshComponent> compPtr = std::make_unique<MeshComponent>(*this, ownerId, mesh, getShader(shaderName));

    const std::string spriteId = compPtr->getId();
    auto&             compRef  = *compPtr;
    m_meshComponents.emplace(spriteId, std::move(compPtr));
    return compRef;
}

InstancedMeshComponent& Game::createInstancedMeshComponent(const std::string& ownerId, const std::string& meshFilePath, const std::string& shaderName)
{
    const auto&                             mesh    = loadMesh(meshFilePath);
    std::unique_ptr<InstancedMeshComponent> compPtr = std::make_unique<InstancedMeshComponent>(*this, ownerId, mesh, getShader(shaderName));

    const std::string spriteId = compPtr->getId();
    auto&             compRef  = *compPtr;
    m_meshComponents.emplace(spriteId, std::move(compPtr));
    return compRef;
}
}  // namespace wander_csm_test
