set(CoreSources
	Debug.cpp
	Maths.cpp
	MultiTriShape.cpp
	ScreenShaker.cpp
	Timer.cpp
	TimerManager.cpp
)

set(CoreHeaders
	Debug.h
	Event.h
	Maths.h
	MultiTriShape.h
	ScreenShaker.h
	TimerManager.h
	Timer.h
)

add_subdirectory(Driver)
add_subdirectory(ECS)
add_subdirectory(Input)
add_subdirectory(Physics)
add_subdirectory(Pooling)

target_sources(Asteroids PRIVATE ${CoreSources} ${CoreHeaders})
target_include_directories(Asteroids PRIVATE ${CMAKE_SOURCE_DIR}/Source/Core)