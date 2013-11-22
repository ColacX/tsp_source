#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <sstream>

#include "Node.h"
#ifdef WIN32
#include "graphic.h"
#endif


#define round(x) int((x) + 0.5f)


std::vector<int> allPermutations(std::vector<Node> nodes)
{
	std::vector<int> path(nodes.size());
	std::sort(nodes.begin(), nodes.end(), less_compare);
	int minLength = std::numeric_limits<int>::max();
	int permutation = 0;
	do
	{
		int length = pathLength(nodes);
		if (length < minLength)
		{
			minLength = length;
			for (size_t ii = 0; ii < nodes.size(); ii++)
			{
				path[ii] = nodes[ii].index;
			}
		}
		permutation++;
	} while (std::next_permutation(nodes.begin(), nodes.end(), less_compare));

	return std::move(path);
}

std::vector<int> greedy(const std::vector<Node>& nodes)
{
	std::vector<int> path(nodes.size());
	std::vector<bool> used(nodes.size());
	path[0] = 0;
	used[0] = true;
	for (size_t ii = 1; ii < nodes.size(); ii++)
	{
		int best = -1;
		for (int jj = 0; jj < int(nodes.size()); jj++)
		{
			const Node& current = nodes[path[ii]];
			if (!used[jj] && (best == -1 || distance(current, nodes[jj]) < distance(current, nodes[best])))
				best = jj;
		}

		path[ii] = best;
		used[best] = true;
	}
	return std::move(path);
}

std::vector<int> opt2(std::vector<Node> nodes);

std::vector<Node> nodes;
std::vector<int> shortestPath;

int main(int argc, char* argv[])
{
#ifdef WIN32
	bool showGraphics = false;
	if (showGraphics)
	{
		graphic::construct();
	}
	FILE* file = fopen("input0.txt", "r+");
#else
	FILE* file = stdin;
#endif
	size_t numNodes = 0;
	fscanf(file, "%d", &numNodes);
	nodes.resize(numNodes);

	for (size_t ii = 0; ii < numNodes; ii++)
	{
		fscanf(file, "%f %f", &nodes[ii].x, &nodes[ii].y);
		nodes[ii].index = ii;
	}

	/*
	if (numNodes <= 11)
	{
		shortestPath = allPermutations(nodes);
	}
	else
	*/
	{
		shortestPath = opt2(nodes);
	}
	std::vector<Node> path(nodes.size());
	for (size_t ii = 0; ii < path.size(); ii++)
	{
		path[ii] = nodes[shortestPath[ii]];
	}
	std::cerr << "Length:" << pathLength(path) << std::endl;
	for (int& index : shortestPath)
	{
		std::cout << index << "\n";
	}
	std::cout << std::endl;

#ifdef WIN32
	if (showGraphics)
	{
		graphic::draw_path(nodes, shortestPath);
		graphic::run(nodes, shortestPath);
	}
	fprintf(stderr, "main: end\n");
#endif
	return 0;
}
