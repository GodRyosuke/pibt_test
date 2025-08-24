#pragma once

#include <queue>
#include "actor.hpp"
#include "rrt.hpp"

namespace wander_csm_test {
class RobotActor : public Actor
{
public:
    enum State
    {
        NOT_ASSIGNED,
        PLANNING,
        MOVEING,
        NUM_STATE,
    };

    RobotActor(Game& game, const std::string& name, const std::string& localizationMapId, const wu::Vec2& initialPos, const wu::Vec3& color);
    ~RobotActor();
    void         updateActor() override;
    void         setGoal(const wu::Vec2& goal) { m_goalQueue.push(goal); }
    unsigned int getPriority() const { return m_priority; }
    void         shutdown() override;

private:
    void initializePriority();
    void planGlobalPath(const wu::Vec2& goal, const std::vector<wu::Vec2>& obstacls);
    void renderGlobalPath();

    std::vector<wu::Vec3>    m_globalPath;
    std::string              m_instanceMeshCompId;
    std::string              m_localizatinMapActorId;
    double                   m_velocity;
    int                      m_nextGoalIdx;
    double                   m_angle;
    wu::Vec3                 m_color;
    std::queue<wu::Vec2>     m_goalQueue;
    unsigned int             m_priority;
    std::vector<std::string> m_robotNames;
    State                    m_state;

    std::vector<std::string> m_resolveConflictRobots;

    RRT m_rrt;
};
}  // namespace wander_csm_test
