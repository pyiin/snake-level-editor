#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "renderwindow.h"

#define FPS 60
#define SIZEX 30
#define SIZEY 20

RenderWindow window;
int level[SIZEX*SIZEY];
std::vector<SDL_Texture*> tileSprites;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 480;
enum{menu, game, end} loop;
bool gameRunning;
Grid grid;
std::vector<SDL_Texture*> title;

void fileWrite(){
	auto fp = fopen("level.bin", "w");
	int sizes[] = {SIZEX, SIZEY};
	fwrite(sizes, sizeof(sizes[0]), sizeof(sizes)/sizeof(sizes[0]), fp);
	fwrite(level, sizeof(level[0]), sizeof(level)/sizeof(level[0]), fp);
	fclose(fp);
}

void init(){
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	IMG_Init(IMG_INIT_PNG);
	gameRunning = 1;
	window.create("snek editor", SCREEN_WIDTH, SCREEN_HEIGHT);
	grid = window.createGrid(SIZEX,SIZEY);
	tileSprites.push_back(window.loadTexture("empty.png"));
	tileSprites.push_back(window.loadTexture("head.png"));
	tileSprites.push_back(window.loadTexture("tail.png"));
	tileSprites.push_back(window.loadTexture("apple.png"));
	tileSprites.push_back(window.loadTexture("wall.png"));
	//SDL_SetTextureColorMod(test, 255, 10, 10);
	//Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	srand((unsigned)time(0));
}


void eventLoop(){
	SDL_Event event;
 	while (SDL_PollEvent(&event))
 	{
		switch (event.type) {
			case SDL_QUIT:{
					gameRunning=0;
		  		break;
			}
			case SDL_MOUSEBUTTONDOWN:{
				auto pos = grid.getCell(event.button.x, event.button.y);
				if(pos == (std::pair<int,int>){-1,-1}){
					fileWrite();
				}
				else{
					level[pos.second*SIZEX + pos.first] ^= 4;
					fileWrite();
				}
			}
			case SDL_KEYDOWN:{
				switch (event.key.keysym.sym) {
				}
			}

			case SDL_WINDOWEVENT:{
				//levelGrid = window.createGrid(4,3);
				grid = window.createGrid(SIZEX, SIZEY, 1.0);
		  	break;
			}
		}
	}
}


void drawBoard(){
	SDL_Rect src;
	src = grid.getSize();
	window.drawRect(100,100,100,&src);
	src.x = 0, src.y = 0, src.w = 16, src.h = 16;
	for(int x=0; x<SIZEX; x++){
		for(int y=0; y<SIZEY; y++){
			//printf("%d\n", snek.getXY(x,y));
			window.renderGrid(x,y,tileSprites[level[y*SIZEX + x]], &grid, src);
			//window.renderGrid(x,y,tileSprites[1], &grid, src);
		}
	}
}

int main(int argc, char* args[]){
	init();
	loop = menu;
	unsigned int time=0;
	while(gameRunning){
		while(loop == menu && gameRunning){
			window.clear(50,50,50);
			eventLoop();
			drawBoard();
			SDL_Delay(int(1000/FPS)); //delay
			window.display();
		}
	}
	window.cleanUp();
	SDL_Quit();
	return 0;
}
