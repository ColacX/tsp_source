#include <vector>
#include <algorithm>
#include <time.h>
#include <assert.h>
#include "Node.h"

#ifdef WIN32
#include "graphic.h"
#endif

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

	int distance(size_t l, size_t r) const
	{
		return distances[l + r * nodes.size()];
	}

	int pathLength(const std::vector<int>& path) const
	{
		return ::pathLength(path, nodes);
	}

	const std::vector<Node> nodes;
private:
	std::vector<int> distances;
};

TSPResult opt2(std::vector<Node> inputpath, std::vector<int> path, clock_t startTime)
{
	Graph graph(inputpath);
	while (true)
	{
#ifndef WIN32
		double diff = double(clock() - startTime) / CLOCKS_PER_SEC;
		if (diff > 1.8)
			break;
#endif
		bool improvementFound = false;
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
				if (oldConnection > newConnection)
				{
					improvementFound = true;
					//Reverse the sequence since an improvement was found
					std::reverse(path.begin() + ii, path.begin() + jj + 1);
#ifdef USE_GRAPHICS
					graphic::draw_path(graph.nodes, path);
#endif
				}
			}
		}
		if (!improvementFound)
		{
			break;
		}
	}
	TSPResult result;
	result.path = path;
	result.length = graph.pathLength(path);
	return result;
}

int connectionDistance(const Graph& graph, const std::vector<int>& path, int a1, int a2, int b1, int b2, int c1, int c2)
{
	return graph.distance(path[a1], path[a2]) + graph.distance(path[b1], path[b2]) + graph.distance(path[c1], path[c2]);
}

void updatePath(std::vector<int>& temp, std::vector<int>& path, int a1, int a2, int b1, int b2, int c1, int c2)
{
	auto tempIter = temp.begin();
	std::copy(path.begin(), path.begin() + a1 + 1, tempIter);
	tempIter += a1 + 1;
	if (a2 < b1)
	{
		std::copy(path.begin() + a2, path.begin() + b1 + 1, tempIter);
		tempIter += b1 - a2 + 1;
	}
	else
	{
		std::reverse_copy(path.begin() + b1, path.begin() + a2 + 1, tempIter);
		tempIter -= b1 - a2 - 1;
	}

	if (b2 < c1)
	{
		std::copy(path.begin() + b2, path.begin() + c1 + 1, tempIter);
		tempIter += c1 - b2 + 1;
	}
	else
	{
		std::reverse_copy(path.begin() + c1, path.begin() + b2 + 1, tempIter);
		tempIter -= c1 - b2 - 1;
	}

	size_t remaining = std::min(std::distance(path.begin() + c2, path.end()), std::distance(tempIter, temp.end()));
	std::copy_n(path.begin() + c2, remaining, tempIter);
	std::swap(temp, path);
}

void testUpdate(const Graph& graph, bool& improvementFound, int& currentDistance, std::vector<int>& temp, std::vector<int>& path, int a1, int a2, int b1, int b2, int c1, int c2)
{
	int newDistance = connectionDistance(graph, path, a1, a2, b1, b2, c1, c2);
	if (newDistance < currentDistance)
	{
		improvementFound = true;
		currentDistance = newDistance;
		updatePath(temp, path, a1, a2, b1, b2, c1, c2);
#ifdef USE_GRAPHICS
		graphic::draw_path(graph.nodes, path);
#endif
	}
}

TSPResult opt3(std::vector<Node> inputpath, std::vector<int> path, clock_t startTime)
{
	Graph graph(inputpath);

	//A vector that can be used for temporary storage when updating the path
	std::vector<int> tempPath(path.size());

	while (true)
	{
		bool improvementFound = false;
		for (int ii = 0; ii < int(path.size()) - 3; ii++)
		{
#ifndef WIN32
			double diff = double(clock() - startTime) / CLOCKS_PER_SEC;
			if (diff > 1.65)
				break;
#endif
			for (int jj = ii + 1; jj < int(path.size()) - 2; jj++)
			{
				for (int kk = jj + 1; kk < int(path.size()) - 1; kk++)
				{
					int oldDistance = connectionDistance(graph, path, ii, ii+1, jj, jj+1, kk, kk+1);

					testUpdate(graph, improvementFound, oldDistance, tempPath, path, ii, jj, ii + 1, jj + 1, kk, kk + 1);
					testUpdate(graph, improvementFound, oldDistance, tempPath, path, ii, jj, ii + 1, kk, jj + 1, kk + 1);
					testUpdate(graph, improvementFound, oldDistance, tempPath, path, ii, jj + 1, kk, ii + 1, jj, kk + 1);
					testUpdate(graph, improvementFound, oldDistance, tempPath, path, ii, jj + 1, kk, jj, ii + 1, kk + 1);
					testUpdate(graph, improvementFound, oldDistance, tempPath, path, ii, kk, jj + 1, jj, ii + 1, kk + 1);
					testUpdate(graph, improvementFound, oldDistance, tempPath, path, ii, kk, jj + 1, ii + 1, jj, kk + 1);
					testUpdate(graph, improvementFound, oldDistance, tempPath, path, ii, ii + 1, jj, kk, jj + 1, kk + 1);
				}
			}
		}
		if (!improvementFound)
		{
			break;
		}
	}
	TSPResult result;
	result.path = path;
	result.length = graph.pathLength(path);
	return result;
}