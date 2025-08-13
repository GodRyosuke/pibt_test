#pragma once

#include "actor.hpp"
#include "texture.hpp"

namespace wander_csm_test {
class Texture;
class LocalizationMapActor : public Actor
{
public:
    LocalizationMapActor(Game& game);
    bool                  isAvailableWorldSpace(const wu::Vec2& pos) const;
    bool                  isAvailablePixelSpace(const wu::Vec2& pos) const;
    std::vector<wu::Vec2> getPassPixels() const;
    wu::Vec2              toPixelSpace(const wu::Vec2& worldSpacePos) const;
    wu::Vec2              toWorldSpace(const wu::Vec2& pixelSpacePos) const;

private:
    std::string   m_localizationImgPath;
    unsigned char m_pathDetectionTh;
    wu::Vec2      m_origin;
};
}  // namespace wander_csm_test
