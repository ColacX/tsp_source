#pragma once
#include <vector>
#include <cmath>
#include <stddef.h>


struct Node
{
	float x, y;
	int index;
};

inline bool less_compare(const Node& l, const Node& r)
{
	if (l.x == r.x)
		return l.y < r.y;
	return l.x < r.x;
}

inline int distance(const Node& l, const Node& r)
{
	return round(std::sqrt(l.x * r.x + l.y * r.y));
}

inline int pathLength(const std::vector<Node>& nodes)
{
	int length = 0;
	for (size_t ii = 0; ii < nodes.size() - 1; ii++)
	{
		length += distance(nodes[ii], nodes[ii + 1]);
	}
	length += distance(nodes.front(), nodes.back());
	return length;
}

inline std::vector<int> getPathVector(const std::vector<Node>& nodes)
{
	std::vector<int> path(nodes.size());
	for (size_t ii = 0; ii < path.size(); ii++)
	{
		path[ii] = nodes[ii].index;
	}
	return std::move(path);
}