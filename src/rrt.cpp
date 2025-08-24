#include "rrt.hpp"
#include <iostream>
#include "actor/localization_map_actor.hpp"
#include "game.hpp"
#include "texture.hpp"

namespace wander_csm_test {
RRT::RRT(const Game& game, const std::string& localizationMapActorId)
    : m_game(game),
      m_localizatinMapActorId(localizationMapActorId),
      m_randomEngine(0),
      m_realDist(0, 1.0),
      m_goalSelectionProb(0.1),
      m_nodeDistance(0.5),
      m_rrtStarSearchGamma(250.0),
      m_maxIterNum(2000),
      m_nearGoalDist(0.5),
      m_obstacleDist(1.0),
      m_isFinishedCalcRRT(true),
      m_isStopPlanning(false)
{
    const auto& localizationMap = m_game.getActor<LocalizationMapActor>(m_localizatinMapActorId);
    m_passPixels                = localizationMap.getPassPixels();
    m_passPixelSelectDist       = std::uniform_int_distribution<int>(0, m_passPixels.size() - 1);
}

void RRT::update()
{
    if (m_isFinishedCalcRRT) {
        return;
    }

    if (m_planFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
        m_planResult        = m_planFuture.get();
        m_isFinishedCalcRRT = true;
        std::cout << "finished rrt in rrt update" << std::endl;
    }
}

void RRT::startPlan(const wu::Vec2& startPos, const wu::Vec2& goalPos, const std::vector<wu::Vec2>& obstacles)
{
    m_planResult.clear();

    std::cout << "start plan called: " << startPos.x() << " " << startPos.y() << std::endl;

    m_planFuture        = std::async(std::launch::async, [this, startPos, goalPos, obstacles]() -> std::vector<wu::Vec2> {
        return asyncPlan(startPos, goalPos, obstacles);
    });
    m_isFinishedCalcRRT = false;
}

std::vector<wu::Vec2> RRT::asyncPlan(const wu::Vec2& startPos, const wu::Vec2& goalPos, const std::vector<wu::Vec2>& obstacles)
{
    const auto&    localizationMap = m_game.getActor<LocalizationMapActor>(m_localizatinMapActorId);
    const wu::Vec2 pixelStartPos   = localizationMap.toPixelSpace(startPos);
    const wu::Vec2 pixelGoalPos    = localizationMap.toPixelSpace(goalPos);

    if (!localizationMap.isAvailablePixelSpace(pixelStartPos)) {
        return {};
    }
    if (!localizationMap.isAvailablePixelSpace(pixelGoalPos)) {
        return {};
    }

    struct Node
    {
        Node(int parent, const wu::Vec2& pos, double cost)
            : parent(parent),
              pos(pos),
              cost(cost)
        {
        }

        int            parent;
        const wu::Vec2 pos;
        double         cost;
    };
    std::vector<Node> nodes;
    nodes.emplace_back(Node(-1, pixelStartPos, 0));

    // p1は障害物上にないか？
    const auto isCollide1Point = [&obstacles, this](const wu::Vec2& p) -> bool {
        const auto& localizationMap        = m_game.getActor<LocalizationMapActor>(m_localizatinMapActorId);
        double      obstacleDistPixelSpace = m_obstacleDist * 20;
        for (const auto& obstacle : obstacles) {
            wu::Vec2 obstaclePixelSpace = localizationMap.toPixelSpace(obstacle);
            if ((obstaclePixelSpace - p).sqNorm() < (obstacleDistPixelSpace * obstacleDistPixelSpace)) {
                return true;
            }
        }
        return !localizationMap.isAvailablePixelSpace(p);
    };

    // p1, p2の間に障害物がないか？
    const auto isCollide2Point = [&isCollide1Point](const wu::Vec2& p1, const wu::Vec2& p2) -> bool {
        if (isCollide1Point(p1) || isCollide1Point(p2)) {
            return true;
        }

        wu::Vec2 dir       = p2 - p1;
        double   p12SqDist = dir.sqNorm();
        dir.normalize();
        wu::Vec2 currentPos = p1;
        while (true) {
            if (isCollide1Point(currentPos)) {
                return true;
            }
            if (p12SqDist < (currentPos - p1).sqNorm()) {
                break;
            }
            currentPos = currentPos + dir;
        }
        return false;
    };

    int          goalNodeIdx       = -1;
    const double pixelNodeDist     = m_nodeDistance * 20;
    const double pixelNearGoalDist = m_nearGoalDist * 20;

    for (std::size_t iterNum = 0;; iterNum++) {
        if (m_isStopPlanning.load()) {
            return {};
        }

        int      selectedPassPixelIdx = m_passPixelSelectDist(m_randomEngine);
        wu::Vec2 selectedPixelPos     = m_passPixels[selectedPassPixelIdx];
        if (m_realDist(m_randomEngine) < m_goalSelectionProb) {
            // 何回かに1回はgoalを選択
            selectedPixelPos = pixelGoalPos;
        }

        // 選択された点に最も近いnodeの点を探す
        double currentDist    = std::numeric_limits<double>::max();
        int    nearestNodeIdx = -1;
        for (int nodeIdx = 0; nodeIdx < nodes.size(); nodeIdx++) {
            const wu::Vec2& pos  = nodes[nodeIdx].pos;
            double          dist = (pos - selectedPixelPos).sqNorm();
            if (dist < currentDist) {
                currentDist    = dist;
                nearestNodeIdx = nodeIdx;
            }
        }
        if (nearestNodeIdx < 0) {
            std::cout << "failed to find nearest node" << std::endl;
            break;
        }

        const wu::Vec2& nearestNodePos = nodes[nearestNodeIdx].pos;

        wu::Vec2 dir = selectedPixelPos - nearestNodePos;
        dir.normalize();

        wu::Vec2 newPosition = nearestNodePos + dir * pixelNodeDist;

        // newPositionから一定の範囲内にあるnodeを求める
        double searchSqRadius = m_rrtStarSearchGamma * std::log(nodes.size()) / static_cast<double>(nodes.size());
        if (nodes.size() == 1) {
            searchSqRadius = 1600;
        }
        if (searchSqRadius < 400) {
            searchSqRadius = 400;
        }
        double minCost        = std::numeric_limits<double>::max();
        int    minCostNodeIdx = -1;
        for (int i = 0; i < nodes.size(); i++) {
            const auto& node = nodes[i];
            // 一定の範囲内に収まっているか？
            if (searchSqRadius < (newPosition - node.pos).sqNorm()) {
                continue;
            }
            // 一定の範囲内に入っており、間に障害物が挟まっていないか？
            if (isCollide2Point(newPosition, node.pos)) {
                continue;
            }
            double totalCost = (newPosition - node.pos).sqNorm() + node.cost;
            if (totalCost < minCost) {
                minCost        = totalCost;
                minCostNodeIdx = i;
            }
        }
        if (minCostNodeIdx < 0) {
            continue;
        }
        nodes.emplace_back(Node(minCostNodeIdx, newPosition, minCost));

        // goal付近まで到達できたか？
        if ((pixelGoalPos - newPosition).sqNorm() <= (pixelNearGoalDist * pixelNearGoalDist)) {
            goalNodeIdx = nodes.size() - 1;
        }
        if ((iterNum >= m_maxIterNum) && (goalNodeIdx > 0)) {
            break;
        }
    }

    int                   currentNodeIdx = nodes[goalNodeIdx].parent;
    std::vector<wu::Vec2> result;
    while (true) {
        if (currentNodeIdx < 0) {
            break;
        }

        const auto& currentNode = nodes[currentNodeIdx];
        const auto& pos         = localizationMap.toWorldSpace(currentNode.pos);
        result.emplace_back(pos);
        currentNodeIdx = currentNode.parent;
    }

    std::cout << "rrt finished" << std::endl;
    return result;
}
}  // namespace wander_csm_test
