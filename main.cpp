#include <iostream>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <time.h>
#include <list>
#include <unordered_map>

#include "Node.h"
#ifdef WIN32
#include "graphic.h"
#else
#define __debugbreak() //NOP
#endif

#include "library.h"
#include "tsp_opt2.h"
#include "tsp_approx_greedy_opt2.h"

double start_time;

void assert_cycle(const std::vector<Node> nodes, TSPResult& tsp_result)
{
	std::vector<bool> visited(nodes.size());

	for (size_t ia = 0; ia < tsp_result.path.size(); ia++)
		visited[tsp_result.path[ia]] = true;

	for (size_t ia = 0; ia < visited.size(); ia++)
	{
		if (!visited[ia])
		{
			__debugbreak();
			throw "not a cycle";
		}
	}
}

void benchmark_start()
{
	start_time = clock();
}

void benchmark_stop(const std::vector<Node> nodes, TSPResult& tsp_result)
{
	double elapsed_time = ((double)clock() - start_time) / CLOCKS_PER_SEC;
	fprintf(stderr, "elapsed_time: %lf\n", elapsed_time);

	assert_cycle(nodes, tsp_result);

	float cycle_length = calculate_cycle_length(nodes, tsp_result.path);
	printf("cycle_length %f\n", cycle_length);
	std::cerr << "Length:" << tsp_result.length << std::endl;
	//for (int index : tsp_result.path)
	//	std::cout << index << " ";

	std::cout << std::endl << std::endl;
}

std::vector<Node> random_nodes()
{
	srand(1337);
	std::vector<Node> nodes(1000);

	for (size_t ia = 0; ia < nodes.size(); ia++)
	{
		nodes[ia].x = 100.0f * (float)rand() / RAND_MAX;
		nodes[ia].y = 100.0f * (float)rand() / RAND_MAX;
		nodes[ia].index = ia;
		//fprintf(stderr, "%d %f %f\n", ia, nodes[ia].x, nodes[ia].y);
	}

	return nodes;
}

#define KATTIS

int main(int argc, char* argv[])
{
#if defined(WIN32) && !defined(KATTIS) 
	graphic::construct();
	FILE* file = fopen("input0.txt", "r+");

	//const std::vector<Node> nodes = parseKattisFile(file);
	const std::vector<Node> nodes = random_nodes();
	std::list<TSPResult> tsp_results;

	benchmark_start();
	tsp_results.push_back(greedy(nodes));
	benchmark_stop(nodes, tsp_results.back());
	const TSPResult& greedyResult = tsp_results.back();

	benchmark_start();
	tsp_results.push_back(tsp_opt2::run(nodes, greedyResult.path));
	benchmark_stop(nodes, tsp_results.back());

	benchmark_start();
	tsp_results.push_back(opt2(nodes, greedyResult.path));
	benchmark_stop(nodes, tsp_results.back());

	//benchmark_start();
	//tsp_results.push_back(allPermutations(nodes));
	//benchmark_stop(nodes, tsp_results.back());

	benchmark_start();
	tsp_results.push_back(tsp_approx_greedy_opt2::run(nodes));
	benchmark_stop(nodes, tsp_results.back());

	graphic::run(nodes, tsp_results);
	fprintf(stderr, "main: end\n");
#else
	clock_t startTime = clock();
	//FILE* file = fopen("input0.txt", "r+");
	//std::fstream f("a280.tsp");
	FILE* file = stdin;
	const std::vector<Node> nodes = parseKattisFile(file);
	
	TSPResult tsp_result;
	if (nodes.size() < 13)
	{
		tsp_result = allPermutations(nodes);
	}
	else
	{
		TSPResult greedyResult = greedy(nodes);
		tsp_result = opt2(nodes, greedyResult.path, startTime);
		TSPResult tsp_result2 = opt3(nodes, tsp_result.path, startTime);
		TSPResult tsp_result3 = opt2(nodes, tsp_result2.path, startTime);

		if (tsp_result2.length < tsp_result.length)
		{
			std::swap(tsp_result, tsp_result2);
		}
		if (tsp_result3.length < tsp_result.length)
		{
			std::swap(tsp_result, tsp_result3);
		}
	}

	for (int index : tsp_result.path)
		std::cout << index << "\n";
#endif
	return 0;
}
