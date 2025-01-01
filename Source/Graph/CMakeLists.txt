set(GraphSources
	Edge.cpp
	Graph.cpp
	Node.cpp
	StateNode.cpp
	TransitionEdge.cpp
)

set(GraphHeaders
	Edge.h
	Graph.h
	Node.h
	StateNode.h
	TransitionEdge.h
)

target_sources(Asteroids PRIVATE ${GraphSources} ${GraphHeaders})
add_subdirectory(FSM)