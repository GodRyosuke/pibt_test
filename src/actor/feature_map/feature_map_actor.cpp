#include "feature_map_actor.hpp"
#include <fstream>
#include "component/instanced_mesh_component.hpp"
#include "definitions.hpp"
#include "game.hpp"
#include "nlohmann/json.hpp"

namespace wander_csm_test {
namespace nl = nlohmann;

FeatureMapActor::FeatureMapActor(Game& game)
    : Actor(game, "feature_map_actor")
{
    const auto toNodeModelTransform = [](const wu::Vec3& position) -> InstancedMeshComponent::ShaderMat4 {
        const auto scale          = wu::Mat4::scale(wu::Vec3(0.8 / 2.0, 0.8 / 2.0, 0.8 / 2.0));
        const auto translation    = wu::Mat4::translation(position);
        wu::Mat4   modelTransform = translation * scale;
        return {
            modelTransform(0, 0), modelTransform(1, 0), modelTransform(2, 0), modelTransform(3, 0),
            modelTransform(0, 1), modelTransform(1, 1), modelTransform(2, 1), modelTransform(3, 1),
            modelTransform(0, 2), modelTransform(1, 2), modelTransform(2, 2), modelTransform(3, 2),
            modelTransform(0, 3), modelTransform(1, 3), modelTransform(2, 3), modelTransform(3, 3)};
    };

    InstancedMeshComponent& meshComponent = game.createInstancedMeshComponent(getId(), std::string(ASSET_PATH) + "circle/circle.obj", "instancedMeshShader");
    std::ifstream           f(std::string(ASSET_PATH) + "map_data/Magnes_202501-01F/features/feature_map.json");
    nl::json                featureMapJson = nl::json::parse(f);
    for (const auto& node : featureMapJson["nodes"]) {
        const std::string nodeId  = node["id"];
        const auto&       posJson = node["pose"]["position"];
        const double      posX    = posJson.contains("x") ? posJson["x"].get<double>() : 0.0;
        const double      posY    = posJson.contains("y") ? posJson["y"].get<double>() : 0.0;
        // const double size = node.contains("range") ? node["range"].get<double>() : 1.0;

        InstancedMeshComponent::InstancedMeshCompData data = {
            toNodeModelTransform(wu::Vec3(posX, posY, 0.0)),
            wu::Vec4(0.2, 0.9, 0.2, 0.9)};
        meshComponent.addData(data);
    }
    addComponent(meshComponent);
}

}  // namespace wander_csm_test
