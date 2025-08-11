#include "localization_map_actor.hpp"
#include "component/mesh_component.hpp"
#include "definitions.hpp"
#include "game.hpp"
#include "yaml-cpp/yaml.h"

namespace wander_csm_test {
LocalizationMapActor::LocalizationMapActor(Game& game)
    : Actor(game, "localization_map"),
      m_localizationImgPath(std::string(ASSET_PATH) + "map_data/Magnes_202501-01F/localizations/localization.png"),
      m_pathDetectionTh(250)
{
    MeshComponent& meshComponent   = game.createMeshComponent(getId(), std::string(ASSET_PATH) + "plane/plane.obj", "localizationMapShader");
    const auto&    localizationTex = game.loadTexture(m_localizationImgPath);
    const double   imgWidth        = localizationTex.getWidth();
    const double   imgHeight       = localizationTex.getHeight();
    const double   fieldWidth      = imgWidth * 0.05;
    const double   fieldHeight     = imgHeight * 0.05;

    const auto localizationYaml = YAML::LoadFile(std::string(ASSET_PATH) + "map_data/Magnes_202501-01F/localizations/localization.yaml");
    double     originX          = localizationYaml["origin"][0].as<double>();
    double     originY          = localizationYaml["origin"][1].as<double>();
    m_origin                    = wu::Vec2(originX, originY);

    // setPosition(wu::Vec3(fieldWidth / 2.0, fieldHeight / 2.0, 0.0));
    setPosition(wu::Vec3(fieldWidth / 2.0, fieldHeight / 2.0, 0.0) + wu::Vec3(originX, originY, -0.001));
    setRotation(wu::PI / 2.0);
    setScale(wu::Vec3(fieldHeight, fieldWidth, 1.));

    meshComponent.setTexture(m_localizationImgPath);
    addComponent(meshComponent);
}

bool LocalizationMapActor::isAvailableSpace(const wu::Vec2& pos) const
{
    // 左上原点 y軸下向きの座標系に変換
    const auto&  localizationTex = m_game.loadTexture(m_localizationImgPath);
    const double imgWidth        = localizationTex.getWidth();
    const double imgHeight       = localizationTex.getHeight();
    const double fieldWidth      = imgWidth * 0.05;
    const double fieldHeight     = imgHeight * 0.05;

    wu::Vec2 imgCenterCoord = pos - wu::Vec2(fieldWidth / 2.0, fieldHeight / 2.0) - m_origin;
    // 左下が原点でy軸上にflipしてloadされているから、この読み方で行う
    wu::Vec2 cvCoord = imgCenterCoord + wu::Vec2(fieldWidth / 2.0, fieldHeight / 2.0);
    // wu::Vec2 cvCoord = imgCenterCoord - wu::Vec2(-fieldWidth / 2.0, fieldHeight / 2.0);
    // cvCoord.y() *= -1;

    if ((cvCoord.x() < 0) || (cvCoord.x() >= fieldWidth) || (cvCoord.y() < 0) || (cvCoord.y() >= fieldHeight)) {
        return false;
    }

    const wu::Vec2 imgPixel = cvCoord * 20;
    auto           pixel    = localizationTex.getPixel(imgPixel.x(), imgPixel.y());
    if (pixel < m_pathDetectionTh) {
        return false;
    }
    return true;
}
}  // namespace wander_csm_test
