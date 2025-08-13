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
      m_nextGoalIdx(-1),
      m_angle(wu::PI / 2.0)
{
    MeshComponent& meshComponent = game.createMeshComponent(getId(), std::string(ASSET_PATH) + "robot_agent/robot_agent.obj", "testMeshShader");
    meshComponent.setColor(wu::Vec3(0.2, 0.2, 0.9));
    addComponent(meshComponent);

    setPosition(wu::Vec3(0, -2, 0));
    setScale(0.5 / 2.0);

    planGlobalPath(wu::Vec2(8, 3.5));

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

    InstancedMeshComponent& instancedMeshComponent = game.createInstancedMeshComponent(getId(), std::string(ASSET_PATH) + "circle/circle.obj", "instancedMeshShader");
    for (const auto& path : m_globalPath) {
        InstancedMeshComponent::InstancedMeshCompData data = {
            toNodeModelTransform(wu::Vec3(path.x(), path.y(), 0.002)),
            wu::Vec4(0.5, 0.5, 0.9, 0.5)};

        instancedMeshComponent.addData(data);
    }
    addComponent(instancedMeshComponent);
}

void RobotActor::planGlobalPath(const wu::Vec2& goal)
{
    // RRTで経路計画
    auto&       rrt        = m_game.getRRT();
    const auto& currentPos = getPosition();

    auto globalPathPixelSpace = rrt.plan(wu::Vec2(currentPos.x(), currentPos.y()), goal);

    const auto& localizationMap = m_game.getActor<LocalizationMapActor>(m_localizatinMapActorId);

    m_globalPath.reserve(globalPathPixelSpace.size());
    for (int i = globalPathPixelSpace.size() - 1; i >= 0; i--) {
        wu::Vec2 posWorldSpace = localizationMap.toWorldSpace(globalPathPixelSpace[i]);
        m_globalPath.emplace_back(wu::Vec3(posWorldSpace.x(), posWorldSpace.y(), 0));
    }

    m_nextGoalIdx = 0;
}
}  // namespace wander_csm_test
