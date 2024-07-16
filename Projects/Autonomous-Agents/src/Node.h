#pragma once

#include "pch.h"


class Node
{
public:
	Node();
	virtual ~Node();

	bool operator==(const Node& rhs) const;

	void AddEdge(const std::weak_ptr<class Edge>& newEdge);

	const std::vector<std::weak_ptr<class Edge>>& GetEdges() const;

private:
	std::vector<std::weak_ptr<class Edge>> edges_;
};
