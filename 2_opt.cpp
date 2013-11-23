#include <vector>
#include "Node.h"

TSPResult opt2(std::vector<Node> nodes)
{
	while (true)
	{
		for (int ii = 0; ii < int(nodes.size()) - 2; ii++)
		{
			for (int jj = ii + 1; jj < int(nodes.size()) - 1; jj++)
			{
				//ii is the start of the first sequence, jj the end of it
				int oldConnection;
				int newConnection;
				if (ii == 0)
				{
					oldConnection = distance(nodes.back(), nodes.front()) + distance(nodes[jj], nodes[jj + 1]);
					newConnection = distance(nodes.back(), nodes[jj]) + distance(nodes.front(), nodes[jj + 1]);
				}
				else
				{
					oldConnection = distance(nodes[ii - 1], nodes[ii]) + distance(nodes[jj], nodes[jj + 1]);
					newConnection = distance(nodes[ii - 1], nodes[jj]) + distance(nodes[ii], nodes[jj + 1]);
				}
				if (newConnection < oldConnection)
				{
					//Reverse the sequence since an improvement was found
					int reversals = (jj - ii + 1) / 2;
					for (int kk = 0; kk < reversals; kk++)
					{
						std::swap(nodes[ii + kk], nodes[jj - kk]);
					}
					continue;
				}
			}
		}
		//No improvement was found
		break;
	}
	TSPResult result;
	result.path = getPathVector(nodes);
	result.length = pathLength(nodes);
	return std::move(result);
}