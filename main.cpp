#include <iostream>
#include <algorithm>
#include <vector>

#ifdef WIN32
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

#define round(x) x + 0.5f

TTF_Font* text_font;
SDL_Color rgba_color = {0xff, 0x00, 0x00, 0x00};

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

int main(int argc, char* argv[])
{
#ifdef WIN32
	if(SDL_Init(SDL_INIT_EVENTS) != 0)
		throw "SDL_Init";

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_Window* sdl_window;
	sdl_window = SDL_CreateWindow("", 10, 20, 1000, 1000, SDL_WINDOW_OPENGL);

	SDL_GLContext sdl_gl_context;
	sdl_gl_context = SDL_GL_CreateContext(sdl_window);
	SDL_GL_MakeCurrent(sdl_window, sdl_gl_context);

	if(TTF_Init() == -1)
		throw "TTF_Init";

	TTF_Font* text_font = TTF_OpenFont("consola.ttf", 72);

	bool program_running = true;
	while(program_running)
	{
		//process sdl events
		SDL_Event ev;
		while(SDL_PollEvent(&ev))
		{
			switch(ev.type)
			{
			case SDL_QUIT:
				program_running = false;
				break;
			case SDL_KEYDOWN:
				switch(ev.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					exit(0);
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				fprintf(stderr, "mouse down x:%d y:%d\n", ev.button.x, ev.button.y);
				break;
			default:
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
		glEnable(GL_COLOR);
		glBegin(GL_LINES);
	
		float s = 0.1f; //scale
		glColor4f(1, 0, 0, 1);
		glVertex2f(0, 0);

		glColor4f(0, 1, 0, 1);
		glVertex2f(s, s);
	
		glEnd();

		glScalef(1, 1, 1);

		SDL_Surface* sdl_surface = TTF_RenderText_Blended(
			text_font, //TTF or OTF text font
			"hello",
			rgba_color //text rgba_color
		);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPixelZoom(1.0f, -1.0f);
		glRasterPos2f(0, 0);
		glDrawPixels(sdl_surface->w, sdl_surface->h, GL_RGBA, GL_UNSIGNED_BYTE, sdl_surface->pixels);

		SDL_GL_SwapWindow(sdl_window);
		glDisable(GL_BLEND);
		SDL_FreeSurface(sdl_surface);
	}

	//destruct
	{
		SDL_GL_DeleteContext(sdl_gl_context);
		SDL_DestroyWindow(sdl_window);
		SDL_Quit();
	}

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

