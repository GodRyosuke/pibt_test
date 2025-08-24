#include "robot_actor.hpp"
#include <iostream>
#include "component/instanced_mesh_component.hpp"
#include "component/mesh_component.hpp"
#include "definitions.hpp"
#include "game.hpp"
#include "localization_map_actor.hpp"
#include "rrt.hpp"

namespace wander_csm_test {
RobotActor::RobotActor(Game& game, const std::string& name, const std::string& localizationMapId, const wu::Vec2& initialPos, const wu::Vec3& color)
    : Actor(game, name),
      m_localizatinMapActorId(localizationMapId),
      m_velocity(0.5),
      m_nextGoalIdx(-1),
      m_angle(wu::PI / 2.0),
      m_color(color),
      m_priority(0),
      m_robotNames({"no1", "no2"}),
      m_state(NOT_ASSIGNED),
      m_rrt(game, localizationMapId)
{
    MeshComponent& meshComponent = game.createMeshComponent(getId(), std::string(ASSET_PATH) + "robot_agent/robot_agent.obj", "testMeshShader");
    meshComponent.setColor(m_color);
    addComponent(meshComponent);

    setPosition(wu::Vec3(initialPos.x(), initialPos.y(), 0));
    setScale(0.5 / 2.0);

    InstancedMeshComponent& instancedMeshComponent = game.createInstancedMeshComponent(getId(), std::string(ASSET_PATH) + "circle/circle.obj", "instancedMeshShader");
    m_instanceMeshCompId                           = instancedMeshComponent.getId();
    addComponent(instancedMeshComponent);

    initializePriority();
}

RobotActor::~RobotActor()
{
    m_rrt.stopPlan();
}

void RobotActor::updateActor()
{
    m_rrt.update();
    if ((m_goalQueue.size() != 0) && (m_nextGoalIdx == -1) && (m_state == NOT_ASSIGNED)) {
        // 次の目的地への経路計画を実行
        m_state = PLANNING;
        planGlobalPath(m_goalQueue.front(), {});
        return;
    }

    if (m_state == PLANNING) {
        std::cout << "in planning" << std::endl;
        if (m_rrt.isPlanFinished()) {
            std::cout << "plan finished in robot update" << std::endl;
            auto globalPathReverse = m_rrt.getPlanResult();

            if (globalPathReverse.size() == 0) {
                m_nextGoalIdx = -1;
                m_state       = NOT_ASSIGNED;
                m_goalQueue.pop();
                return;
            }

            m_state                     = MOVEING;
            const auto& localizationMap = m_game.getActor<LocalizationMapActor>(m_localizatinMapActorId);
            m_globalPath.clear();
            m_globalPath.reserve(globalPathReverse.size());
            for (int i = globalPathReverse.size() - 1; i >= 0; i--) {
                const auto& path = globalPathReverse[i];
                m_globalPath.emplace_back(wu::Vec3(path.x(), path.y(), 0));
            }
            const auto& currentGoal = m_goalQueue.front();
            m_globalPath.emplace_back(wu::Vec3(currentGoal.x(), currentGoal.y(), 0));
            m_nextGoalIdx = 0;

            renderGlobalPath();
        }
        return;
    }

    if (m_nextGoalIdx == -1) {
        return;
    }

    // 目的地へのナビゲーション
    auto& currentPos = getPosition();

    // 衝突しそうな機体がないかチェック
    std::vector<wu::Vec2> obstacls;
    for (const auto& name : m_robotNames) {
        const auto robots = m_game.getActorByName<RobotActor>(name);
        if (robots.size() == 0) {
            continue;
        }
        const auto& robot = robots[0].get();
        if (robot.getName() == getName()) {
            continue;
        }
        // 自分自身ではなかったら

        const auto& robotPos = robot.getPosition();
        if ((currentPos - robotPos).sqNorm() > 1) {
            if (m_resolveConflictRobots.size() > 0) {
                // 衝突が解消している
                auto iter = std::find(m_resolveConflictRobots.begin(), m_resolveConflictRobots.end(), name);
                m_resolveConflictRobots.erase(iter);
            }
            continue;
        }
        // 1mより近くに接近したら
        if (m_priority > robot.getPriority()) {
            // 自分より低いpriorityのロボットが接近してきた場合、相手が移動するまで待機
            return;
        }
        auto iter = std::find(m_resolveConflictRobots.begin(), m_resolveConflictRobots.end(), name);
        if (iter != m_resolveConflictRobots.end()) {
            // すでに衝突の解決をしているロボットなら
            continue;
        }

        // 自分がほかのロボットよりpriorityが低ければ
        obstacls.emplace_back(wu::Vec2(robotPos.x(), robotPos.y()));
        m_resolveConflictRobots.emplace_back(robot.getName());
    }

    const auto& currentGoal = m_goalQueue.front();

    if (obstacls.size()) {
        planGlobalPath(currentGoal, obstacls);
        m_state = PLANNING;
    }

    const double deltaT       = m_game.getDeltaT();
    double       nextGoalDist = (m_globalPath[m_nextGoalIdx] - currentPos).norm();
    while (true) {
        if ((nextGoalDist > 0.3) && (m_nextGoalIdx < (m_globalPath.size() - 1))) {
            break;
        }
        if ((nextGoalDist < 0.1) && (m_nextGoalIdx == (m_globalPath.size() - 1))) {
            // 目的地へ到着
            m_nextGoalIdx = -1;
            std::cout << "Robot \"" << getName() << "\" has reached the destination: " << currentPos.x() << ", " << currentPos.y() << std::endl;
            m_state = NOT_ASSIGNED;
            m_goalQueue.pop();
            return;
        }
        if (m_nextGoalIdx == (m_globalPath.size() - 1)) {
            break;
        }
        m_nextGoalIdx++;
        nextGoalDist = (m_globalPath[m_nextGoalIdx] - currentPos).norm();
    }

    // pure pursuit法による制御
    const auto& nextGoal      = m_globalPath[m_nextGoalIdx];
    double      nextGoalAngle = std::atan2(nextGoal.y() - currentPos.y(), nextGoal.x() - currentPos.x());
    if (nextGoalAngle < 0) {
        nextGoalAngle += 2 * wu::PI;
    }

    double angleAlpha      = nextGoalAngle - m_angle;
    double robotVelocity   = std::min((currentPos - m_globalPath[m_globalPath.size() - 1]).norm() * 0.3, m_velocity);
    double angularVelocity = 2 * m_velocity * std::sin(angleAlpha) / nextGoalDist;

    m_angle += angularVelocity * deltaT;
    currentPos += wu::Vec3(std::cos(m_angle), std::sin(m_angle), 0) * robotVelocity * deltaT;
    setRotation(m_angle - wu::PI / 2.0);
}

void RobotActor::shutdown()
{
    m_rrt.stopPlan();
    m_state = NOT_ASSIGNED;
}

void RobotActor::planGlobalPath(const wu::Vec2& goal, const std::vector<wu::Vec2>& obstacls)
{
    // RRTで経路計画
    const auto& currentPos = getPosition();
    std::cout << "start planning: " << currentPos.x() << " " << currentPos.y() << std::endl;

    m_rrt.startPlan(wu::Vec2(currentPos.x(), currentPos.y()), goal, obstacls);
}

void RobotActor::renderGlobalPath()
{
    // 経路の描画準備
    const auto toNodeModelTransform = [](const wu::Vec3& position) -> InstancedMeshComponent::ShaderMat4 {
        const auto scale          = wu::Mat4::scale(wu::Vec3(0.3 / 2.0, 0.3 / 2.0, 0.3 / 2.0));
        const auto translation    = wu::Mat4::translation(position);
        wu::Mat4   modelTransform = translation * scale;
        return {
            modelTransform(0, 0), modelTransform(1, 0), modelTransform(2, 0), modelTransform(3, 0),
            modelTransform(0, 1), modelTransform(1, 1), modelTransform(2, 1), modelTransform(3, 1),
            modelTransform(0, 2), modelTransform(1, 2), modelTransform(2, 2), modelTransform(3, 2),
            modelTransform(0, 3), modelTransform(1, 3), modelTransform(2, 3), modelTransform(3, 3)};
    };

    InstancedMeshComponent& instancedMeshComponent = m_game.getMeshComponent<InstancedMeshComponent>(m_instanceMeshCompId);
    instancedMeshComponent.clearData();
    for (const auto& path : m_globalPath) {
        InstancedMeshComponent::InstancedMeshCompData data = {
            toNodeModelTransform(wu::Vec3(path.x(), path.y(), 0.002)),
            wu::Vec4(m_color.x(), m_color.y(), m_color.z(), 0.7)};

        instancedMeshComponent.addData(data);
    }
}

void RobotActor::initializePriority()
{
    unsigned int maxPriority = 0;
    for (const auto& name : m_robotNames) {
        const auto robots = m_game.getActorByName<RobotActor>(name);
        if (robots.size() == 0) {
            continue;
        }
        maxPriority = std::max(robots[0].get().getPriority(), maxPriority);
    }
    m_priority = maxPriority + 1;
}
}  // namespace wander_csm_test
