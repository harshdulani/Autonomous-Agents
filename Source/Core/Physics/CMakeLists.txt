set(PhysicsSources
	Collider.cpp
	ImplicitGrid.cpp
	Physics.cpp
	PhysicsComponent.cpp
)
set(PhysicsHeaders
	Collider.h
	ImplicitGrid.h
	Physics.h
	PhysicsComponent.h
)

target_sources(Asteroids PRIVATE ${PhysicsSources} ${PhysicsHeaders})
