#include "component.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "actor/actor.hpp"
#include "game.hpp"
#include "input_event.hpp"

namespace wander_csm_test {
Component::Component(Game& game, const std::string& ownerId, int updateOrder)
    : m_updateOrder(updateOrder),
      m_id(boost::lexical_cast<std::string>(boost::uuids::random_generator()())),
      m_game(game),
      m_ownerId(ownerId)
{
    // m_owner.addComponent(std::move(*this));
}

Component::Component(Component&& other) noexcept
    : m_updateOrder(other.m_updateOrder),
      m_id(std::move(other.m_id)),
      m_ownerId(std::move(other.m_ownerId)),
      m_game(other.m_game)
//   m_owner(other.m_owner)
{
    other.m_id      = "";
    other.m_ownerId = "";
}

Component::~Component()
{
    // m_owner.removeComponent(this);
}

Component& Component::operator=(Component&& other) noexcept
{
    if (*this == other) {
        return *this;
    }
    m_updateOrder = other.m_updateOrder;
    m_id          = std::move(other.m_id);
    m_ownerId     = std::move(other.m_ownerId);
    // m_viewer = std::move(other.m_viewer); // これが呼ばれるときは、this->m_Viewerはすでに実態を持っているはず
    // m_viewer = other.m_viewer;

    other.m_id      = "";
    other.m_ownerId = "";

    return *this;
}
}  // namespace wander_csm_test
