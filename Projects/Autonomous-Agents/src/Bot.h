#pragma once

#include "pch.h"
#include "BaseVehicle.h"

class Bot : public BaseVehicle
{
public:
	Bot();

	void Update(float deltaTime) override;
	void OnCollision(std::weak_ptr<GameEntity> other) override;

	virtual void InitialisePerception(float perceptionRadius);

	float GetSeparationRadius() const { return separationRadius_; }
	float GetNeighborRadius() const { return perceptionRadius_; }
	std::vector<std::weak_ptr<Bot>>& GetFlock() { return flock_; }

protected:
	void AddForce(const sf::Vector2f& force);
	void AddFlockingForces();

	sf::Vector2f SeekLocation(const sf::Vector2f& location) const;
	
	sf::Vector2f Separation();
	sf::Vector2f Alignment();
	sf::Vector2f Cohesion();

	void UpdateVelocity(float deltaTime);

	float GetPerceptionRadius() const;
	
	//seeking
	float maxSpeed = 90.f;
	float maxSteerForce = 0.025f;
	float minSteerForce = 0.45f;
	float brakingDistance = 250.f;

	// custom velocity implementation
	sf::Vector2f velocity;
	sf::Vector2f acceleration;	
private:

	//separation - radius for repelling boids
	float separationRadius_ = -1.f;

	//alignment
	float perceptionRadius_ = 50.f ;
	float fov_;

	//movement fsm
	std::weak_ptr<class FSM> movementFSM_;

	std::vector<std::weak_ptr<Bot>> separaters_;
	std::vector<std::weak_ptr<Bot>> flock_;
	
protected:

	template<typename T>
	void TryAddToList(std::shared_ptr<T> entity, std::vector<std::weak_ptr<T>>& list)
	{
		auto it = std::find_if(list.begin(),
							   list.end(),
							   [&entity](const std::weak_ptr<GameEntity> b) -> bool
							   {
								   if (b.expired())
								   {
									   return false;
								   }
								   return entity == b.lock();
							   });
		if (it == list.end())
		{
			list.push_back(entity);
		}
	}

	template<typename T>
	void RemoveFromList(int& index, std::vector<std::weak_ptr<T>>& list)
	{
		if (index >= list.size() || index < 0)
		{
			return;
		}

		// DEFRAGMENTATION of vector by filling in gaps
		if (index == list.size() - 1)
		{
			// there is no need to fill any gap if the object is at the end of the list
			// just make sure that element is not accessed anymore
			list.pop_back();
			return;
		}

		// move pointer to last object stored in list to the new emptied index
		std::iter_swap((list.begin() + index), (list.begin() + list.size() - 1));
		list.pop_back();

		//decre the iterator so that this element we brought from the back of the list is 
		//also traversed
		index--;
	}
};
