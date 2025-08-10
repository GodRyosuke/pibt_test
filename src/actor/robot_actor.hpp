#pragma once

#include "actor.hpp"

namespace wander_csm_test {
class RobotActor : public Actor
{
public:
    RobotActor(Game& game, const std::string& name);
};
}  // namespace wander_csm_test
