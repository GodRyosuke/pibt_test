#include "test_mesh_actor.hpp"
#include "component/mesh_component.hpp"
#include "definitions.hpp"
#include "game.hpp"
#include "util/wander_math.hpp"

namespace wander_csm_test {
TestMeshActor::TestMeshActor(Game& game)
    : Actor(game, "test_mesh_actor"),
      m_angle(0.0)
{
    setPosition(wu::Vec3(0.0, 0.0, 1.0));

    MeshComponent& meshComponent = game.createMeshComponent(getId(), std::string(ASSET_PATH) + "TestCube/ConcreteBox.obj");
    addComponent(meshComponent);
}

void TestMeshActor::updateActor()
{
    m_angle += 0.01;
    while (m_angle > 2 * wu::PI) {
        m_angle -= 2 * wu::PI;
    }

    setRotation(m_angle);
}
}  // namespace wander_csm_test
