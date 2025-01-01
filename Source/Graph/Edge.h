#pragma once
#include <memory>

class Node;

class Edge
{
public:
	virtual ~Edge() = default;
	
	std::weak_ptr<Node> GetSrcNode() const;
	std::weak_ptr<Node> GetDestNode() const;

	void SetSrcNode(std::weak_ptr<Node> node);
	void SetDestNode(std::weak_ptr<Node> node);

private:
	std::weak_ptr<Node> srcNode_;
	std::weak_ptr<Node> destNode_;
};