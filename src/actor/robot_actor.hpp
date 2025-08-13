#pragma once

#include <queue>
#include "actor.hpp"

namespace wander_csm_test {
class RobotActor : public Actor
{
public:
    RobotActor(Game& game, const std::string& name, const std::string& localizationMapId, const wu::Vec2& initialPos, const wu::Vec3& color);
    void updateActor() override;
    void setGoal(const wu::Vec2& goal) { m_goalQueue.push(goal); }

private:
    void planGlobalPath(const wu::Vec2& goal);

    std::vector<wu::Vec3> m_globalPath;
    std::string           m_instanceMeshCompId;
    std::string           m_localizatinMapActorId;
    double                m_velocity;
    int                   m_nextGoalIdx;
    double                m_angle;
    wu::Vec3              m_color;
    std::queue<wu::Vec2>  m_goalQueue;
};
}  // namespace wander_csm_test
