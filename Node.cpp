#include "Node.h"
#include <vector>
#include <cmath>
#include <stddef.h>
#include <random>
#include <string>
#include <assert.h>

bool less_compare(const Node& l, const Node& r)
{
	if (l.x == r.x)
		return l.y < r.y;
	return l.x < r.x;
}

int distance(const Node& l, const Node& r)
{
	return int(round(std::sqrt(l.x * r.x + l.y * r.y)));
}

int pathLength(const std::vector<Node>& nodes)
{
	int length = 0;
	for (size_t ii = 0; ii < nodes.size() - 1; ii++)
	{
		length += distance(nodes[ii], nodes[ii + 1]);
	}
	length += distance(nodes.front(), nodes.back());
	return length;
}
int pathLength(const std::vector<int>& path, const std::vector<Node>& nodes)
{
	int length = 0;
	for (size_t ii = 0; ii < nodes.size() - 1; ii++)
	{
		length += distance(nodes[path[ii]], nodes[path[ii + 1]]);
	}
	length += distance(nodes[path.front()], nodes[path.back()]);
	return length;
}

std::vector<int> getPathVector(const std::vector<Node>& nodes)
{
	std::vector<int> path(nodes.size());
	for (size_t ii = 0; ii < path.size(); ii++)
	{
		path[ii] = nodes[ii].index;
	}
	return std::move(path);
}


std::vector<Node> randomTest(int testSize)
{
	std::vector<Node> nodes(testSize);
	for (size_t ii = 0; ii < nodes.size(); ii++)
	{
		nodes[ii].x = float(rand() % 10000);
		nodes[ii].y = float(rand() % 10000);
		nodes[ii].index = ii;
	}
	return std::move(nodes);
}

std::vector<Node> parseTSPLib(std::istream& input)
{
	std::string dump;
	std::getline(input, dump);//NAME
	std::getline(input, dump);//COMMENT
	std::getline(input, dump);//TYPE
	size_t dimension = 0;
	input >> dump >> dump >> dimension >> dump;//DIMENSION
	std::getline(input, dump);//EDGE_WEIGHT_TYPE
	std::getline(input, dump);//NODE_COORD_SECTIOn
	std::vector<Node> nodes(dimension);
	for (size_t ii = 0; ii < dimension; ii++)
	{
		size_t t;
		if (!(input >> t >> nodes[ii].x >> nodes[ii].y))
			throw std::runtime_error("Failed to parse coordinates");
		assert(t == ii + 1);
		nodes[ii].index = ii;
	}
	return std::move(nodes);
}

std::vector<Node> parseKattisFile(FILE* file)
{
	size_t numNodes = 0;
	fscanf(file, "%d", &numNodes);
	std::vector<Node> nodes(numNodes);

	for (size_t ii = 0; ii < numNodes; ii++)
	{
		fscanf(file, "%f %f", &nodes[ii].x, &nodes[ii].y);
		nodes[ii].index = ii;
	}
	return std::move(nodes);
}
