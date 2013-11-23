#include <vector>
#include <algorithm>
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
				Node& beginA = nodes[ii];
				Node& endA = nodes[jj];
				Node& beginB = nodes[jj + 1];
				Node& endB = ii == 0 ? nodes.back() : nodes[ii - 1];
				int oldConnection = distance(endB, beginA) + distance(endA, beginB);
				int newConnection = distance(endB, endA) + distance(beginA, beginB);
				
				if (newConnection < oldConnection)
				{
					//Reverse the sequence since an improvement was found
					std::reverse(nodes.begin() + ii, nodes.begin() + jj + 1);
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