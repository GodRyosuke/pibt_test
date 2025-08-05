#pragma once

#include <unordered_map>
#include "gl.hpp"
#include "util/wander_math.hpp"

namespace wander_csm_test {
class Game;

class InputEvent
{
public:
    InputEvent(const Game& game);
    void init(GLFWwindow* window);
    void input();

    enum ButtonState
    {
        PRESS,     ///< つい先ほどMouseMiddleButtonが押された
        PRESSED,   ///< MouseMiddleButtonが押されっぱなし
        RELEASE,   ///< つい先ほどMouseMiddleButtonが解放された
        RELEASED,  ///< ずっとMouseMiddleButtonは押されていない
        NUM_MOUSE_MIDDLE_BUTTON_STATES,
    };
    ButtonState getButtonState(int mouseButtonType) const;

    bool            isChangedMousePos() const { return m_isChangedMousePos; }
    void            setMousePosCenter() const;
    const wu::Vec2& getMousePos() const { return m_mousePos; }
    void            changeCursorVisibility(int visibilityType) const;
    const wu::Vec2& getScrollOffset() const { return m_scrollOffset; }

private:
    void updateMousePos();
    void updateButtonStates();

    GLFWwindow* m_glfwWindow;

    std::unordered_map<int, ButtonState> m_buttonStates;
    std::unordered_map<int, ButtonState> m_lastButtonStates;

    wu::Vec2 m_mousePos;
    wu::Vec2 m_scrollOffset;
    bool     m_isChangedMousePos;

    const Game& m_game;
};
}  // namespace wander_csm_test
