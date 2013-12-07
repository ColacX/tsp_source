#pragma once

namespace tsp_approx_greedy_opt2
{
	const int max_nodes = 1024;
	float q_distance[max_nodes][max_nodes];
	const int max_cells_side = 128;
	std::vector<int> cells[max_cells_side][max_cells_side];

	inline float quick_distance(float ax, float ay, float bx, float by)
	{
		float dx = bx - ax;
		float dy = by - ay;
		return dx*dx + dy*dy;
	}

	float quick_length(const std::vector<int>& path)
	{
		float length = 0;
		
		for (size_t ia = 0; ia < path.size() - 1; ia++)
		{
			length += q_distance[path[ia]][path[ia+1]];
		}

		length += q_distance[path.size() - 1][0];
		return length;
	}

	TSPResult run(const std::vector<Node>& nodes)
	{
		fprintf(stderr, "running tsp_approx_greedy_opt2...\n");
		double start_time = clock();
		int node_count = nodes.size();

		//foreach node sort the nodes by their y value
		//std::sort();
		//split the nodes into even y_regions
		//sort each y_region by their x values
		//split each y_region into x_regions

		int best_cell_size = 0;
		const float tweak = 0.00001f;

		//foreach node calculate the distance between each node
		for (int ia = 0; ia < node_count; ia++)
		{
			////foreach node put the node into the closest bucket
			//int cells_x = (int)(nodes[ia].x * tweak);
			//int cells_y = (int)(nodes[ia].y * tweak);
			//cells[cells_x][cells_y].push_back(ia);
			//int cell_size = cells[cells_x][cells_y].size();
			////printf("cells: %d %d %d\n", cells_x, cells_y, cell_size);
			//
			//if (best_cell_size < cell_size)
			//	best_cell_size = cell_size;

			for (int ib = 0; ib < node_count; ib++)
			{
				q_distance[ia][ib] = quick_distance(nodes[ia].x, nodes[ia].y, nodes[ib].x, nodes[ib].y);
			}
		}

		fprintf(stderr, "best cell size: %d\n", best_cell_size);
		float best_length = std::numeric_limits<float>::max();
		std::vector<int> best_path;

		while (true)
		{
			if (1.80 < ((double)clock() - start_time) / CLOCKS_PER_SEC)
			{
				fprintf(stderr, "time limit reached\n");
				break;
			}

			//greedy part
			std::vector<int> path(node_count);
			//std::vector<bool> used(node_count);

			////start the path at a random node
			//int start = rand() % node_count;
			//path[0] = start;
			//used[start] = true;

			////for rest of path
			//for (int ii = 1; ii < node_count; ii++)
			//{
			//	//{
			//	//	int cells_x = (int)(nodes[ii - 1].x * tweak);
			//	//	int cells_y = (int)(nodes[ii - 1].y * tweak);

			//	//	std::vector<int>& cell = cells[cells_x][cells_y];
			//	//	//printf("cells: %d %d %d\n", cells_x, cells_y, cell.size());

			//	//	for (int ib = 0; ib < cell.size(); ib++)
			//	//	{
			//	//		int node_index = cell[ib];

			//	//		if (!used[node_index])
			//	//		{
			//	//			//grab the first available in the same cell, this is an approximation
			//	//			used[node_index] = true;
			//	//			path[ii] = node_index;
			//	//			goto continue_building_path;
			//	//		}
			//	//	}
			//	//}

			//	{
			//		//otherwize default to searching for the best
			//		float best_distance = std::numeric_limits<float>::max();
			//		int best_node = -1;

			//		//for each node
			//		for (int jj = 0; jj < node_count; jj++)
			//		{
			//			if (!used[jj] && q_distance[path[0]][jj] < best_distance)
			//			{
			//				best_distance = q_distance[path[0]][jj];
			//				best_node = jj;
			//				//fprintf(stderr, "distance: %f\n", distance);
			//			}
			//		}

			//		//fprintf(stderr, "best_distance: %f\n", best_distance);
			//		path[ii] = best_node;
			//		used[best_node] = true;
			//	}

			//	continue_building_path:;
			//}

			for (int ib = 0; ib < node_count; ib++)
			{
				path[ib] = ib;
			}

			std::random_shuffle(path.begin(), path.end());

			////randomize some parts of the path
			//for (int ib = 0; ib < node_count; ib++)
			//{
			//	std::swap(path[std::rand() % node_count], path[std::rand() % node_count]);
			//}

			float length;
			length = quick_length(path);
			//fprintf(stderr, "greedy quick length: %f\n", length);

			{
				//opt2 part
				while (true)
				{
					if (1.80 < ((double)clock() - start_time) / CLOCKS_PER_SEC)
					{
						fprintf(stderr, "time limit reached\n");
						break;
					}

					int bestStart = 0, bestEnd = 0;
					float bestImprovement = 0;

					for (int ii = 0; ii < node_count - 2; ii++)
					{
						for (int jj = ii + 1; jj < node_count - 1; jj++)
						{
							//ii is the start of the first sequence, jj the end of it
							int beginA = path[ii];
							int endA = path[jj];
							int beginB = path[jj + 1];
							int endB = ii == 0 ? path[node_count - 1] : path[ii - 1];

							float oldConnection = q_distance[endB][beginA] + q_distance[endA][beginB];
							float newConnection = q_distance[endB][endA] + q_distance[beginA][beginB];
							float improvement = oldConnection - newConnection;

							if (improvement > 0)
							{
								bestStart = ii;
								bestEnd = jj;
								bestImprovement = improvement;

								//Reverse the sequence since an improvement was found
								for (int ia = 0; ia < (bestEnd - bestStart + 1) / 2; ia++)
								{
									//swap
									int temp = path[bestStart + ia];
									path[bestStart + ia] = path[bestEnd - ia];
									path[bestEnd - ia] = temp;
								}
							}
						}
					}

					if (bestImprovement == 0)
					{
						//No improvement was found
						break;
					}
				}

				length = quick_length(path);
				fprintf(stderr, "opt2 quick length: %f\n", length);

				if (length < best_length)
				{
					best_length = length;
					best_path = path;
				}
			}
		}

		TSPResult tsp_result;
		tsp_result.length = int(best_length);
		tsp_result.path = best_path;

		double elapsed_time = ((double)clock() - start_time) / CLOCKS_PER_SEC;
		fprintf(stderr, "final time: %lf\n", elapsed_time);
		return tsp_result;
	}
};
