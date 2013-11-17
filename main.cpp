#include <iostream>
#include <algorithm>
#include <vector>

struct Node
{
	float x, y;
	int index;
};

std::istream& operator>>(std::istream& in, Node& node)
{
	return std::cin >> node.x >> node.y;
}

bool operator<(const Node& l, const Node& r)
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

std::vector<int> allPermutations(const std::vector<Node>& nodes)
{
	std::vector<int> path(nodes.size());
	std::sort(nodes.begin(), nodes.end());
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
	} while (std::next_permutation(nodes.begin(), nodes.end()));

	return std::move(path);
}

std::vector<int> greedy(const std::vector<Node>& nodes)
{
	std::vector<int> path(nodes.size());
	std::vector<bool> used(nodes.size());
	path[0] = 0;
	used[0] = true;
	for (size_t ii = 0; ii < nodes.size(); ii++)
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
	size_t numNodes = 0;
	std::cin >> numNodes;
	std::vector<Node> nodes(numNodes);
	for (size_t ii = 0; ii < numNodes; ii++)
	{
		std::cin >> nodes[ii];
		nodes[ii].index = ii;
	}

	std::vector<int> shortestPath;
	if (numNodes <= 11)
	{
		shortestPath = allPermutations(nodes);
	}
	else
	{
		shortestPath = greedy(nodes);
	}
	for (int i : shortestPath)
	{
		std::cout << i << "\n";
	}
	std::cout << std::endl;
	return 0;
}

