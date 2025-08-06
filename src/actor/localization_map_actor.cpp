#include "localization_map_actor.hpp"
#include "component/mesh_component.hpp"
#include "definitions.hpp"
#include "game.hpp"

namespace wander_csm_test {
LocalizationMapActor::LocalizationMapActor(Game& game)
    : Actor(game, "localization_map")
{
    MeshComponent& meshComponent = game.createMeshComponent(getId(), std::string(ASSET_PATH) + "plane/plane.fbx", "localizationMapShader");
    meshComponent.setTexture(std::string(ASSET_PATH) + "sample_map.png");
    addComponent(meshComponent);

    setScale(2.0 / 5.0);
}
}  // namespace wander_csm_test
