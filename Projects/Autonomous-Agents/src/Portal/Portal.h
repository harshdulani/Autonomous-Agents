#pragma once

#include "../GameEntity.h"
class FSMHandler;

class Portal : public GameEntity
{
public:
	Portal();
	
	void AddVisuals();
	void SetPortalRadius(const float radius);
	void SetLinkedPortal(const std::weak_ptr<Portal>& linkedPortal);
	void InitialiseFSM();

	void OnCollision(std::weak_ptr<GameEntity> other) override;

	void SetIsInCooldown(bool status);
	void EndFSMCooldown();

	sf::Color GetReadyColor() const;
	sf::Color GetWaitColor() const;

	sf::Color GetRenderColor() const;
	void SetRenderColor(const sf::Color& color);
	
private:
	void StartFSMCooldown();
	void SendToLinkedPortal(const std::weak_ptr<GameEntity>& other);

	FSMManager* fsmMgr_ = nullptr;
	std::weak_ptr<Portal> linkedPortal_;
	uint32_t stateMachineHandle_ = 0;

	// render calculations
	int resolution_ = 32;
	float slice_ = 0.f;
	sf::Color readyColor_;
	sf::Color waitColor_;
	sf::Color renderColor_;

	float portalRadius_ = 0.f;
	bool isInCooldown_ = false;
};
