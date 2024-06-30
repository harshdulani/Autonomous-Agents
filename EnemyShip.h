#pragma once

#include "Bot.h"

class ShootingComponent;
class ParticleSystem;
class FSMManager;
class GroupAttackingPolicy;

class EnemyShip : public Bot
{
public:
	EnemyShip();

	void Update(float deltaTime) override;

	void OnCollision(std::weak_ptr<GameEntity> other) override;

	void SetTarget(std::weak_ptr<GameEntity> target);
	void InitObstacleAvoidance(float avoidanceRadius, float collisionRadius);
	void InitThrusterParticles();
	void InitialiseHealthIndicator(float radius);

	void InitialiseAutoShootFSM();
	virtual void InitialiseChaseFSM(float minDistance, 
									float pathRadius);
	virtual void BulletHit();

	std::weak_ptr<GameEntity> GetTarget() const { return target_; }

	sf::Vector2f GetSeekLocation() const { return seekLocation_; }
	void SetSeekLocation(const sf::Vector2f& val) { seekLocation_ = val; }

	std::weak_ptr<ShootingComponent> GetShootingComponent() { return shootingComponent; }
	std::weak_ptr<FSM> GetAutoShootFSM() const;
	std::weak_ptr<FSM> GetChaseFSM() const;

	// Group attack behavior policy
	void AttackComplete();

	void SetAttacking(bool val);
	bool IsAttacking() const;

protected:
	sf::Vector2f SeekTarget();

	std::weak_ptr<sf::CircleShape> GetHealthIndicator() const { return healthIndicator_; }
	void UpdateHealthIndicator();
	
	void SetChaseFSMHandle(uint32_t val) { fsmHandle_chaseFSM = val; }

	//std::weak_ptr<ParticleSystem> thruster;

private:
	sf::Vector2f AvoidObstacles();
	std::weak_ptr<GroupAttackingPolicy> groupAttack_;
	std::weak_ptr<ShootingComponent> shootingComponent;
	
	//FSMs
	FSMManager* fsmMgr;
	uint32_t fsmHandle_chaseFSM = 0;
	uint32_t fsmHandle_autoShootFSM_ = 0;

	//collision avoidance
	float collisionRadius_ = -1.f;
	float obstacleAvoiderRadius_ = -1.f;

	sf::Vector2f seekLocation_;
		
	// health indicator
	std::weak_ptr<sf::CircleShape> healthIndicator_;

	std::weak_ptr<GameEntity> target_;
	std::vector<std::weak_ptr<GameEntity>> obstacles_;
};
