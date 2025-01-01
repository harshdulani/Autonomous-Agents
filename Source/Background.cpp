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
	for (int i = 0; i < starCount_; ++i)
	{
		stars_[i] = {{Maths::GetRandFloat(0, cameraWidth_),Maths::GetRandFloat(0, cameraHeight_)},
								Maths::LerpColor({255, 255, 51},
												{51, 153, 255},
												Maths::GetRandFloat(0, 1.f))
							};
	}
	RefreshTwinkler();

	SetRotation(0.f);
	ship_ = std::move(ship);
}

void Background::Update(const float deltaTime)
{
	// twinkler
	timeElapsed_ += deltaTime;
	auto dimVal = abs(cosf(timeElapsed_));

	for (auto& twinklingStar : twinklers_)
	{
		stars_[twinklingStar.first].color = Maths::LerpColor(sf::Color::Black, twinklingStar.second, dimVal);
	}
	TryRefreshTwinkler(dimVal);

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

void Background::TryRefreshTwinkler(float dimVal)
{
	if (!hasDimmed_ && dimVal > 0.01f)
	{
		return;
	}
	if (dimVal < 0.01f)
	{
		//reached min brightness
		hasDimmed_ = true;
		return;
	}
	if (dimVal < 0.9f)
	{
		// hasn't reached max brightness yet after dimming
		return;
	}
	RefreshTwinkler();
}

void Background::RefreshTwinkler()
{
	hasDimmed_ = false;
	for (const auto& item : twinklers_)
	{
		stars_[item.first].color = item.second;
	}
	twinklers_.clear();
	int count = static_cast<int>(static_cast<float>(starCount_) * 0.5f);
	for (int i = 0; i < count; i++)
	{
		int index = static_cast<int>(Maths::GetRandFloat(0.f, static_cast<float>(starCount_ - 1)));
		twinklers_.emplace(index, stars_[index].color);
	}
}
