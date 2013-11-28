#pragma once

namespace tsp_opt2
{
	const int max_nodes = 1024;
	float q_distance[max_nodes][max_nodes];

	float quick_distance(float ax, float ay, float bx, float by)
	{
		float dx = bx - ax;
		float dy = by - ay;
		return dx*dx + dy*dy;
	}

	TSPResult run(const std::vector<Node> nodes, const std::vector<int> initial_path)
	{
		fprintf(stderr, "running tsp_opt2...\n");
		double start_time = clock();
		TSPResult tsp_result;
		int node_count = nodes.size();

		if (node_count >= max_nodes)
		{
			__debugbreak();
			throw "max_nodes exceeded";
		}

		//download inital path to stack memory
		int path_indices[max_nodes];
		int path_temp[max_nodes];

		for (int ia = 0; ia < initial_path.size(); ia++)
			path_indices[ia] = initial_path[ia];

		//calculate the distance between each node
		for (int ia = 0; ia < node_count; ia++)
		{
			for (int ib = 0; ib < node_count; ib++)
			{
				q_distance[ia][ib] = quick_distance(nodes[ia].x, nodes[ia].y, nodes[ib].x, nodes[ib].y);
			}
		}

		while (true)
		{
			double elapsed_time = ((double)clock() - start_time) / CLOCKS_PER_SEC;
			if (elapsed_time > 1.80)
				break;

			int bestStart = 0, bestEnd = 0;
			float bestImprovement = 0;

			for (int ii = 0; ii < node_count - 2; ii++)
			{
				for (int jj = ii + 1; jj < node_count - 1; jj++)
				{
					//ii is the start of the first sequence, jj the end of it
					int beginA = path_indices[ii];
					int endA = path_indices[jj];
					int beginB = path_indices[jj + 1];
					int endB = ii == 0 ? path_indices[node_count - 1] : path_indices[ii - 1];

					float oldConnection = q_distance[endB][beginA] + q_distance[endA][beginB];
					float newConnection = q_distance[endB][endA] + q_distance[beginA][beginB];
					float improvement = oldConnection - newConnection;

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
				//fprintf(stderr, "bestStart:%d bestEnd:%d\n", bestStart, bestEnd);

				//for (int ia = 0; ia < node_count; ia++)
				//	fprintf(stderr, "%d ", path_indices[ia]);
				//fprintf(stderr, "\n");

				//Reverse the sequence since an improvement was found
				for (int ia = 0; ia < (bestEnd - bestStart + 1) / 2; ia++)
				{
					//swap
					int temp = path_indices[bestStart + ia];
					path_indices[bestStart + ia] = path_indices[bestEnd - ia];
					path_indices[bestEnd - ia] = temp;
				}

				//for (int ia = 0; ia < node_count; ia++)
				//	fprintf(stderr, "%d ", path_indices[ia]);
				//fprintf(stderr, "\n");
			}
			else
			{
				//No improvement was found
				break;
			}
		}

		//finalize results
		tsp_result.path.resize(node_count);
		for (int ia = 0; ia < node_count; ia++)
			tsp_result.path[ia] = path_indices[ia];
		tsp_result.length = calculate_cycle_length(nodes, tsp_result.path);
		return tsp_result;
	}

};
