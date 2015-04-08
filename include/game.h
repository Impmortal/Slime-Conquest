#ifndef GAME_H
#define GAME_H

#include "Tile.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
using namespace std;
//Declare Player's animation directory
extern string direc_player;

//Declare Amount of player's animation frame
extern const int PLAYER_WALKING_ANIMATION_FRAMES;

//Declare Size of player
extern int player_x;
extern int player_y;
extern int player_w;
extern int player_h;

//Declare window
extern SDL_Window* gWindow;

//Declare window's renderer
extern SDL_Renderer* gRenderer;

//Declare Screen resolution qHD (16:9)
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

class game
{
    public:
        //Starts up SDL and creates window
        bool init();

        //Frees media and shuts down SDL
        void close( Tile* );

        //Loads media
        bool loadMedia( Tile* tiles[] );

        //Sets tiles from tile map
        bool setTiles( Tile *tiles[] );

        //Checks collision box against set of tiles
        bool touchesWall( SDL_Rect box, Tile* tiles[] );

        //Box collision detector
        bool checkCollision( SDL_Rect a, SDL_Rect b );

        //Main program running the game
        int main( int , char* );
};

#endif // GAME_H
