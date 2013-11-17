#include <iostream>
#include <algorithm>
#include <vector>

#define round(x) x + 0.5f

struct Node
{
	float x, y;
	int index;
};

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
		length += distance(nodes[ii], nodes[ii+1]);
	}
	length += distance(nodes.front(), nodes.back());
	return length;
}

std::vector<int> allPermutations(std::vector<Node>& nodes)
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
	}
	while(std::next_permutation(nodes.begin(), nodes.end(), less_compare));

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

int main(int argc, char* argv[])
{
#ifdef WIN32
	FILE* file = fopen("tsp_source/input0.txt", "r+");
#else
	FILE* file = stdin;
#endif
	size_t numNodes = 0;
	fscanf(file, "%d", &numNodes);
	std::vector<Node> nodes(numNodes);

	for (size_t ii = 0; ii < numNodes; ii++)
	{
		fscanf(file, "%f %f", &nodes[ii].x, &nodes[ii].y);
		nodes[ii].index = ii;
	}

	std::vector<int> shortestPath;

	//if (numNodes <= 11)
	{
		//shortestPath = allPermutations(nodes);
	}
	//else
	{
		shortestPath = greedy(nodes);
	}

	for(int ia=0; ia<shortestPath.size(); ia++)
	{
		std::cout << shortestPath[ia] << "\n";
	}
	std::cout << std::endl;

#ifdef WIN32
	getchar();
#endif
	return 0;
}

