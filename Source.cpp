#include "SDL.h"
#undef main
#include <iostream>
#include <vector>
#include <random>

SDL_Renderer* renderer;
SDL_Window* window;
bool isRunning;
bool fullscreen;
std::vector<std::vector<int>> coord; // 0 void, 1 head, -1 apple
int X = 3;
int Y = 0;
int length = 3;
int old_direction = 0;// 0 right, 1 up, 2 left, 3 down
int new_direction = 0;
int score = 0;
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> random100(5, 95);
std::uniform_int_distribution<> random255(0, 255);
std::uniform_int_distribution<> random1(0, 1);
void update();

int main() {
	fullscreen = false;
	int flags = 0;
	flags = SDL_WINDOW_RESIZABLE;
	if (fullscreen) {
		flags = flags | SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems Initialized!\n";

		window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, flags);
		if (window) {
			std::cout << "Window Created!\n";
			SDL_SetWindowMinimumSize(window, 100, 100);
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 121, 121, 121, 255);
			std::cout << "Renderer created!\n";
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			isRunning = true;
		}

	}

	while (isRunning) {
		update();
		if (score < 10 && length) {
			SDL_Delay(43-2*score);
		}
		else {
			SDL_Delay(10);
		}
	}

	//frees memory associated with renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);	//error here
	SDL_Quit();


	return 0;
}

//simple render function
void update() {
	if (coord.size() == 0) {
		for (int y = 0; y < 100; y++) {
			std::vector<int> New;
			for (int x = 0; x < 100; x++) {
				New.push_back(0);
			}
			coord.push_back(New);
		}
		coord[50][50] = -1;
	}
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			if (old_direction != 3) {
				new_direction = 1;
			}
			break;
		case SDLK_DOWN:
			if (old_direction != 1) {
				new_direction = 3;
			}
			break;
		case SDLK_LEFT:
			if (old_direction != 0) {
				new_direction = 2;
			}
			break;
		case SDLK_RIGHT:
			if (old_direction != 2) {
				new_direction = 0;
			}
			break;
		default:;
		}
		break;
	default:
		break;
	}
	old_direction = new_direction;
	if (new_direction == 0) {
		X += 1;
	}
	else if (new_direction == 3) {
		Y += 1;
	}
	else if (new_direction == 2) {
		X -= 1;
	}
	else {
		Y -= 1;
	}
	if (Y >= 0 && Y < 100 && X >= 0 && X < 100 && coord[Y][X] <= 0) {
		if (coord[Y][X] == -1) { // apple
			length+=3;
			score++;
			const char* string_score = "score"+char(score);
			SDL_SetWindowTitle(window, string_score);
			int x = X;
			int y = Y;
			while (coord[y][x] != 0) {
				x = random100(gen);
				y = random100(gen);
			}
			coord[y][x] = -1;

		}
		coord[Y][X] = 1;
	}
	else {
		SDL_Delay(5000);
		isRunning = false;
	}
	for (int y = 0; y < 100; y++) {
		for (int x = 0; x < 100; x++) {
			if (coord[y][x] >= 1) {
				if (coord[y][x] >= length) {
					coord[y][x] = 0;
				}
				else {
					coord[y][x] += 1;
				}
			}
		}
	}
	SDL_RenderClear(renderer);
	for (int y = 0; y < 100; y++) {
		for (int x = 0; x < 100; x++) {
			if (coord[y][x] >= 1) {
				SDL_Rect rect = { 8 * x, 8 * y, 8, 8 }; // x, y, width, height
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
			if (coord[y][x] == -1) {
				SDL_Rect rect = { 8 * x, 8 * y, 8, 8 }; // x, y, width, height
				SDL_SetRenderDrawColor(renderer, random1(gen)*255, random1(gen)*255, random1(gen)*255, 255);
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 255);
	SDL_RenderPresent(renderer);
}
