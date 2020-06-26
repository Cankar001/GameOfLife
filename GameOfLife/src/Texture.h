#pragma once

#include <SDL2/SDL.h>
#include <SDL_Image/SDL_image.h>

SDL_Texture *create_texture(SDL_Renderer *renderer, const std::string &path)
	{
	SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
	if (texture == NULL)
		{
		printf("%s", IMG_GetError());
		__debugbreak();
		}

	return texture;
	}

void draw_texture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h)
	{
	SDL_Rect wrapper = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &wrapper);
	}

void render_texture(SDL_Renderer *renderer, const std::string &path, int x, int y, int w, int h)
	{
	SDL_Texture *texture = IMG_LoadTexture(renderer, path.c_str());
	if (texture == NULL)
		{
		printf("%s", IMG_GetError());
		__debugbreak();
		}

	SDL_Rect wrapper = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, NULL, &wrapper);
	SDL_DestroyTexture(texture);
	}