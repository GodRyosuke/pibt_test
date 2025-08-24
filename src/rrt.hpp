#pragma once

#include <atomic>
#include <future>
#include <random>
#include <string>
#include <vector>
#include "util/wander_math.hpp"

namespace wander_csm_test {
class Game;
class Texture;

class RRT
{
public:
    RRT(const Game& game, const std::string& localizationMapActorId);
    void                         startPlan(const wu::Vec2& startPos, const wu::Vec2& goalPos, const std::vector<wu::Vec2>& obstacles);
    const std::vector<wu::Vec2>& getPlanResult() const { return m_planResult; }
    bool                         isPlanFinished() const { return m_isFinishedCalcRRT; }
    void                         update();
    void                         stopPlan() { m_isStopPlanning = true; }

private:
    std::vector<wu::Vec2> asyncPlan(const wu::Vec2& startPos, const wu::Vec2& goalPos, const std::vector<wu::Vec2>& obstacles);

    const Game&           m_game;
    std::string           m_localizatinMapActorId;
    std::vector<wu::Vec2> m_passPixels;
    double                m_goalSelectionProb;
    double                m_nodeDistance;
    double                m_rrtStarSearchGamma;
    std::size_t           m_maxIterNum;
    double                m_nearGoalDist;
    double                m_obstacleDist;

    std::atomic<bool> m_isStopPlanning;

    std::mt19937                           m_randomEngine;
    std::uniform_int_distribution<int>     m_passPixelSelectDist;
    std::uniform_real_distribution<double> m_realDist;

    std::future<std::vector<wu::Vec2>> m_planFuture;
    bool                               m_isFinishedCalcRRT;
    std::vector<wu::Vec2>              m_planResult;
};
}  // namespace wander_csm_test
