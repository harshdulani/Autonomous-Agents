set(DriverSources
	Game.cpp
	Source.cpp
	System.cpp
)
set(DriverHeaders
	Game.h
	System.h
)

add_executable(Asteroids Source.cpp)

target_sources(Asteroids PRIVATE ${DriverSources} ${DriverHeaders})

target_include_directories(Asteroids PRIVATE ${CMAKE_SOURCE_DIR}/Source/Core/Driver)