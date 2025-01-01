#pragma once

#include "GameEntity.h"
#include <memory>
#include <vector>

class InputComponent;

class ImplicitGrid final : public GameEntity
{
	friend class Physics;
public:
	~ImplicitGrid() override;

	void Update(float deltaTime) override;
	
	void SetupDebugControls();
	void SetDebugGridVisibility(bool val);
	void SetDebugTextVisibility(bool val);

	void InitGrid(int Width, int Height);
	void InitDebugText();
	
	int GetGridCellSizeX() const { return gridCellSizeX_; }
	void SetGridCellSizeX(int val) 
	{
		gridCellSizeX_ = std::max(1, val);
		invGridCellSizeX_ = 1.f / static_cast<float>(gridCellSizeX_);
		RefreshGrid();
	}

	int GetGridCellSizeY() const { return gridCellSizeY_; }
	void SetGridCellSizeY(int val) 
	{
		gridCellSizeY_ = std::max(1, val);
		invGridCellSizeY_ = 1.f / static_cast<float>(gridCellSizeY_);
		RefreshGrid();
	}

private:
	void TestEntityAgainstGrid(int index);
	void TestEntityPairForCollision(const std::shared_ptr<GameEntity>& a,
									const std::shared_ptr<GameEntity>& b);
	
	void RefreshGrid();
	
	void ClearGrids();
	void UpdatePositions();

	void OnEntityCreated(std::weak_ptr<GameEntity> entity);
	void OnEntityDestroyed(std::weak_ptr<GameEntity> entity);
	void CleanUpOldObjects();

	void Add32ObjectCollection();
	void Remove32ObjectCollection();
	void GenerateDebugGrid();

	std::vector<std::weak_ptr<GameEntity>> GetGridCellOccupiers(int x, int y);

	void GetGridCellOccupiers(int x, int y,
							  std::vector<std::weak_ptr<GameEntity>>& inVector);

	void GetEntityBounds(const std::shared_ptr<GameEntity>& entity, int& x1, int& y1, int& x2, int& y2) const;
	void GetRadiusBounds(float posX,
						 float posY,
						 float radius,
						 int& x1, int& y1, int& x2, int& y2) const;
	
	std::weak_ptr<GameEntity> GetEntityAtIndex(int index);

	static bool AreColliding(const Collider& a, const Collider& b);
	static float FVectorDistance(const sf::Vector2f& a, const sf::Vector2f& b);
	static int GetBitIndex(const int32_t& mask);

	void RemoveObjectAtIndex(int32_t index);

	void UpdateCollisionPairCount();

private:
	std::vector<std::weak_ptr<GameEntity>> entities_;

	// stores a collection of 31 objects (in a int32_t - 31 bits for data, 1 bit for sign)
	// each of which can be accessed using the bit index in global arrays
	int numOfObjectCollections_ = 0; // by default maintain one entire grid for 1 collection of 32 entities
	const int collectionSize_ = 31; // 31 data bits and 1 sign bit

	//stores a 2d/jagged vector of type uint32 of type rowBitVector[numObjectsDiv32][gridHeight]
	//each set bit in the uint signifies that there is an entity present
	// with index bitIndex is present in the gridCell
	std::vector<std::vector<uint32_t>> globalRowCollections_;
	std::vector<std::vector<uint32_t>> globalColumnCollections_;

	int gridWidth_ = 0;
	int gridHeight_ = 0;

	//size of square cells in px
	int gridCellSizeX_ = 64;
	int gridCellSizeY_ = 64;

	float invGridCellSizeX_ = 0.f;
	float invGridCellSizeY_ = 0.f;

	//use for sizing grid cells up and down
	int gridSizeChanger_ = 1;

	// no of grid cells per column/ horizontally
	int gridCellCountX_ = 0;
	// no of grid cells per row/ vertically
	int gridCellCountY_ = 0;

	//debug
	bool bCollisionPairsVisibility_ = false;
	int collisionPairsCount_ = 0;

	std::weak_ptr<InputComponent> inputC;
	
	sf::Font font_;

	// making these a weak ptr because we want to toggle these using numpad
	// else would just leave these empty and have functions to Create<these>Drawables()
	std::weak_ptr<sf::Text> collisionPairsText_;
	std::weak_ptr<sf::VertexArray> debugGrid_;

	bool bDebugGridVisibility_ = false;
	sf::Color renderColor_;
	
	//event handles
	uint32_t eventHandle_EntityCreated_ = 0;
	uint32_t eventHandle_EntityDestroyed_ = 0;
	uint32_t eventHandle_LevelEnd_ = 0;

	struct FVector2
	{
		float x;
		float y;
	};
};
