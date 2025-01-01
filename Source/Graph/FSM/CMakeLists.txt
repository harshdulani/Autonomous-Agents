set(FSMSources
	FSM.cpp
	FSMManager.cpp
	FSMParameter.cpp
)


set(FSMHeaders
	FSM.h
	FSMManager.h
	FSMParameter.h
)

target_sources(Asteroids PRIVATE ${FSMSources} ${FSMHeaders})
#target_include_directories(Asteroids PRIVATE ${CMAKE_SOURCE_DIR}/Source/Graph/FSM)