#include <iostream>
#include <algorithm>
#include <vector>

#ifdef WIN32
#include <glut.h>
#include <Windows.h>
#undef min
#undef max
#endif

#define round(x) x + 0.5f

struct Node
{
	float x, y;
	int index;
};

bool less_compare(const Node& l, const Node& r)
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

std::vector<int> allPermutations(std::vector<Node>& nodes)
{
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
	}
	while(std::next_permutation(nodes.begin(), nodes.end(), less_compare));

	return std::move(path);
}

std::vector<int> greedy(const std::vector<Node>& nodes)
{
	std::vector<int> path(nodes.size());
	std::vector<bool> used(nodes.size());
	path[0] = 0;
	used[0] = true;
	for (size_t ii = 1; ii < nodes.size(); ii++)
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

void display_function()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void idle_function()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_COLOR);
	glBegin(GL_LINES);
	
	float scale = 1.0f;
	glColor4f(1, 0, 0, 1);
	glVertex2f(0, 0);

	glColor4f(0, 1, 0, 1);
	glVertex2f(scale, scale);
	
	glEnd();

	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
#ifdef WIN32
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA);
	glutInitWindowSize(1000, 1000);

	glutCreateWindow("avalg13_project2_tsp");
	glutDisplayFunc(display_function);
	glutIdleFunc(idle_function);
	//glutReshapeFunc();
	//glutKeyboardFunc();
	glutMainLoop();

	FILE* file = fopen("tsp_source/input0.txt", "r+");
#else
	FILE* file = stdin;
#endif
	size_t numNodes = 0;
	fscanf(file, "%d", &numNodes);
	std::vector<Node> nodes(numNodes);

	for (size_t ii = 0; ii < numNodes; ii++)
	{
		fscanf(file, "%f %f", &nodes[ii].x, &nodes[ii].y);
		nodes[ii].index = ii;
	}

	std::vector<int> shortestPath;

	//if(numNodes <= 11)
	//{
		shortestPath = allPermutations(nodes);
	//}
	//else
	{
		shortestPath = greedy(nodes);
	}

	for(int ia=0; ia<shortestPath.size(); ia++)
	{
		std::cout << shortestPath[ia] << "\n";
	}
	std::cout << std::endl;

#ifdef WIN32
	fprintf(stderr, "main: end\n");
	getchar();
#endif
	return 0;
}

