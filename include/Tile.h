#ifndef TILE_H
#define TILE_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

//Tile constants
const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

//The different tile sprites
const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//The tile
class Tile
{
    public:
		//Initializes position and type
		Tile( int x, int y, int tileType );

		//Shows the tile
		void render( SDL_Rect& camera , SDL_Renderer* , SDL_Rect* );

		//Get the tile type
		int getType();

		//Get the collision box
		SDL_Rect getBox();

		//Box collision detector
        bool checkCollision( SDL_Rect a, SDL_Rect b );

        //Get tile variable
        int getTile();

    private:
		//The attributes of the tile
		SDL_Rect mBox;

		//The tile type
		int mType;
};

#endif // TILE_H
