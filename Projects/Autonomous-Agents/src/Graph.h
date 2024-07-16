#pragma once

#include "pch.h"

#include "Node.h"
#include "Edge.h"

template<typename T, typename U>
class Graph
{
public:
	Graph();

	std::shared_ptr<T>* FindNodeSharedPtr(T* node);
	
	template<typename V>
	std::weak_ptr<V> CreateNode();

	std::weak_ptr<U> CreateEdge(std::weak_ptr<T> src, std::weak_ptr<T> dest);

	//template<typename V>
	//std::shared_ptr<V> GetNodeOfType();
	
protected:
	//adjacency list
	std::vector<std::shared_ptr<T>> nodes;
	std::vector<std::shared_ptr<U>> edges;

private:
	void AssertIsNode()
	{
		static_assert((std::is_base_of<Node, T>::value && "Type name T is not of type Node"));
	}

	void AssertIsEdge()
	{
		static_assert((std::is_base_of<Edge, U>::value && "Type U is not of type Edge"));
	}
};

template<typename T, typename U>
Graph<T, U>::Graph()
{
	AssertIsNode();
	AssertIsEdge();

	nodes = std::vector<std::shared_ptr<T>>();
	edges = std::vector<std::shared_ptr<U>>();
}

template<typename T, typename U>
std::shared_ptr<T>* Graph<T, U>::FindNodeSharedPtr(T* node)
{
	AssertIsNode();
	return &(*std::find_if(nodes.begin(), nodes.end(),
		[&node](std::shared_ptr<T>& n) -> bool
		{
			return n.get() == node;
		}));
}

template<typename T, typename U>
template<typename V>
inline std::weak_ptr<V> Graph<T, U>::CreateNode()
{
	assert((std::is_base_of<T, V>::value && "V is not a node type"));
	nodes.emplace_back(std::make_shared<V>());
	return std::dynamic_pointer_cast<V>(nodes.back());
}

template<typename T, typename U>
inline std::weak_ptr<U> Graph<T, U>::CreateEdge(std::weak_ptr<T> src, std::weak_ptr<T> dest)
{
	edges.push_back(std::make_shared<U>());
	std::shared_ptr<Edge> newEdge = edges.back();

	newEdge->SetSrcNode(std::static_pointer_cast<Node>(src.lock()));
	newEdge->SetDestNode(std::static_pointer_cast<Node>(dest.lock()));

	//Add edge to node
	src.lock()->AddEdge(newEdge);
	return edges.back();
}

//template<typename T, typename U>
//template<typename V>
//std::shared_ptr<V> Graph<T, U>::GetNodeOfType()
//{
//	assert((std::is_base_of<T, V>::value && "U is not of type T"));
//
//	for (auto& node : nodes)
//	{
//		if (auto n = std::dynamic_pointer_cast<V>(n))
//		{
//			return n;
//		}
//	}
//	return nullptr;
//}
