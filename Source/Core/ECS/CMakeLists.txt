set(ECSSources
	Component.cpp
	GameEntity.cpp
	Object.cpp
	ObjectManager.cpp
	PrimitiveComponent.cpp
	SceneComponent.cpp
)
set(ECSHeaders
	Component.h
	GameEntity.h
	GameEntity.inl
	Object.h
	ObjectManager.h
	ObjectManager.inl
	PrimitiveComponent.h
	SceneComponent.h
)

target_sources(Asteroids PRIVATE ${ECSSources} ${ECSHeaders})

target_include_directories(Asteroids PRIVATE ${CMAKE_SOURCE_DIR}/Source/Core/ECS)