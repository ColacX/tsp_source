#include <vector>
#include <algorithm>
#include <time.h>
#include "Node.h"

class Graph
{
public:
	Graph(std::vector<Node> nodes)
		: nodes(std::move(nodes))
	{}

	int distance(size_t l, size_t r)
	{
		return ::distance(nodes[l], nodes[r]);
	}

	int pathLength(const std::vector<int>& path)
	{
		return ::pathLength(path, nodes);
	}

private:
	const std::vector<Node> nodes;
};

TSPResult opt2(std::vector<Node> inputNodes, std::vector<int> nodes)
{
	Graph graph(inputNodes);
	clock_t startTime = clock();
	while (true)
	{
		double diff = double(clock() - startTime) / CLOCKS_PER_SEC;
		if (diff > 1.65)
			break;
		
		int bestStart = 0, bestEnd = 0;
		int bestImprovement = 0;
		for (int ii = 0; ii < int(nodes.size()) - 2; ii++)
		{
			for (int jj = ii + 1; jj < int(nodes.size()) - 1; jj++)
			{
				//ii is the start of the first sequence, jj the end of it
				int beginA = nodes[ii];
				int endA = nodes[jj];
				int beginB = nodes[jj + 1];
				int endB = ii == 0 ? nodes.back() : nodes[ii - 1];

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
			std::reverse(nodes.begin() + bestStart, nodes.begin() + bestEnd + 1);
		}
		else
		{
			//No improvement was found
			break;
		}
	}
	TSPResult result;
	result.path = nodes;
	result.length = graph.pathLength(nodes);
	return result;
}