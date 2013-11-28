#include <vector>
#include <algorithm>
#include <time.h>
#include "Node.h"

class Graph
{
public:
	Graph(std::vector<Node> inputNodes)
		: nodes(std::move(inputNodes))
		, distances(nodes.size() * nodes.size())
	{
		for (size_t ii = 0; ii < nodes.size(); ii++)
		{
			for (size_t jj = 0; jj < nodes.size(); jj++)
			{
				distances[ii + jj * nodes.size()] = ::distance(nodes[ii], nodes[jj]);
			}
		}
	}

	int distance(size_t l, size_t r)
	{
		return distances[l + r * nodes.size()];
	}

	int pathLength(const std::vector<int>& path)
	{
		return ::pathLength(path, nodes);
	}

private:
	const std::vector<Node> nodes;
	std::vector<int> distances;
};

TSPResult opt2(std::vector<Node> inputpath, std::vector<int> path)
{
	fprintf(stderr, "running opt2...\n");
	Graph graph(inputpath);
	clock_t startTime = clock();

	while (true)
	{
		double diff = double(clock() - startTime) / CLOCKS_PER_SEC;
		if (diff > 1.65)
			break;
		
		int bestStart = 0, bestEnd = 0;
		int bestImprovement = 0;
		for (int ii = 0; ii < int(path.size()) - 2; ii++)
		{
			for (int jj = ii + 1; jj < int(path.size()) - 1; jj++)
			{
				//ii is the start of the first sequence, jj the end of it
				int beginA = path[ii];
				int endA = path[jj];
				int beginB = path[jj + 1];
				int endB = ii == 0 ? path.back() : path[ii - 1];

				int oldConnection = graph.distance(endB, beginA) + graph.distance(endA, beginB);
				int newConnection = graph.distance(endB, endA) + graph.distance(beginA, beginB);
				int improvement = oldConnection - newConnection;
				if (improvement > bestImprovement)
				{
					bestStart = ii;
					bestEnd = jj;
					bestImprovement = improvement;
				}
			}
		}
		if (bestImprovement > 0)
		{
			//Reverse the sequence since an improvement was found
			std::reverse(path.begin() + bestStart, path.begin() + bestEnd + 1);
		}
		else
		{
			//No improvement was found
			break;
		}
	}
	TSPResult result;
	result.path = path;
	result.length = graph.pathLength(path);
	return result;
}