#include "pch.h"
#include "Edge.h"
#include "Node.h"

std::weak_ptr<Node> Edge::GetSrcNode() const
{
	return srcNode_;
}

std::weak_ptr<Node> Edge::GetDestNode() const
{
	return destNode_;
}

void Edge::SetSrcNode(std::weak_ptr<Node> node)
{
	srcNode_ = std::move(node);
}

void Edge::SetDestNode(std::weak_ptr<Node> node)
{
	destNode_ = std::move(node);
}
