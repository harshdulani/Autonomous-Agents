#include "GameHUD.h"
#include "PlayerShip.h"

GameHUD::~GameHUD()
{
	System::GetInstance()->GetGame().Event_ScoreUpdate.Unsubscribe(eventHandle_ScoreUpdate_);
	System::GetInstance()->GetGame().Event_LivesUpdate.Unsubscribe(eventHandle_LivesUpdate_);
}

void GameHUD::InitGameHUD()
{
	if (!uiFont_.loadFromFile("RecursiveSansLnrSt-Med.ttf"))
	{
		// error...
	}

	System::GetInstance()->GetGame().Event_ScoreUpdate.Subscribe(BindSubscriber(&GameHUD::OnScoreUpdate, this));
	System::GetInstance()->GetGame().Event_LivesUpdate.Subscribe(BindSubscriber(&GameHUD::OnLivesUpdate, this));

	livesText_ = CreateDrawable<sf::Text>();	
	if (auto livesText = livesText_.lock())
	{
		livesText->setCharacterSize(14);
		livesText->setStyle(sf::Text::Regular);
		livesText->setFont(uiFont_);	
		livesText->setString("Lives: 3");
		livesText->setPosition(12.5f, 12.5f);
	}

	scoreText_ = CreateDrawable<sf::Text>();
	if (auto scoreText = scoreText_.lock())
	{
		scoreText->setCharacterSize(14);
		scoreText->setStyle(sf::Text::Regular);
		scoreText->setFont(uiFont_);
		scoreText->setString("Score: 0");	
		RefreshTextBounds_Score(scoreText);
	}
}

void GameHUD::OnScoreUpdate(int newScore)
{
	if (auto scoreText = scoreText_.lock())
	{
		scoreText->setString("Score: " + std::to_string(newScore));
		// todo: don't hard code!
		if (score_ <= 99 && newScore > 99)
		{
			RefreshTextBounds_Score(scoreText);
		}
	}
}

void GameHUD::OnLivesUpdate(int newLives)
{
	if (auto livesText = livesText_.lock())
		livesText->setString("Lives: " + std::to_string(newLives));
}

void GameHUD::RefreshTextBounds_Score(const std::shared_ptr<sf::Text>& scoreText)
{
	scoreText->setPosition(System::GetInstance()->GetWindowWidth() * 0.5f - scoreText->getGlobalBounds().width, 12.5f);	
}
