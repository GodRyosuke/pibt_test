#include "localization_map_actor.hpp"
#include "component/mesh_component.hpp"
#include "definitions.hpp"
#include "game.hpp"
#include "yaml-cpp/yaml.h"

namespace wander_csm_test {
LocalizationMapActor::LocalizationMapActor(Game& game)
    : Actor(game, "localization_map")
{
    MeshComponent& meshComponent   = game.createMeshComponent(getId(), std::string(ASSET_PATH) + "plane/plane.obj", "localizationMapShader");
    const auto&    localizationTex = game.loadTexture(m_localizationImgPath);
    const double   imgWidth        = localizationTex.getWidth();
    const double   imgHeight       = localizationTex.getHeight();
    const double   fieldWidth      = imgWidth * 0.05;
    const double   fieldHeight     = imgHeight * 0.05;

    const auto localizationYaml = YAML::LoadFile(std::string(ASSET_PATH) + "Magnes_202501-01F/localizations/localization.yaml");
    double     originX          = localizationYaml["origin"][0].as<double>();
    double     originY          = localizationYaml["origin"][1].as<double>();
    m_origin                    = wu::Vec2(originX, originY);

    // setPosition(wu::Vec3(fieldWidth / 2.0, fieldHeight / 2.0, 0.0));
    setPosition(wu::Vec3(fieldWidth / 2.0, fieldHeight / 2.0, 0.0) + wu::Vec3(originX, originY, -0.001));
    setRotation(wu::PI / 2.0);
    setScale(wu::Vec3(fieldHeight, fieldWidth, 1.));
    
    meshComponent.setTexture(localizationImgPath);
    addComponent(meshComponent);

    // setScale(2.0 / 5.0);
}
}  // namespace wander_csm_test
