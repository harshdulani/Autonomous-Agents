#include "Background.h"
#include <utility>
#include "System.h"
#include "Ships/PlayerShip.h"
#include "Maths.h"
#include "Physics/PhysicsComponent.h"

void Background::Initialise(std::weak_ptr<PlayerShip> ship,
							float cameraWidth,
							float cameraHeight,
							int starCount,
							float parallaxSpeed)
{
	cameraHeight_ = cameraHeight;
	cameraWidth_ = cameraWidth;
	parallaxSpeed_ = parallaxSpeed;

	starCount_ = starCount;
	stars_ = sf::VertexArray(sf::Points, starCount_);
	twinklePhases_.resize(starCount_);
	baseColors_.resize(starCount_);
	for (int i = 0; i < starCount_; ++i)
	{
		stars_[i] = {{Maths::GetRandFloat(0, cameraWidth_),Maths::GetRandFloat(0, cameraHeight_)},
								Maths::LerpColor({255, 255, 51},
												{51, 153, 255},
												Maths::GetRandFloat(0, 1.f))
							};
		baseColors_[i] = stars_[i].color;
		if (Maths::GetRandFloat(0, 1.f) < 0.5f)
			twinklePhases_[i] = Maths::GetRandFloat(0.f, 6.28318f);
		else
			twinklePhases_[i] = -1.f;
	}

	SetRotation(0.f);
	ship_ = std::move(ship);
}

void Background::Update(const float deltaTime)
{
	// twinkler - each star twinkles independently with its own phase offset
	twinkleTime_ += deltaTime;
	for (int i = 0; i < starCount_; ++i)
	{
		if (twinklePhases_[i] >= 0.f)
		{
			float dimVal = fabs(cosf(twinkleTime_ + twinklePhases_[i]));
			stars_[i].color = Maths::LerpColor(sf::Color::Black, baseColors_[i], dimVal);
		}
	}

	if (auto ship = ship_.lock())
	{
		auto velocity = -ship->GetPhysicsComponent()->GetVelocity();
		// positions
		for (int i = 0; i < starCount_; ++i)
		{
			stars_[i].position.x += velocity.x * parallaxSpeed_ * deltaTime;
			stars_[i].position.y += velocity.y * parallaxSpeed_ * deltaTime;
			stars_[i].position.x = Maths::WrapModulo(stars_[i].position.x,
													0.f,
													cameraWidth_);
			stars_[i].position.y = Maths::WrapModulo(stars_[i].position.y,
													0.f,
													cameraHeight_);
		}
	}
}

void Background::Render(sf::RenderWindow& Window) const
{
	GameEntity::Render(Window);
	
	Window.draw(stars_);
}


