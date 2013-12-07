#include <stdexcept>
#include <sstream>
#include "graphic.h"

namespace graphic
{

SDL_Window* sdl_window;
SDL_GLContext sdl_gl_context;
TTF_Font* text_font;
float s = 0.01f;
SDL_Color bgra_color = { 0xff, 0xff, 0xff, 0x00 };
void construct()
{
	if (SDL_Init(SDL_INIT_EVENTS) != 0)
		throw std::runtime_error("SDL_Init");

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	sdl_window = SDL_CreateWindow("avalg13_project2_tsp", 10, 30, 1000, 1000, SDL_WINDOW_OPENGL);
	sdl_gl_context = SDL_GL_CreateContext(sdl_window);

	SDL_GL_MakeCurrent(sdl_window, sdl_gl_context);

	if (TTF_Init() == -1)
	{
		__debugbreak();
		throw std::runtime_error("TTF_Init");
	}

	text_font = TTF_OpenFont("consola.ttf", 20);
	if (!text_font)
	{
		__debugbreak();
		throw std::runtime_error("TTF_OpenFont");
	}
}

bool less_index_compare(const Node& l, const Node& r)
{
	return l.index < r.index;
}

void draw_path(const std::vector<Node>& nodes, const std::vector<int>& shortestPath)
{
	if (nodes.empty() || shortestPath.empty())
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw all nodes
	for (size_t ia = 0; ia < nodes.size(); ia++)
	{
		const Node& n = nodes[ia];
		std::stringstream ss;
		ss << n.index;

		SDL_Surface* sdl_surface = TTF_RenderText_Blended(
			text_font, //TTF or OTF text font
			ss.str().c_str(),
			bgra_color //text rgba_color
			);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPixelZoom(1.0f, -1.0f);
		glRasterPos2f(n.x * s - 1, n.y * s - 1);
		glDrawPixels(sdl_surface->w, sdl_surface->h, GL_RGBA, GL_UNSIGNED_BYTE, sdl_surface->pixels);
		glDisable(GL_BLEND);
		SDL_FreeSurface(sdl_surface);
	}

	//draw all edges
	const Node& n_start = nodes[shortestPath[0]];
	const Node* p = &n_start;

	for (size_t ia = 1; ia < shortestPath.size(); ia++)
	{
		const Node& n = nodes[shortestPath[ia]];

		glEnable(GL_COLOR);
		glBegin(GL_LINES);

		glColor4f(1, 0, 0, 1);
		glVertex2f(p->x * s - 1, p->y * s - 1);

		glColor4f(0, 1, 0, 1);
		glVertex2f(n.x * s - 1, n.y * s - 1);

		glEnd();

		p = &n;
	}

	glEnable(GL_COLOR);
	glBegin(GL_LINES);

	glColor4f(0, 0, 1, 1);
	glVertex2f(p->x * s - 1, p->y * s - 1);

	glColor4f(0, 1, 1, 1);
	glVertex2f(n_start.x * s - 1, n_start.y * s - 1);

	glEnd();

	SDL_GL_SwapWindow(sdl_window);
}

void run(const std::vector<Node> nodes, const std::list<TSPResult> tsp_results)
{
	bool program_running = true;
	while (program_running)
	{
		//process sdl events
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				program_running = false;
				break;
			case SDL_KEYDOWN:
				for (int ia = 0; ia < 12 && ia < tsp_results.size(); ia++)
				{
					if (ev.key.keysym.sym == SDLK_F1 + ia)
					{
						auto it = tsp_results.begin();
						std::advance(it, ia);
						draw_path(nodes, it->path);
					}
				}

				switch (ev.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					exit(0);
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			default:
				break;
			}
		}
	}

	SDL_GL_DeleteContext(sdl_gl_context);
	SDL_DestroyWindow(sdl_window);
	TTF_CloseFont(text_font);
	SDL_Quit();
}

}