#include "camera_actor.hpp"
#include <cmath>
#include "game.hpp"

namespace wander_csm_test {
CameraActor::CameraActor(Game& game)
    : Actor(game, "camera"),
      m_target({1.0, 0.0, 0.0}),
      m_up({0.0, 0.0, 1.0}),
      m_fov(45.0 * wu::PI / 180),
      m_near(0.1),
      m_far(100.0),
      m_horizontalAngle(0.),
      m_verticalAngle(0.),
      m_needRecompute(true),
      m_isOrthoView(false),
      m_orthoRight(10.)
{
    setPosition({0.0, -5.0, 10.0});
    setTarget(getPosition() + wu::Vec3({1.0, 0.0, 0.0}));

    double screenWidth  = game.getScreenWidth();
    double screenHeight = game.getScreenHeight();
    double aspect       = screenHeight / screenWidth;
    m_projMat           = std::move(wu::Mat4::perspective(m_fov, aspect, m_near, m_far));
}

void CameraActor::updateActor()
{
    const double screenWidth  = static_cast<double>(m_game.getScreenWidth());
    const double screenHeight = static_cast<double>(m_game.getScreenHeight());

    if (m_game.isChangedScreenSize()) {
        double aspect = screenHeight / screenWidth;
        if (m_isOrthoView) {
            m_projMat = std::move(wu::Mat4::orthogonal(m_orthoRight, -m_orthoRight, m_orthoRight * aspect, -m_orthoRight * aspect, m_near, m_far));
        } else {
            m_projMat = std::move(wu::Mat4::perspective(m_fov, aspect, m_near, m_far));
        }
    }

    if (m_needRecompute) {
        m_viewMat       = std::move(wu::Mat4::view(getPosition(), m_target, m_up));
        m_needRecompute = false;
    }
}

void CameraActor::inputActor(const InputEvent& InputEvent)
{
    updateTranslation(InputEvent);
    updateRotation(InputEvent);

    if (InputEvent.getButtonState('Z') == InputEvent::PRESS) {
        if (m_isOrthoView) {
            toPerspective();
            m_isOrthoView = false;
        } else {
            toOrthogonal();
            m_isOrthoView = true;
        }
    }
}

void CameraActor::updateTranslation(const InputEvent& inputEvent)
{
    auto&    pos     = getPosition();
    wu::Vec3 forward = m_target - pos;
    forward.normalize();
    wu::Vec3 right = forward.cross(m_up);
    right.normalize();
    wu::Vec3 up = right.cross(forward);

    if (inputEvent.getButtonState(GLFW_MOUSE_BUTTON_LEFT) == InputEvent::PRESS) {
        m_startTranslationPoint = inputEvent.getMousePos();
        m_lastPos               = pos;
        m_lastTarget            = m_target;
    }
    if (inputEvent.getButtonState(GLFW_MOUSE_BUTTON_LEFT) == InputEvent::PRESSED) {
        wu::Vec2 mouseDeltaX = inputEvent.getMousePos() - m_startTranslationPoint;
        wu::Vec3 deltaX      = -right * 0.1 * mouseDeltaX.x();
        wu::Vec3 deltaY      = up * 0.1 * mouseDeltaX.y();

        pos             = m_lastPos + deltaX + deltaY;
        m_target        = m_lastTarget + deltaX + deltaY;
        m_needRecompute = true;
    }

    double scrollYOffset = inputEvent.getScrollOffset().y();
    if (std::abs(scrollYOffset) > 1e-5) {
        pos += forward * 0.1 * scrollYOffset;
        m_target += forward * 0.1 * scrollYOffset;
        m_orthoRight    = std::max(0.1, m_orthoRight - 0.1 * scrollYOffset);
        m_needRecompute = true;
    }
}

void CameraActor::updateRotation(const InputEvent& inputEvent)
{
    const double screenWidth  = static_cast<double>(m_game.getScreenWidth());
    const double screenHeight = static_cast<double>(m_game.getScreenHeight());

    if (inputEvent.getButtonState(GLFW_MOUSE_BUTTON_MIDDLE) == InputEvent::PRESS) {
        if (m_isOrthoView) {
            toPerspective();
            m_isOrthoView = false;
        }
        inputEvent.setMousePosCenter();
    }
    if (inputEvent.isChangedMousePos() && (inputEvent.getButtonState(GLFW_MOUSE_BUTTON_MIDDLE) == InputEvent::PRESSED)) {
        inputEvent.changeCursorVisibility(GLFW_CURSOR_HIDDEN);
        m_horizontalAngle += 0.3 * m_game.getDeltaT() * ((screenWidth / 2.) - inputEvent.getMousePos().x());
        m_verticalAngle += 0.3 * m_game.getDeltaT() * ((screenHeight / 2.) - inputEvent.getMousePos().y());
        m_target = getPosition() + wu::Vec3(
                                       std::cos(m_verticalAngle) * std::cos(m_horizontalAngle),
                                       std::cos(m_verticalAngle) * std::sin(m_horizontalAngle),
                                       std::sin(m_verticalAngle));
        m_needRecompute = true;
        inputEvent.setMousePosCenter();
    }
    if (inputEvent.getButtonState(GLFW_MOUSE_BUTTON_MIDDLE) == InputEvent::RELEASED) {
        inputEvent.changeCursorVisibility(GLFW_CURSOR_NORMAL);
    }
}

void CameraActor::toPerspective()
{
    double aspect   = static_cast<double>(m_game.getScreenHeight()) / static_cast<double>(m_game.getScreenWidth());
    m_projMat       = std::move(wu::Mat4::perspective(m_fov, aspect, m_near, m_far));
    m_up            = wu::Vec3(0, 0, 1);
    m_needRecompute = true;
}

void CameraActor::toOrthogonal()
{
    double aspect   = static_cast<double>(m_game.getScreenHeight()) / static_cast<double>(m_game.getScreenWidth());
    double top      = m_orthoRight * aspect;
    m_projMat       = std::move(wu::Mat4::orthogonal(m_orthoRight, -m_orthoRight, top, -top, m_near, m_far));
    m_up            = wu::Vec3(0, 1, 0);
    m_target        = getPosition() + wu::Vec3(0, 0, -1);
    m_needRecompute = true;
}
}  // namespace wander_csm_test
