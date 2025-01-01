set(Source_Sources
	Asteroid.cpp
	Background.cpp
	GameHUD.cpp
)

set(Source_Headers
	Asteroid.h
	Background.h
	GameHUD.h
)

add_subdirectory(Core)
add_subdirectory(Graph)
add_subdirectory(ParticleSystem)
add_subdirectory(Portal)
add_subdirectory(Ships)

target_sources(Asteroids PRIVATE ${Source_Sources} ${Source_Headers})
target_include_directories(Asteroids PRIVATE ${CMAKE_SOURCE_DIR}/Source)