#pragma once
#include <vector>
#include <stdio.h>
#include <iosfwd>

typedef std::vector<int> Path;

struct TSPResult
{
	int length;
	Path path;
};

struct Node
{
	float x, y;
	int index;
};

bool less_compare(const Node& l, const Node& r);

int distance(const Node& l, const Node& r);

int pathLength(const std::vector<Node>& nodes);
int pathLength(const Path& path, const std::vector<Node>& nodes);

std::vector<int> getPathVector(const std::vector<Node>& nodes);

std::vector<Node> randomTest(int testSize);

std::vector<Node> parseTSPLib(std::istream& input);
std::vector<Node> parseKattisFile(FILE* input);

class Graph
{
public:
	explicit Graph(std::vector<Node> inputNodes);

	int distance(size_t l, size_t r) const
	{
		return distances[l + r * nodes.size()];
	}

	int pathLength(const Path& path) const
	{
		return ::pathLength(path, nodes);
	}

	const std::vector<int>& getNeighbours(size_t i) const
	{
		return nearestNeighbours[i];
	}

	int getMinLink() const
	{
		return minLink;
	}

	const std::vector<Node> nodes;
private:
	std::vector<int> distances;
	std::vector<std::vector<int>> nearestNeighbours;
	int minLink;
};