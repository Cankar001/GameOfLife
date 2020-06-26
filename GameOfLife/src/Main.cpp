#include <sstream>

#include <SDL2/SDL.h>
#undef main

#include "Texture.h"
#include "Text.h"

#define ZELLENWIDTH 10
#define ZELLENHEIGHT 10
#define ZELLENABSTAND 2
#define NX (1280 / ZELLENWIDTH)
#define NY (720 / ZELLENHEIGHT)

SDL_Window *window;
SDL_Renderer *renderer;
bool bitmap[NX][NY];
bool running = true;
bool gameRunning = true;
uint32_t gen = 1;
uint32_t waitingTime = 200;

TextData gameStateGenText = { "Current generation: 0", 20 };
TextData gameStatePausedText = { "Game is currently: playing.", 20 };
TextData gameStateRefreshText = { "Game Refresh Rate: 200ms.", 20 };

void CBlock(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)
	{
	// calculate color
	uint32_t r = ((color >> 24) & 0xff) / 255;
	uint32_t g = ((color >> 16) & 0xff) / 255;
	uint32_t b = ((color >> 8) & 0xff) / 255;
	uint32_t a = (color & 0xff) / 255;
	SDL_Rect rect = { x, y, w, h };

	if (r == 1) r = 0xFF;
	else r = 0x00;

	if (g == 1) g = 0xFF;
	else g = 0x00;

	if (b == 1) b = 0xFF;
	else b = 0x00;

	if (a == 1) a = 0xFF;
	else a = 0x00;

	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderFillRect(renderer, &rect);
	}

void draw_field_map()
	{
	for (uint16_t y = 0; y < NY; y++)
		{
		for (uint16_t x = 0; x < NX; x++)
			{
			if (bitmap[x][y] == 1)
				CBlock(x * (ZELLENWIDTH + ZELLENABSTAND), y * (ZELLENHEIGHT + ZELLENABSTAND), ZELLENWIDTH, ZELLENHEIGHT, 0xFF0000FF);
			else
				CBlock(x * (ZELLENWIDTH + ZELLENABSTAND), y * (ZELLENHEIGHT + ZELLENABSTAND), ZELLENWIDTH, ZELLENHEIGHT, 0xFFFFFFFF);
			}
		}
	}

void init()
	{
	// initialize bitmap with random values between 0 and 1
	for (uint16_t y = 0; y < NY; y++)
		{
		for (uint16_t x = 0; x < NX; x++)
			{
			bitmap[x][y] = rand() % 2;
			}
		}

	// draw bitmap once in the beginning
	draw_field_map();
	}

void poll_events()
	{
	SDL_Event e;
	while (SDL_PollEvent(&e))
		{
		if (e.button.button == SDL_BUTTON_LEFT)
			{
			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);
			
			int x = mouseX / (ZELLENWIDTH + ZELLENABSTAND);
			int y = mouseY / (ZELLENHEIGHT + ZELLENABSTAND);

			bitmap[x][y] = 1 - bitmap[x][y];
			draw_field_map();
			}
		else
			{
			switch (e.type)
				{
				case SDL_QUIT:
					running = false;
					break;

				case SDL_KEYDOWN:
					switch (e.key.keysym.scancode)
						{
						case SDL_SCANCODE_ESCAPE:
							running = false;
							break;

						case SDL_SCANCODE_SPACE:
							if (gameRunning)
								{
								gameRunning = false;
								gameStatePausedText.Text = "Game is currently: paused.";
								}
							else
								{
								gameRunning = true;
								gameStatePausedText.Text = "Game is currently: playing.";
								}
							break;

						case SDL_SCANCODE_UP:
							if (waitingTime > 0)
								{
								waitingTime -= 100;
								std::stringstream ss;
								ss << "Game Refresh Rate:" << waitingTime << "ms.";
								gameStateRefreshText.Text = ss.str();
								}
							break;

						case SDL_SCANCODE_DOWN:
							if (waitingTime < 700)
								{
								waitingTime += 100;
								std::stringstream ss;
								ss << "Game Refresh Rate:" << waitingTime << "ms.";
								gameStateRefreshText.Text = ss.str();
								}
							break;
						}
					break;
				}
			}
		}
	}

