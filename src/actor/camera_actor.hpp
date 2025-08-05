#pragma once

#include "actor.hpp"

namespace wander_csm_test {
class CameraActor : public Actor
{
public:
    CameraActor(Game& game);
    void setTarget(const wu::Vec3& target)
    {
        m_target        = target;
        m_needRecompute = true;
    };
    void updateActor() override;
    void inputActor(const InputEvent& inputEvent) override;
    void move(const wu::Vec3& deltaX)
    {
        getPosition() += deltaX;
        m_target += deltaX;
        m_needRecompute = true;
    }
    const wu::Mat4& getViewMat() const { return m_viewMat; }
    const wu::Mat4& getProjMat() const { return m_projMat; }

private:
    /// @brief カメラの位置を更新する
    void updateTranslation(const InputEvent& inputEvent);
    /// @brief カメラの向きを更新する
    void updateRotation(const InputEvent& inputEvent);

    /// @brief ortho -> perspective
    void toPerspective();
    /// @brief perspective -> ortho
    void toOrthogonal();

    bool m_needRecompute;

    wu::Vec3 m_target;
    wu::Vec3 m_up;
    wu::Mat4 m_viewMat;
    wu::Mat4 m_projMat;
    double   m_fov;
    double   m_near;
    double   m_far;

    wu::Vec2 m_startTranslationPoint;
    wu::Vec3 m_lastTarget;
    wu::Vec3 m_lastPos;
    double   m_orthoRight;

    double m_horizontalAngle;
    double m_verticalAngle;
    bool   m_isOrthoView;
};
}  // namespace wander_csm_test
