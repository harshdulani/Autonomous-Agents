#pragma once

#include "GameEntity.h"

#include <vector>
#include <memory>

class PlayerShip;

class Background : public GameEntity
{
public:
	void Initialise(std::weak_ptr<PlayerShip> ship,
					float cameraWidth,
					float cameraHeight,
					int starCount,
					float parallaxSpeed);

	void Update(float deltaTime) override;
	void Render(sf::RenderWindow& Window) const override;

private:
	float cameraHeight_ = 0;
	float cameraWidth_ = 0;

	//star parallax
	std::weak_ptr<PlayerShip> ship_;
	float parallaxSpeed_ = 0;

	// star sliding window
	int starCount_ = 0;
	sf::VertexArray stars_;	

	// per-star twinkle data: phase offset (-1 = no twinkle), base color
	std::vector<float> twinklePhases_;
	std::vector<sf::Color> baseColors_;
	float twinkleTime_ = 0;
};
