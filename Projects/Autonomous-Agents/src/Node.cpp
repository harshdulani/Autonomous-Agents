#include "pch.h"
#include "Node.h"
#include "Edge.h"

Node::Node()
{
	edges_ = std::vector<std::weak_ptr<Edge>>();
}

Node::~Node()
{
	for (auto& edge : edges_)
	{
		edge.reset();
	}
	edges_.clear();
}

bool Node::operator==(const Node& rhs) const { return this == &rhs; }

void Node::AddEdge(const std::weak_ptr<Edge>& newEdge)
{
	edges_.push_back(newEdge);
}

const std::vector<std::weak_ptr<Edge>>& Node::GetEdges() const
{
	return edges_;
}
