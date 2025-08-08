#include "localization_map_actor.hpp"
#include "component/mesh_component.hpp"
#include "definitions.hpp"
#include "game.hpp"

namespace wander_csm_test {
LocalizationMapActor::LocalizationMapActor(Game& game)
    : Actor(game, "localization_map")
{
    MeshComponent& meshComponent = game.createMeshComponent(getId(), std::string(ASSET_PATH) + "plane/plane.obj", "localizationMapShader");
    const std::string localizationImgPath =std::string(ASSET_PATH) + "localization.png";
    const auto& localizationTex = game.loadTexture(localizationImgPath);
    const double imgWidth = localizationTex.getWidth();
    const double imgHeight = localizationTex.getHeight();
    const double fieldWidth = imgWidth * 0.05;
    const double fieldHeight = imgHeight * 0.05;
    setScale(wu::Vec3(fieldHeight, fieldWidth, 1.));
    
    meshComponent.setTexture(localizationImgPath);
    addComponent(meshComponent);

    // setScale(2.0 / 5.0);
}
}  // namespace wander_csm_test
