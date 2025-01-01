#pragma once

#include "GameEntity.h"

#include <map>
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
	//twinkler
	void TryRefreshTwinkler(float dimVal);
	void RefreshTwinkler();
	
	float cameraHeight_ = 0;
	float cameraWidth_ = 0;

	//star parallax
	std::weak_ptr<PlayerShip> ship_;
	float parallaxSpeed_ = 0;

	// star sliding window
	int starCount_ = 0;
	sf::VertexArray stars_;	

	// only some stars twinkle, and after they each dim cycle, a new star is selected to twinkle
	std::map<int, sf::Color> twinklers_;
	float timeElapsed_ = 0;
	bool hasDimmed_ = false;
};
