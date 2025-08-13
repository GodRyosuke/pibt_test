#include "robot_actor.hpp"
#include <iostream>
#include "component/instanced_mesh_component.hpp"
#include "component/mesh_component.hpp"
#include "definitions.hpp"
#include "game.hpp"
#include "localization_map_actor.hpp"
#include "rrt.hpp"

namespace wander_csm_test {
RobotActor::RobotActor(Game& game, const std::string& name, const std::string& localizationMapId)
    : Actor(game, name),
      m_localizatinMapActorId(localizationMapId),
      m_velocity(0.5),
      m_nextGoalIdx(-1),
      m_angle(wu::PI / 2.0)
{
    MeshComponent& meshComponent = game.createMeshComponent(getId(), std::string(ASSET_PATH) + "robot_agent/robot_agent.obj", "testMeshShader");
    meshComponent.setColor(wu::Vec3(0.2, 0.2, 0.9));
    addComponent(meshComponent);

    setPosition(wu::Vec3(0, -2, 0));
    setScale(0.5 / 2.0);

    InstancedMeshComponent& instancedMeshComponent = game.createInstancedMeshComponent(getId(), std::string(ASSET_PATH) + "circle/circle.obj", "instancedMeshShader");
    m_instanceMeshCompId                           = instancedMeshComponent.getId();
    addComponent(instancedMeshComponent);
}

void RobotActor::updateActor()
{
    if (m_nextGoalIdx < 0) {
        return;
    }
    auto&        currentPos = getPosition();
    const double deltaT     = m_game.getDeltaT();
    double nextGoalDist = (m_globalPath[m_nextGoalIdx] - currentPos).norm();
    while (true) {
        if ((nextGoalDist > 0.3) && (m_nextGoalIdx < (m_globalPath.size() - 1))) {
            break;
        }
        if ((nextGoalDist < 0.1) && (m_nextGoalIdx == (m_globalPath.size() - 1))) {
            // 目的地へ到着
            m_nextGoalIdx = -1;
            std::cout << "Robot \"" << getName() << "\" has reached the destination: " << currentPos.x() << ", " << currentPos.y() << std::endl;
            return;
        }
        if (m_nextGoalIdx == (m_globalPath.size() - 1)) {
            break;
        }
        m_nextGoalIdx++;
        nextGoalDist = (m_globalPath[m_nextGoalIdx] - currentPos).norm();
    }

    // pure pursuit法による制御
    const auto& nextGoal      = m_globalPath[m_nextGoalIdx];
    double      nextGoalAngle = std::atan2(nextGoal.y() - currentPos.y(), nextGoal.x() - currentPos.x());
    if (nextGoalAngle < 0) {
        nextGoalAngle += 2 * wu::PI;
    }

    double angleAlpha      = nextGoalAngle - m_angle;
    double robotVelocity   = std::min((currentPos - m_globalPath[m_globalPath.size() - 1]).norm() * 0.3, m_velocity);
    double angularVelocity = 2 * m_velocity * std::sin(angleAlpha) / nextGoalDist;

    m_angle += angularVelocity * deltaT;
    currentPos += wu::Vec3(std::cos(m_angle), std::sin(m_angle), 0) * robotVelocity * deltaT;
    setRotation(m_angle - wu::PI / 2.0);
}

void RobotActor::planGlobalPath(const wu::Vec2& goal)
{
    // RRTで経路計画
    auto&       rrt        = m_game.getRRT();
    const auto& currentPos = getPosition();

    auto globalPathPixelSpace = rrt.plan(wu::Vec2(currentPos.x(), currentPos.y()), goal);

    const auto& localizationMap = m_game.getActor<LocalizationMapActor>(m_localizatinMapActorId);

    m_globalPath.clear();
    m_globalPath.reserve(globalPathPixelSpace.size());
    for (int i = globalPathPixelSpace.size() - 1; i >= 0; i--) {
        wu::Vec2 posWorldSpace = localizationMap.toWorldSpace(globalPathPixelSpace[i]);
        m_globalPath.emplace_back(wu::Vec3(posWorldSpace.x(), posWorldSpace.y(), 0));
    }

    m_nextGoalIdx = 0;

    const auto toNodeModelTransform = [](const wu::Vec3& position) -> InstancedMeshComponent::ShaderMat4 {
        const auto scale          = wu::Mat4::scale(wu::Vec3(0.3 / 2.0, 0.3 / 2.0, 0.3 / 2.0));
        const auto translation    = wu::Mat4::translation(position);
        wu::Mat4   modelTransform = translation * scale;
        return {
            modelTransform(0, 0), modelTransform(1, 0), modelTransform(2, 0), modelTransform(3, 0),
            modelTransform(0, 1), modelTransform(1, 1), modelTransform(2, 1), modelTransform(3, 1),
            modelTransform(0, 2), modelTransform(1, 2), modelTransform(2, 2), modelTransform(3, 2),
            modelTransform(0, 3), modelTransform(1, 3), modelTransform(2, 3), modelTransform(3, 3)};
    };

    InstancedMeshComponent& instancedMeshComponent = m_game.getMeshComponent<InstancedMeshComponent>(m_instanceMeshCompId);
    instancedMeshComponent.clearData();
    for (const auto& path : m_globalPath) {
        InstancedMeshComponent::InstancedMeshCompData data = {
            toNodeModelTransform(wu::Vec3(path.x(), path.y(), 0.002)),
            wu::Vec4(m_color.x(), m_color.y(), m_color.z(), 0.7)};

        instancedMeshComponent.addData(data);
    }
}
}  // namespace wander_csm_test
