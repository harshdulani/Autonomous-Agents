#pragma once
#include "pch.h"

#include "GameEntity.h"

class Asteroid : public GameEntity
{
public:
	void OnCollision(std::weak_ptr<GameEntity> other) override;
	
	void InitAsteroid(float radius, sf::Color color, float angularMomentum, int size);
	void AsteroidHit();

	int GetSize() const;
	
private:
	void SpawnExplosion(int oldSize);
	int size_ = -1;
};
