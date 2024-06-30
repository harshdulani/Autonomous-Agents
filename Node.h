#pragma once

#include <vector>
#include <memory>

class Edge;

class Node
{
public:
	Node();
	virtual ~Node();

	bool operator==(const Node& rhs) const;

	void AddEdge(const std::weak_ptr<Edge>& newEdge);

	const std::vector<std::weak_ptr<Edge>>& GetEdges() const;

private:
	std::vector<std::weak_ptr<Edge>> edges;
};
