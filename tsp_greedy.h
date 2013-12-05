#pragma once

namespace tsp_greedy
{
	const int max_nodes = 1024;
	float q_distance[max_nodes][max_nodes];

	inline float quick_distance(float ax, float ay, float bx, float by)
	{
		float dx = bx - ax;
		float dy = by - ay;
		return dx*dx + dy*dy;
	}

	float quick_length(const std::vector<int>& path)
	{
		float length = 0;
		
		for (int ia = 0; ia < path.size() - 1; ia++)
		{
			length += q_distance[path[ia]][path[ia+1]];
		}

		length += q_distance[path.size() - 1][0];
		return length;
	}

	TSPResult run(const std::vector<Node>& nodes)
	{
		fprintf(stderr, "running tsp_greedy...\n");
		double start_time = clock();
		int node_count = nodes.size();
		
		//calculate the distance between each node
		for (int ia = 0; ia < node_count; ia++)
		{
			for (int ib = 0; ib < node_count; ib++)
			{
				q_distance[ia][ib] = quick_distance(nodes[ia].x, nodes[ia].y, nodes[ib].x, nodes[ib].y);
			}
		}

		float best_length = std::numeric_limits<float>::max();
		std::vector<int> best_path;

		for (int ia = 0; ia < 10; ia++)
		{
			std::vector<int> path(node_count);
			std::vector<bool> used(node_count);

			//start the path at a random node
			int start = rand() % node_count;
			path[0] = start;
			used[start] = true;

			//for rest of path
			for (int ii = 1; ii < node_count; ii++)
			{
				float best_distance = std::numeric_limits<float>::max();
				int best_node = -1;

				//for each node
				for (int jj = 0; jj < node_count; jj++)
				{
					float distance = q_distance[path[0]][jj];

					if (!used[jj] && distance < best_distance)
					{
						best_distance = distance;
						best_node = jj;
						//fprintf(stderr, "distance: %f\n", distance);
					}
				}

				//fprintf(stderr, "best_distance: %f\n", best_distance);
				path[ii] = best_node;
				used[best_node] = true;
			}

			float length = calculate_cycle_length(nodes, path);
			fprintf(stderr, "length: %f\n", length);

			if (length < best_length)
			{
				best_length = length;
				best_path = path;
			}
		}

		TSPResult tsp_result;
		tsp_result.length = best_length;
		tsp_result.path = best_path;

		double elapsed_time = ((double)clock() - start_time) / CLOCKS_PER_SEC;
		fprintf(stderr, "final time: %lf\n", elapsed_time);
		return tsp_result;
	}
};
