set(ShipSources
	AShootingStrategy.cpp
	AutoShoot_SuspendedState.cpp
	AutoShoot_WaitAndShootState.cpp
	Base_WaitState.cpp
	BaseVehicle.cpp
	Bot.cpp
	Bullet.cpp
	Enemy_ChaseState.cpp
	Enemy_RevolveState.cpp
	EnemyShip.cpp
	PlayerShip.cpp
	GroupAttackingPolicy.cpp
	ShootingComponent.cpp
)

set(ShipHeaders
	AShootingStrategy.h
	AutoShoot_SuspendedState.h
	AutoShoot_WaitAndShootState.h
	Base_WaitState.h
	BaseVehicle.h
	Bot.h
	Bullet.h
	Enemy_ChaseState.h
	Enemy_RevolveState.h
	EnemyShip.h
	GroupAttackingPolicy.h
	PlayerShip.h
	ShootingComponent.h
)


target_sources(Asteroids PRIVATE ${ShipSources} ${ShipHeaders})
#target_include_directories(Asteroids PRIVATE ${CMAKE_SOURCE_DIR}/Source/Ships)