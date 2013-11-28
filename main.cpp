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

#include "library.h"

int main(int argc, char* argv[])
{
#ifdef WIN32
	bool showGraphics = true;
	if (showGraphics)
	{
		graphic::construct();
	}
	FILE* file = fopen("input0.txt", "r+");
#else
	FILE* file = stdin;
#endif
	const std::vector<Node> nodes = parseKattisFile(file);
	std::vector<TSPResult> tsp_results;
	
	TSPResult greedyResult = greedy(nodes);
	tsp_results.push_back(greedyResult);
	tsp_results.push_back(opt2(nodes, greedyResult.path));
	tsp_results.push_back(allPermutations(nodes));

	for (TSPResult& tsp_result : tsp_results)
	{
		float cycle_length = calculate_cycle_length(nodes, tsp_result.path);
		printf("cycle_length %f\n", cycle_length);
		std::cerr << "Length:" << tsp_result.length << std::endl;
		for (int index : tsp_result.path)
		{
			std::cout << index << " ";
		}
		std::cout << std::endl;

		graphic::draw_path(nodes, tsp_result.path);
	}

#ifdef WIN32
	if (showGraphics)
	{
		graphic::run(nodes, tsp_results);
	}
	fprintf(stderr, "main: end\n");
#endif
	return 0;
}
