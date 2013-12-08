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

	TSPResult run(const std::vector<Node> nodes, const std::vector<int> greedy_path)
	{
		fprintf(stderr, "running tsp_opt2...\n");
		double start_time = clock();
		TSPResult tsp_result;
		int node_count = nodes.size();
		srand(1337);

		if (node_count >= max_nodes)
		{
			__debugbreak();
			throw "max_nodes exceeded";
		}

		//initial path
		Path path_indices(node_count);
		for (int ia = 0; ia < node_count; ia++)
			path_indices[ia] = greedy_path[ia];

		//calculate the distance between each node
		for (int ia = 0; ia < node_count; ia++)
		{
			for (int ib = 0; ib < node_count; ib++)
			{
				q_distance[ia][ib] = quick_distance(nodes[ia].x, nodes[ia].y, nodes[ib].x, nodes[ib].y);
			}
		}

		const double randomize_max = 1;
		float best_length = std::numeric_limits<float>::max();
		Path best_path;

		for (int ia = 0; ia < randomize_max; ia++)
		{
			while (true)
			{
				double elapsed_time = ((double)clock() - start_time) / CLOCKS_PER_SEC;
				if (elapsed_time > 1.80 / randomize_max * (ia+1))
				{
					fprintf(stderr, "break time: %lf\n", elapsed_time);
					break;
				}

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

							//Reverse the sequence since an improvement was found
							for (int ia = 0; ia < (bestEnd - bestStart + 1) / 2; ia++)
							{
								//swap
								int temp = path_indices[bestStart + ia];
								path_indices[bestStart + ia] = path_indices[bestEnd - ia];
								path_indices[bestEnd - ia] = temp;
							}
						}
					}
				}

				if (bestImprovement > 0)
				{
					
				}
				else
				{
					//No improvement was found
					break;
				}
			}

			float length = calculate_cycle_length(nodes, path_indices);
			fprintf(stderr, "length: %f\n", length);

			if (length < best_length)
			{
				best_length = length;
				best_path = path_indices;
			}

			//randomize some parts of the initial path
			for (int ib = 0; ib < 10; ib++)
			{
				std::swap(path_indices[std::rand() % node_count], path_indices[std::rand() % node_count]);
			}
		}

		//finalize results
		tsp_result.path.resize(node_count);
		for (int ia = 0; ia < node_count; ia++)
			tsp_result.path[ia] = best_path[ia];
		tsp_result.length = int(best_length);

		double elapsed_time = ((double)clock() - start_time) / CLOCKS_PER_SEC;
		fprintf(stderr, "final time: %lf\n", elapsed_time);
		return tsp_result;
	}

};
