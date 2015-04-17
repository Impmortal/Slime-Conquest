#ifndef TILE_H
#define TILE_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

//Tile constants
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TOTAL_TILES = 4650;
const int TOTAL_TILE_SPRITES = 33;

//The different tile sprites
const int TILE_TRANSPAENT = 0;
const int TILE_TREE_LOWEST = 1;
const int TILE_TREE_LOW = 2;
const int TILE_TREE_MID = 3;
const int TILE_TREE_HIGH = 4;
const int TILE_LEAF_LEFT_LOWEST = 5;
const int TILE_LEAF_LEFT_LOW = 6;
const int TILE_LEAF_LEFT_HIGH = 7;
const int TILE_LEAF_LEFT_HIGHEST = 8;
const int TILE_LEAF_RIGHT_LOWEST = 9;
const int TILE_LEAF_RIGHT_LOW = 10;
const int TILE_LEAF_RIGHT_HIGH = 11;
const int TILE_LEAF_RIGHT_HIGHEST = 12;
const int TILE_LEAF_MID_HIGH = 13;
const int TILE_LEAF_MID_HIGHEST = 14;
const int TILE_CEMETARY = 15;
const int TILE_ONE_SMALLTREE = 16;
const int TILE_TWO_SMALLTREE = 17;
const int TILE_ROCK_LEFT = 18;
const int TILE_ROCK_RIGHT = 19;
const int TILE_ROCK_HIGH = 20;

//Collision tiles
const int TILE_GROUND_TOPLEFT = 21;
const int TILE_GROUND_TOPMID = 22;
const int TILE_GROUND_TOPRIGHT = 23;
const int TILE_GROUND_LEFT = 24;
const int TILE_GROUND_RIGHT = 25;
const int TILE_NEARHIGH_LEFT = 26;
const int TILE_NEARHIGH_RIGHT = 27;
const int TILE_HIGHGROUND_TOPLEFT = 28;
const int TILE_HIGHGROUND_TOPMID = 29;
const int TILE_HIGHGROUND_TOPRIGHT = 30;
const int TILE_HIGHGROUND_LEFT = 31;
const int TILE_HIGHGROUND_RIGHT = 32;


//The dimensions of the level
const int LEVEL_WIDTH = 29760;
const int LEVEL_HEIGHT = 600;

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
