#pragma once

#include <string>

#include <boost/operators.hpp>

// #include "core/input_event.hpp"

namespace wander_csm_test {
// class Actor;
class InputEvent;
class Game;

class Component : private boost::equality_comparable<Component>
{
public:
    explicit Component(Game& game, const std::string& ownerId, int updateOrder = 100);
    // explicit Component(Actor& owner, int updateOrder = 100);
    Component(Component&& other) noexcept;
    Component(const Component& other) = delete;
    virtual ~Component();
    virtual void update() {}
    virtual void input(const InputEvent& event) {}
    virtual void onUpdateWorldTransform() {}

    const std::string&        getId() const { return m_id; }
    int                       getUpdateOrder() const { return m_updateOrder; }
    virtual const std::string getComponentType() const { return ""; }

    bool operator==(const Component& other) const noexcept
    {
        return m_id == other.m_id;
    }
    Component& operator=(const Component&) = delete;
    Component& operator=(Component&& other) noexcept;

protected:
    std::string m_id;
    std::string m_ownerId;
    int         m_updateOrder;
    Game&       m_game;
    // Actor& m_owner;
};
}  // namespace wander_csm_test
