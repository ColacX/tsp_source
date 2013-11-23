#pragma once
#include <vector>

struct TSPResult
{
	int length;
	std::vector<int> path;
};

struct Node
{
	float x, y;
	int index;
};

bool less_compare(const Node& l, const Node& r);

int distance(const Node& l, const Node& r);

int pathLength(const std::vector<Node>& nodes);
int pathLength(const std::vector<int>& path, const std::vector<Node>& nodes);

std::vector<int> getPathVector(const std::vector<Node>& nodes);

std::vector<Node> randomTest(int testSize);