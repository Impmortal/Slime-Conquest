#ifndef LTEXTURE_H
#define LTEXTURE_H

#include "Tile.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>

class LTexture
{
	public:
		//Initializes variables
        LTexture();

        //Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string , SDL_Renderer* );

		//Deallocates texture
		void freemem();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		//Renders texture at given point
		void render( int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:

		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

#endif // LTEXTURE_H
