#include "Edge.h"
#include <utility>

std::weak_ptr<Node> Edge::GetSrcNode() const
{
	return srcNode;
}

std::weak_ptr<Node> Edge::GetDestNode() const
{
	return destNode;
}

void Edge::SetSrcNode(std::weak_ptr<Node> node)
{
	srcNode = std::move(node);
}

void Edge::SetDestNode(std::weak_ptr<Node> node)
{
	destNode = std::move(node);
}
