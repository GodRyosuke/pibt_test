#include "actor.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "game.hpp"
#include "input_event.hpp"

// #include "core/component/component.hpp"
// #include "core/component/sprite_component.hpp"
// #include "core/viewer.hpp"
// #include "util/logging.hpp"

namespace wander_csm_test {
Actor::Actor(Game& game, const std::string& name)
    : m_id(boost::lexical_cast<std::string>(boost::uuids::random_generator()())),
      m_name(name),
      m_game(game),
      m_recomputeWorldTransform(true),
      m_position(0.0f, 0.0f, 0.0f),
      m_orientation(wu::Mat4::identity()),
      m_scale(1.0f, 1.0f, 1.0f)
{
    // m_viewer.addActor(*this);
}

Actor::Actor(Actor&& other) noexcept
    : m_game(other.m_game),
      m_name(other.m_name),
      m_id(other.m_id),
      m_position(std::move(other.m_position)),
      m_orientation(std::move(other.m_orientation)),
      m_scale(std::move(other.m_scale)),
      m_worldTransform(std::move(other.m_worldTransform)),
      m_components(std::move(other.m_components))
{
    other.m_id   = "";
    other.m_name = "";
}

Actor::~Actor()
{
    m_components.clear();
}

void Actor::update()
{
    computeWorldTransform();

    updateComponents();
    updateActor();

    computeWorldTransform();
}

void Actor::updateComponents()
{
    for (auto& comp : m_components) {
        comp.get().update();
        // comp.update();
    }
}

void Actor::input(const InputEvent& event)
{
    for (auto& comp : m_components) {
        comp.get().input(event);
        // comp.input(event);
    }

    inputActor(event);
}

void Actor::computeWorldTransform()
{
    if (m_recomputeWorldTransform) {
        m_recomputeWorldTransform  = false;
        const wu::Mat4 translation = wu::Mat4::translation(m_position);
        const wu::Mat4 scale       = wu::Mat4::scale(m_scale);
        m_worldTransform           = translation * m_orientation * scale;

        // Inform components world transform updated
        // for (auto& comp : m_components) {
        //     comp.get().onUpdateWorldTransform();
        //     // comp.onUpdateWorldTransform();
        // }
    }
}

// template <typename CompType>
// CompType& Actor::constructComponent(int updateOrder)
// {
//     auto& comp = m_components.emplace_back(std::move(Component(m_viewer, updateOrder)));
//     return static_cast<CompType&>(comp);
// }

// Component& Actor::constructComponent(int updateOrder)
// {
//     m_components.emplace_back(std::move(Component(m_viewer, updateOrder)));
//     return m_components[m_components.size() - 1];
// }

void Actor::addComponent(Component& component)
{
    int myOrder = component.getUpdateOrder();

    auto iter = m_components.begin();
    for (;
         iter != m_components.end();
         ++iter) {
        if (myOrder < (*iter).get().getUpdateOrder()) {
            // if (myOrder < (*iter).getUpdateOrder()) {
            break;
        }
    }

    // Inserts element before position of iterator
    // auto& pos = m_components.insert(iter, std::move(component));
    m_components.insert(iter, component);
    // Component& comp = *pos;
    // auto compType = comp.getComponentType();
    // if (comp.getComponentType() == "sprite") {
    //     // m_viewer.getRenderer().addSpriteComponent(static_cast<SpriteComponent&>(comp));
    // }
}

void Actor::removeComponent(Component& component)
{
    // auto iter = std::find(m_components.begin(), m_components.end(), component);
    auto iter = std::find_if(m_components.begin(), m_components.end(),
                             [&](const std::reference_wrapper<Component>& comp) { return comp.get() == component; });
    if (iter != m_components.end()) {
        m_components.erase(iter);
    }
}
}  // namespace wander_csm_test
