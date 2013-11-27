#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <time.h>

#include "Node.h"
#ifdef WIN32
#include "graphic.h"
#endif


#define round(x) int((x) + 0.5f)


TSPResult allPermutations(std::vector<Node> nodes)
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

	TSPResult result;
	result.length = pathLength(path, nodes);
	result.path = std::move(path);
	return result;
}

TSPResult greedy(const std::vector<Node>& nodes)
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
	TSPResult result;
	result.length = pathLength(path, nodes);
	result.path = std::move(path);
	return result;
}

TSPResult opt2(std::vector<Node> nodes);


int main(int argc, char* argv[])
{
#ifdef WIN32
	bool showGraphics = true;
	if (showGraphics)
	{
		graphic::construct();
	}
	FILE* file = fopen("input0.txt", "r+");
	std::ifstream f("att48.tsp");
#else
	FILE* file = stdin;
#endif
	std::vector<Node> nodes = parseKattisFile(file);
	TSPResult greedyResult = greedy(nodes);
	std::vector<Node> temp = nodes;
	for (size_t ii = 0; ii < temp.size(); ii++)
	{
		nodes[ii] = temp[greedyResult.path[ii]];
	}

	TSPResult result;
	/*
	if (numNodes <= 11)
	{
		result = allPermutations(nodes);
	}
	else
	*/
	{
		result = opt2(nodes);
	}
	if (greedyResult.length < result.length)
	{
		result = greedyResult;
	}
	std::cerr << "Length:" << result.length << std::endl;
	for (int index : result.path)
	{
		std::cout << index << "\n";
	}
	std::cout << std::endl;

#ifdef WIN32
	if (showGraphics)
	{
		graphic::draw_path(nodes, result.path);
		graphic::run(nodes, result.path);
	}
	fprintf(stderr, "main: end\n");
#endif
	return 0;
}
