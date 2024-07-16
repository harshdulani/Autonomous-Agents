#pragma once
#include "pch.h"


class Edge
{
public:
	virtual ~Edge() = default;
	
	std::weak_ptr<class Node> GetSrcNode() const;
	std::weak_ptr<class Node> GetDestNode() const;

	void SetSrcNode(std::weak_ptr<class Node> node);
	void SetDestNode(std::weak_ptr<class Node> node);

private:
	std::weak_ptr<class Node> srcNode_;
	std::weak_ptr<class Node> destNode_;
};