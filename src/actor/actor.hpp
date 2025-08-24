#pragma once

#include <string>
#include <vector>

#include <boost/operators.hpp>
#include "util/wander_math.hpp"

#include "component/component.hpp"
// #include "core/component/sprite_component.hpp"
// #include "input_event.hpp"
// #include "core/viewer.hpp"

namespace wander_csm_test {
class Game;
class InputEvent;

class Actor : private boost::equality_comparable<Actor>
{
public:
    Actor(Game& viewer, const std::string& name);
    Actor(const Actor&) = delete;
    Actor(Actor&&) noexcept;
    virtual ~Actor();
    virtual void shutdown() {}

    void update();
    void input(const InputEvent& event);

    const std::string& getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    void               setName(const std::string& name) { m_name = name; }
    // template <typename ChildActor>
    // std::unique_ptr<ChildActor> get_child_ptr()
    // {
    //     ChildActor* child = static_cast<ChildActor*>(this);
    //     if (!child) {
    //         LOG(error) << "Actor id: " + m_id;
    //         LOG(error) << "Actor name: " + m_name;
    //         throw std::runtime_error("failed to cast child actor");
    //     }
    //     return std::unique_ptr<ChildActor>(child);
    // }

    // Component& constructComponent(int updateOrder = 100);
    // template <typename CompType>
    // CompType& constructComponent(int updateOrder = 100)
    // {
    //     auto& comp = m_components.emplace_back(std::move(Component(m_viewer, updateOrder)));
    //     return static_cast<CompType&>(comp);
    // }
    void addComponent(Component& comp);
    void removeComponent(Component& comp);

    const wu::Mat4& getWorldTransform() const { return m_worldTransform; }
    void            setPosition(const wu::Vec3& position)
    {
        m_position                = position;
        m_recomputeWorldTransform = true;
    }
    void setRotation(const wu::Mat4& rotation)
    {
        m_orientation             = rotation;
        m_recomputeWorldTransform = true;
    }
    void setRotation(float angle)
    {
        m_orientation             = wu::Mat4::rotation(wu::Vec3(0.0f, 0.0f, 1.0f), angle);
        m_recomputeWorldTransform = true;
    }
    void setScale(const wu::Vec3& scale)
    {
        m_scale                   = scale;
        m_recomputeWorldTransform = true;
    }
    void setScale(float scale)
    {
        m_scale                   = wu::Vec3(scale, scale, scale);
        m_recomputeWorldTransform = true;
    }
    const wu::Vec3& getPosition() const { return m_position; }
    wu::Vec3&       getPosition() { return m_position; }

    bool operator==(const Actor& other) const noexcept
    {
        return m_id == other.m_id;
    }

protected:
    virtual void updateActor() {}
    virtual void inputActor(const InputEvent& event) {}
    Game&        m_game;
    bool         m_recomputeWorldTransform;

private:
    void updateComponents();
    void computeWorldTransform();

    std::string m_name;
    std::string m_id;

    wu::Vec3 m_position;
    wu::Mat4 m_orientation;
    wu::Vec3 m_scale;
    wu::Mat4 m_worldTransform;

    std::vector<std::reference_wrapper<Component>> m_components;
    // std::vector<std::reference_wrapper<Component>> m_components;
};
}  // namespace wander_csm_test
