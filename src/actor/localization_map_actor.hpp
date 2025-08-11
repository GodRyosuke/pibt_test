#pragma once

#include "actor.hpp"
#include "texture.hpp"

namespace wander_csm_test {
class Texture;
class LocalizationMapActor : public Actor
{
public:
    LocalizationMapActor(Game& game);
    bool isAvailableSpace(const wu::Vec2& pos) const;

private:
    std::string   m_localizationImgPath;
    unsigned char m_pathDetectionTh;
    wu::Vec2      m_origin;
};
}  // namespace wander_csm_test
