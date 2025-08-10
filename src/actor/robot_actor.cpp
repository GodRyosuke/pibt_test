#include "robot_actor.hpp"
#include "component/mesh_component.hpp"
#include "definitions.hpp"
#include "game.hpp"

namespace wander_csm_test {
RobotActor::RobotActor(Game& game, const std::string& name)
    : Actor(game, name)
{
    MeshComponent& meshComponent = game.createMeshComponent(getId(), std::string(ASSET_PATH) + "robot_agent/objects.obj", "testMeshShader");
    meshComponent.setColor(wu::Vec3(0.2, 0.2, 0.9));
    addComponent(meshComponent);

    setPosition(wu::Vec3(-5.0, 0.0, 0.0));
    setScale(0.5 / 2.0);
}
}  // namespace wander_csm_test
