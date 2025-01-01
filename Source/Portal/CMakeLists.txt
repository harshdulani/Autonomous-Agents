set(PortalSources
	Portal.cpp
	Portal_ReadyState.cpp
	Portal_WaitState.cpp
	)

set(PortalHeaders
	Portal.h
	Portal_ReadyState.h
	Portal_WaitState.h
)

target_sources(Asteroids PRIVATE ${PortalSources} ${PortalHeaders})
#target_include_directories(Asteroids PRIVATE ${CMAKE_SOURCE_DIR}/Source/Portal)