int16_t get_neighbours(uint16_t x, uint16_t y)
	{
	// Moor Funktion
	// P[(x + 1) % NX][y] -> Rechter Rand
	// P[(x - 1 + NX) % NX][y] -> Linker Rand

	int16_t nachbarn = 0;
	nachbarn += bitmap[(x + 1) % NX][y];
	nachbarn += bitmap[(x - 1 + NX) % NX][y];
	nachbarn += bitmap[x][(y + 1) % NY];
	nachbarn += bitmap[x][(y - 1 + NY) % NY];
	nachbarn += bitmap[(x + 1) % NX][(y + 1) % NY];
	nachbarn += bitmap[(x - 1 + NX) % NX][(y - 1 + NY) % NY];
	nachbarn += bitmap[(x + 1) % NX][(y - 1 + NY) % NY];
	nachbarn += bitmap[(x - 1 + NX) % NX][(y + 1) % NY];
	return nachbarn;
	}

void calc_next_gen()
	{
	bool nextGen[NX][NY];
	for (uint16_t y = 0; y < NY; y++)
		{
		for (uint16_t x = 0; x < NX; x++)
			{
			int16_t neighbours = get_neighbours(x, y);

			switch (neighbours)
				{
				case 0:
				case 1:
					// Zelle tot
					nextGen[x][y] = 0;
					break;

				case 2:
					break;

				case 3:
					// Neue Zelle
					nextGen[x][y] = 1;
					break;

				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					// Zelle tot
					nextGen[x][y] = 0;
					break;
				}
			}
		}

	// Bitmap übertragen
	for (uint16_t y = 0; y < NY; y++)
		{
		for (uint16_t x = 0; x < NX; x++)
			{
			bitmap[x][y] = nextGen[x][y];
			}
		}
	}

