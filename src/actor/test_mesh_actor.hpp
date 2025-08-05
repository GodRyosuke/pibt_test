#pragma once

#include "actor/actor.hpp"

namespace wander_csm_test {
class TestMeshActor : public Actor
{
public:
    TestMeshActor(Game& game);
    void updateActor() override;

private:
    double m_angle;
};
}  // namespace wander_csm_test
