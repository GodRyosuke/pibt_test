#include "input_event.hpp"
#include <cassert>
#include "game.hpp"

namespace wander_csm_test {
InputEvent::InputEvent(const Game& game)
    : m_glfwWindow(nullptr),
      m_game(game)
{
}

void InputEvent::init(GLFWwindow* window)
{
    m_glfwWindow = window;
    glfwSetWindowUserPointer(m_glfwWindow, &m_scrollOffset);
    glfwSetScrollCallback(m_glfwWindow, [](GLFWwindow* window, double xoffset, double yoffset) {
        wu::Vec2* scrollOffset = static_cast<wu::Vec2*>(glfwGetWindowUserPointer(window));
        scrollOffset->x()      = xoffset;
        scrollOffset->y()      = yoffset;
    });
    glfwPollEvents();

    m_buttonStates[GLFW_MOUSE_BUTTON_LEFT]   = InputEvent::RELEASE;
    m_buttonStates[GLFW_MOUSE_BUTTON_RIGHT]  = InputEvent::RELEASE;
    m_buttonStates[GLFW_MOUSE_BUTTON_MIDDLE] = InputEvent::RELEASE;
    for (int code = static_cast<int>('A'); code <= static_cast<int>('Z'); ++code) {
        m_buttonStates[code] = InputEvent::RELEASE;
    }
    updateMousePos();
}

void InputEvent::input()
{
    m_scrollOffset = wu::Vec2();
    glfwPollEvents();

    updateMousePos();
    updateButtonStates();
}

void InputEvent::updateMousePos()
{
    double mouseX, mouseY;
    glfwGetCursorPos(m_glfwWindow, &mouseX, &mouseY);
    m_isChangedMousePos = false;
    if ((std::abs(m_mousePos.x() - mouseX) > 0.000000001) || (std::abs(m_mousePos.y() - mouseY) > 0.000000001)) {
        m_isChangedMousePos = true;
    }
    m_mousePos.x() = mouseX;
    m_mousePos.y() = mouseY;
}

void InputEvent::updateButtonStates()
{
    for (const auto mouseButtonType : {GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE}) {
        auto& currentButtonState = m_buttonStates[mouseButtonType];
        auto& lastButtonState    = m_lastButtonStates[mouseButtonType];

        if (glfwGetMouseButton(m_glfwWindow, mouseButtonType) == GLFW_PRESS) {
            if (lastButtonState == RELEASE) {
                currentButtonState = PRESS;
            } else {
                currentButtonState = PRESSED;
            }
            lastButtonState = PRESS;
        } else if (glfwGetMouseButton(m_glfwWindow, mouseButtonType) == GLFW_RELEASE) {
            if (lastButtonState == PRESS) {
                currentButtonState = RELEASE;
            } else {
                currentButtonState = RELEASED;
            }
            lastButtonState = RELEASE;
        }
    }

    for (int code = static_cast<int>('A'); code <= static_cast<int>('Z'); ++code) {
        auto& currentButtonState = m_buttonStates[code];
        auto& lastButtonState    = m_lastButtonStates[code];

        if (glfwGetKey(m_glfwWindow, code) == GLFW_PRESS) {
            if (lastButtonState == RELEASE) {
                currentButtonState = PRESS;
            } else {
                currentButtonState = PRESSED;
            }
            lastButtonState = PRESS;
        } else if (glfwGetKey(m_glfwWindow, code) == GLFW_RELEASE) {
            if (lastButtonState == PRESS) {
                currentButtonState = RELEASE;
            } else {
                currentButtonState = RELEASED;
            }
            lastButtonState = RELEASE;
        }
    }
}

void InputEvent::setMousePosCenter() const
{
    double screenWidth  = m_game.getScreenWidth();
    double screenHeight = m_game.getScreenHeight();
    glfwSetCursorPos(m_glfwWindow, screenWidth / 2, screenHeight / 2);
}

void InputEvent::changeCursorVisibility(int visibilityType) const
{
    glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, visibilityType);
}

InputEvent::ButtonState InputEvent::getButtonState(int mouseButtonType) const
{
    const auto& iter = m_buttonStates.find(mouseButtonType);
    if (iter == m_buttonStates.end()) {
        assert(false);
    }
    return iter->second;
}
}  // namespace wander_csm_test
