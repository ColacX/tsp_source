#pragma once
#include <vector>
#include <list>
#include "Node.h"


#ifdef WIN32
#define USE_GRAPHICS 1

#include <Windows.h>
#undef min
#undef max

#include <gl\GL.h>
#pragma comment(lib, "opengl32.lib")

#include <SDL.h>
#pragma comment(lib, "SDL2.lib")
#undef main

#include <SDL_ttf.h>
#pragma comment(lib, "SDL2_ttf.lib")
#endif

namespace graphic
{
	void construct();

	void draw_path(const std::vector<Node>& nodes, const std::vector<int>& shortestPath);

	void run(const std::vector<Node> nodes, const std::list<TSPResult> tsp_results);
};