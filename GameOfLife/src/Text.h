#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2_TTF/SDL_ttf.h>

struct TextData
	{
	std::string Text;
	std::string FontPath = "assets/fonts/arial.ttf";
	int FontSize = 24;
	Uint8 Red = 0;
	Uint8 Green = 0;
	Uint8 Blue = 0;
	Uint8 Alpha = 255;

	TextData(const std::string &text, int fontSize = 24, Uint8 red = 0, Uint8 green = 0, Uint8 blue = 0, Uint8 alpha = 255)
		: Text(text), FontSize(fontSize), Red(red), Green(green), Blue(blue), Alpha(alpha)
		{
		}
	};

SDL_Texture *create_text(SDL_Renderer *renderer, const TextData &data, int *w, int *h)
	{
	TTF_Font *font = TTF_OpenFont(data.FontPath.c_str(), data.FontSize);
	if (font == NULL)
		{
		printf("%s", TTF_GetError());
		__debugbreak();
		}

	SDL_Color color = { data.Red, data.Green, data.Blue, data.Alpha };
	SDL_Surface *surface = TTF_RenderText_Solid(font, data.Text.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	*w = surface->w;
	*h = surface->h;

	TTF_CloseFont(font);
	SDL_FreeSurface(surface);
	return texture;
	}

void draw_text(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h)
	{
	SDL_Rect wrapper = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &wrapper);
	}

void render_text(SDL_Renderer *renderer, const TextData &data, int x, int y)
	{
	TTF_Font *font = TTF_OpenFont(data.FontPath.c_str(), data.FontSize);
	if (font == NULL)
		{
		printf("%s", TTF_GetError());
		__debugbreak();
		}

	SDL_Color color = { data.Red, data.Green, data.Blue, data.Alpha };
	SDL_Surface* surface = TTF_RenderText_Solid(font, data.Text.c_str(), color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect wrapper = { x, y, surface->w, surface->h };
	SDL_RenderCopy(renderer, texture, NULL, &wrapper);

	TTF_CloseFont(font);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	}