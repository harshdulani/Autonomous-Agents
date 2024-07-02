#include "AShootingStrategy.h"
#include "System.h"
#include "ObjectManager.h"
#include "SceneComponent.h"
#include "Bullet.h"

std::weak_ptr<Bullet> AShootingStrategy::SpawnBullet(sf::Vector2f Position)
{
	return System::GetInstance()->GetObjectMgr()->CreateGameEntity<Bullet>(Position);
}

bool StandardShootingStrategy::TryShoot(SceneComponent& Muzzle, bool bPlayer)
{
	sf::Vector2f ForwardVector = Muzzle.GetForwardVector();
	sf::Vector2f shipPos = Muzzle.GetWorldPosition();
	sf::Vector2f position = shipPos + ForwardVector * 10.f;

	bool success = true;
	if (auto b = SpawnBullet(position).lock())
	{
		b->InitialiseStraightBullet(ForwardVector, true);
		b->CreateCollider(4.0f);
		b->SetIsPlayerOwned(bPlayer);
	}
	else { success = false; }
	return success;
}

bool TripleShootingStrategy::TryShoot(SceneComponent& Muzzle, bool bPlayer)
{
	sf::Vector2f ForwardVector = Muzzle.GetForwardVector();
	sf::Vector2f Position = Muzzle.GetWorldPosition();

	sf::Vector2f RightVector = Muzzle.GetRightVector();

	bool success = true;
	if (auto b = SpawnBullet(Position).lock())
	{
		b->InitialiseStraightBullet(ForwardVector, false);
		b->CreateCollider(4.0f);
		b->SetIsPlayerOwned(bPlayer);
		b->SetRotation(Muzzle.GetWorldRotation()); 
	}
	else { success = false; }
	
	if (auto b = SpawnBullet(Position + RightVector * 10.f).lock())
	{
		b->InitialiseStraightBullet(ForwardVector, false);
		b->CreateCollider(4.0f);
		b->SetIsPlayerOwned(bPlayer);
		b->SetRotation(Muzzle.GetWorldRotation()); 
	}
	else { success = false; }

	if (auto b = SpawnBullet(Position - RightVector * 10.f).lock())
	{
		b->InitialiseStraightBullet(ForwardVector, false);
		b->CreateCollider(4.0f);
		b->SetIsPlayerOwned(bPlayer);
		b->SetRotation(Muzzle.GetWorldRotation()); 
	}
	else { success = false; }

	return success;
}

SineShootingStrategy::SineShootingStrategy() :
	WaveAmplitude(0.f),
	WaveFrequency(0.f)
{
}

bool SineShootingStrategy::TryShoot(SceneComponent& Muzzle, bool bPlayer)
{
	sf::Vector2f direction = Muzzle.GetForwardVector();
	sf::Vector2f shipPos = Muzzle.GetWorldPosition();
	sf::Vector2f position = shipPos + direction * 10.f;
	sf::Vector2f perpendicular = Muzzle.GetRightVector();
	
	bool success = true;
	if (auto b = SpawnBullet(position + perpendicular * 7.5f).lock())
	{
		b->InitialiseSineWaveBullet(direction, perpendicular, -WaveAmplitude, WaveFrequency);
		b->CreateCollider(3.0f);
		b->SetIsPlayerOwned(bPlayer);
	}
	else { success = false; }
	if (auto b = SpawnBullet(position - perpendicular * 7.5f).lock())
	{
		b->InitialiseSineWaveBullet(direction, perpendicular, WaveAmplitude, WaveFrequency);
		b->CreateCollider(3.0f);
		b->SetIsPlayerOwned(bPlayer);
	}
	else { success = false; }
	return success;
}

void SineShootingStrategy::SetSineWaveProperties(float Amplitude, float Frequency)
{
	WaveAmplitude = Amplitude;
	WaveFrequency = Frequency;
}
