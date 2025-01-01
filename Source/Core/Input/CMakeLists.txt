set(InputSources
	InputComponent.cpp
	InputHandler.cpp
)
set(InputHeaders
	InputComponent.h
	InputHandler.h
)

target_sources(Asteroids PRIVATE ${InputSources} ${InputHeaders})
