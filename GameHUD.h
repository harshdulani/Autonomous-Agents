#pragma once
#include "GameEntity.h"

class GameHUD : public GameEntity
{
public:
	~GameHUD() override;
	
	void InitGameHUD();

private:
	void OnScoreUpdate(int newScore);
	void OnLivesUpdate(int newLives);
	void RefreshTextBounds_Score(const std::shared_ptr<sf::Text>& scoreText);

public:

private:
	uint32_t eventHandle_ScoreUpdate_ = 0;
	uint32_t eventHandle_LivesUpdate_ = 0;
	
	std::weak_ptr<sf::Text> scoreText_;
	std::weak_ptr<sf::Text> livesText_;
	sf::Font uiFont_;

	int score_ = -1;
};
