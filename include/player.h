#ifndef PLAYER_H
#define PLAYER_H

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "Tile.h"

class Player
{
    public:
		//The dimensions of the player
		static const int PLAYER_WIDTH = 61;
		static const int PLAYER_HEIGHT = 42;

		//Maximum axis velocity of the player
		static const int PLAYER_VEL = 6;

		//Initializes the variables
		Player();

		//Takes key presses and adjusts the dot's velocity
		void handleEvent( SDL_Event& e );

		//Moves the player and check collision against tiles
		void move( Tile *tiles[] );

		//
		void gravity( Tile *tiles[] );

		//Centers the camera over the player
		void setCamera( SDL_Rect& camera );

		//Shows the player on the screen
		void render( SDL_Rect& camera , SDL_Renderer* gRenderer, SDL_Rect* );

    private:
		//Collision box of the player
		SDL_Rect mBox;

		//The velocity of the player
		int mVelX, mVelY;

		int startTime;

		int Keyup;

		int onGround;

		int respwan_x;
		int respwan_y;
};
#endif // PLAYER_H
