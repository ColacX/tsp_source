//put old stuff here
#pragma once

#define round(x) int((x) + 0.5f)

TSPResult allPermutations(std::vector<Node> nodes)
{
	fprintf(stderr, "running allPermutations...\n");
	const std::vector<Node> originalNodes = nodes;
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
	result.length = pathLength(path, originalNodes);
	result.path = std::move(path);
	return result;
}

TSPResult greedy(const std::vector<Node>& nodes)
{
	fprintf(stderr, "running greedy...\n");
	std::vector<int> path(nodes.size());
	std::vector<bool> used(nodes.size());
	path[0] = 0;
	used[0] = true;
	for (size_t ii = 1; ii < nodes.size(); ii++)
	{
		int best = -1;
		for (int jj = 0; jj < int(nodes.size()); jj++)
		{
			const Node& current = nodes[path[ii-1]];
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

TSPResult opt2(std::vector<Node> inputpath, std::vector<int> path, clock_t startTime);
TSPResult opt3(std::vector<Node> inputpath, std::vector<int> path, clock_t startTime);


float euclidian_distance(float ax, float ay, float bx, float by)
{
	float dx = bx - ax;
	float dy = by - ay;
	return float(round(sqrtf(dx*dx + dy*dy)));
}

float calculate_cycle_length(const std::vector<Node> nodes, const std::vector<int> path)
{
	float cycle_distance = 0.0f;

	for (size_t ia = 0; ia < path.size() - 1; ia++)
	{
		cycle_distance += euclidian_distance(nodes[path[ia]].x, nodes[path[ia]].y, nodes[path[ia + 1]].x, nodes[path[ia + 1]].y);
	}

	cycle_distance += euclidian_distance(nodes[path[path.size() - 1]].x, nodes[path[path.size() - 1]].y, nodes[path[0]].x, nodes[path[0]].y);
	return cycle_distance;
}
