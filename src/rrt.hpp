#pragma once

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
    std::vector<wu::Vec2> plan(const wu::Vec2& startPos, const wu::Vec2& goalPos);

private:
    const Game&           m_game;
    std::string           m_localizatinMapActorId;
    std::vector<wu::Vec2> m_passPixels;
    double                m_goalSelectionProb;
    double                m_nodeDistance;
    double                m_rrtStarSearchGamma;
    std::size_t           m_maxIterNum;
    double                m_nearGoalDist;

    std::mt19937                           m_randomEngine;
    std::uniform_int_distribution<int>     m_passPixelSelectDist;
    std::uniform_real_distribution<double> m_realDist;
};
}  // namespace wander_csm_test