int main(int argc, char *argv[])
	{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);

	// Window Setup
	window = SDL_CreateWindow("Game Of Life - by Can Karka", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	
	SDL_Surface *windowIcon = IMG_Load("K:/Programme/GameOfLife/GameOfLife/assets/textures/icon.png");
	SDL_SetWindowIcon(window, windowIcon);

	SDL_Rect bgControls = { 20, 20, 350, 260 };
	SDL_Rect bgStats = { (1280 - 350) - 20, 20, 350, 150 };
	TextData titelData = { "Controls:", 30 };
	TextData statsData = { "Game States:", 30 };
	TextData spaceDescData = { "Play/Pause game.", 20 };
	TextData fastGameSpeedData = { "Increase game speed.", 20 };
	TextData slowGameSpeedData = { "Decrease game speed.", 20 };
	TextData editCellData = { "Enable/Disable Cell manually.", 20 };
	TextData quitGameData = { "Quit game.", 20 };

	// Init gameOfLife
	init();

	// Create static text
	int titelWidth, titelHeight;
	SDL_Texture *titel = create_text(renderer, titelData, &titelWidth, &titelHeight);

	int statsTitelWidth, statsTitelHeight;
	SDL_Texture *stats = create_text(renderer, statsData, &statsTitelWidth, &statsTitelHeight);

	int spaceDescWidth, spaceDescHeight;
	SDL_Texture *spaceDesc = create_text(renderer, spaceDescData, &spaceDescWidth, &spaceDescHeight);

	int fastGameSpeedWidth, fastGameSpeedHeight;
	SDL_Texture *fastGameSpeed = create_text(renderer, fastGameSpeedData, &fastGameSpeedWidth, &fastGameSpeedHeight);

	int slowGameSpeedWidth, slowGameSpeedHeight;
	SDL_Texture *slowGameSpeed = create_text(renderer, slowGameSpeedData, &slowGameSpeedWidth, &slowGameSpeedHeight);

	int editCellWidth, editCellHeight;
	SDL_Texture *editCell = create_text(renderer, editCellData, &editCellWidth, &editCellHeight);

	int quitGameWidth, quitGameHeight;
	SDL_Texture *quitGame = create_text(renderer, quitGameData, &quitGameWidth, &quitGameHeight);

	// Create Textures
	SDL_Texture *arrowUpKeyTexture = create_texture(renderer, "K:/Programme/GameOfLife/GameOfLife/assets/textures/arrow-up.png");
	SDL_Texture *arrowDownKeyTexture = create_texture(renderer, "K:/Programme/GameOfLife/GameOfLife/assets/textures/arrow-down.png");
	SDL_Texture *spaceKeyTexture = create_texture(renderer, "K:/Programme/GameOfLife/GameOfLife/assets/textures/space.png");
	SDL_Texture *escapeKeyTexture = create_texture(renderer, "K:/Programme/GameOfLife/GameOfLife/assets/textures/escape.png");
	SDL_Texture *mouseTexture = create_texture(renderer, "K:/Programme/GameOfLife/GameOfLife/assets/textures/mouse.png");

	// Main Game loop
	while (running)
		{
		// Clear Screen
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);

		if (gameRunning)
			{
			// Calculate next generation
			calc_next_gen();
			gen++;

			// set new generation counter
			std::stringstream ss;
			ss << "Current generation: " << gen;
			gameStateGenText.Text = ss.str();
			}

		// Draw bitmap
		draw_field_map();

		// Draw background for text
		SDL_SetRenderDrawColor(renderer, 0xF0, 0xF0, 0xF0, 0xF0);
		SDL_RenderFillRect(renderer, &bgControls);
		SDL_RenderFillRect(renderer, &bgStats);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderDrawRect(renderer, &bgControls);
		SDL_RenderDrawRect(renderer, &bgStats);

		// Draw text
		draw_text(renderer, titel, 30, 30, titelWidth, titelHeight);
		draw_text(renderer, spaceDesc, 90, 80, spaceDescWidth, spaceDescHeight);
		draw_text(renderer, fastGameSpeed, 90, 120, fastGameSpeedWidth, fastGameSpeedHeight);
		draw_text(renderer, slowGameSpeed, 90, 160, slowGameSpeedWidth, slowGameSpeedHeight);
		draw_text(renderer, editCell, 90, 200, editCellWidth, editCellHeight);
		draw_text(renderer, quitGame, 90, 240, quitGameWidth, quitGameHeight);

		draw_text(renderer, stats, 1280 - 350, 30, statsTitelWidth, statsTitelHeight);
		render_text(renderer, gameStateGenText, 1280 - 350, 70);
		render_text(renderer, gameStatePausedText, 1280 - 350, 100);
		render_text(renderer, gameStateRefreshText, 1280 - 350, 130);

		// draw textures
		draw_texture(renderer, spaceKeyTexture, 20, 60, 64, 64);
		draw_texture(renderer, arrowUpKeyTexture, 35, 115, 32, 32);
		draw_texture(renderer, arrowDownKeyTexture, 35, 155, 32, 32);
		draw_texture(renderer, mouseTexture, 35, 195, 32, 32);
		draw_texture(renderer, escapeKeyTexture, 35, 235, 32, 32);

		// calculate events and swap window buffer
		SDL_RenderPresent(renderer);
		poll_events();
		SDL_GL_SwapWindow(window);
		SDL_Delay(waitingTime);
		}

	// delete textures
	SDL_DestroyTexture(mouseTexture);
	SDL_DestroyTexture(escapeKeyTexture);
	SDL_DestroyTexture(spaceKeyTexture);
	SDL_DestroyTexture(arrowDownKeyTexture);
	SDL_DestroyTexture(arrowUpKeyTexture);

	// delete text
	SDL_DestroyTexture(quitGame);
	SDL_DestroyTexture(editCell);
	SDL_DestroyTexture(slowGameSpeed);
	SDL_DestroyTexture(fastGameSpeed);
	SDL_DestroyTexture(spaceDesc);
	SDL_DestroyTexture(stats);
	SDL_DestroyTexture(titel);

	// delete renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(windowIcon);
	SDL_DestroyWindow(window);
	
	// Shutdown
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
	return 0;
	}
