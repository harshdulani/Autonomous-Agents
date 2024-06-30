#include "Node.h"
#include "Edge.h"

Node::Node()
{
	edges = std::vector<std::weak_ptr<Edge>>();
}

Node::~Node()
{
	for (auto& edge : edges)
	{
		edge.reset();
	}
	edges.clear();
}

bool Node::operator==(const Node& rhs) const { return this == &rhs; }

void Node::AddEdge(const std::weak_ptr<Edge>& newEdge)
{
	edges.push_back(newEdge);
}

const std::vector<std::weak_ptr<Edge>>& Node::GetEdges() const
{
	return edges;
}
