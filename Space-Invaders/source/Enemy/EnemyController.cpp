#include "../../header/Enemy/EnemyController.h"
#include "../../header/Enemy/EnemyView.h"
#include "../../header/Enemy/EnemyModel.h"
#include "../../header/Enemy/EnemyConfig.h"
#include "../../header/Global/ServiceLocator.h"
#include "../../header/Entity/EntityConfig.h"
#include "../../header/Player/PlayerController.h"
#include "../../header/Particle/ParticleSystem.h"
#include "../../header/Sound/SoundService.h"
#include <random>

namespace Enemy
{
    using namespace Global;
    using namespace Time;
    using namespace Collision;
    using namespace Entity;
    using namespace Player;
    using namespace Sound;

    EnemyController::EnemyController(EnemyType type)
    {
        enemy_view = std::make_unique<EnemyView>();
        enemy_model = std::make_unique<EnemyModel>(type);
    }

    void EnemyController::initialize()
    {
        enemy_model->initialize();
        enemy_model->setEnemyPosition(getRandomInitialPosition());
        enemy_view->initialize(this);
    }

    void EnemyController::update()
    {
        move();
        enemy_view->update();
        handleOutOfBounds();
    }

    void EnemyController::render()
    {
        enemy_view->render();
    }

    sf::Vector2f EnemyController::getRandomInitialPosition()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> disX(enemy_model->left_most_position.x, enemy_model->right_most_position.x);
        std::uniform_real_distribution<float> disY(enemy_model->left_most_position.y, enemy_model->right_most_position.y);
        float x_position = disX(gen);
        float y_position = disY(gen);
        return sf::Vector2f(x_position, y_position);
    }

    void EnemyController::handleOutOfBounds()
    {
        sf::Vector2f enemyPosition = getEnemyPosition();
        sf::Vector2u windowSize = ServiceLocator::getInstance()->getGraphicService()->getGameWindow()->getSize();

        if (enemyPosition.x < 0 || enemyPosition.x > windowSize.x ||
            enemyPosition.y < 0 || enemyPosition.y > windowSize.y)
        {
            destroy();
        }
    }

    sf::Vector2f EnemyController::getEnemyPosition() const
    {
        return enemy_model->getEnemyPosition();
    }

    void EnemyController::onCollision(ICollider* other_collider)
    {
        if (auto player_controller = dynamic_cast<PlayerController*>(other_collider))
        {
            processScore();
            destroy();
        }
    }

    void EnemyController::processScore()
    {
        ServiceLocator::getInstance()->getPlayerService()->increaseEnemiesKilled(1);
    }

    void EnemyController::destroy()
    {
        ServiceLocator::getInstance()->getParticleService()->spawnParticleSystem(enemy_model->getEnemyPosition(),
            Particle::ParticlesType::EXPLOSION);

        ServiceLocator::getInstance()->getSoundService()->playSound(SoundType::EXPLOSION);
        ServiceLocator::getInstance()->getEnemyService()->destroyEnemy(this);
    }
}
