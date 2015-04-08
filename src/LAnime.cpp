#ifndef LANIME_H
#define LANIME_H

LTexture gSpriteSheetTexture;

class LAnime
{
    public:

        //Loads media
        bool loadMedia( char * directory, SDL_Renderer* gRenderer, SDL_Rect gSpriteClips[],
                       int have_x, int have_y, int have_w, int have_h, int have_frame );
};

bool LAnime::loadMedia( char * directory, SDL_Renderer* gRenderer, SDL_Rect gSpriteClips[],
                       int have_x, int have_y, int have_w, int have_h, int have_frame )
{
	//Loading success flag
	bool success = true;

    //Looping Material
    int run;

	//Load sprite sheet texture
	if( !gSpriteSheetTexture.loadFromFile( directory , gRenderer ) )
	{
		printf( "Failed to load walking animation texture!\n" );
		success = false;
	}
	else
	{
		//Set sprite clips
		for (run = 0; run < have_frame; run++)
        {
            gSpriteClips[ run ].x = (have_x*(run+1)) - have_x;
            gSpriteClips[ run ].y = have_y;
            gSpriteClips[ run ].w = have_w;
            gSpriteClips[ run ].h = have_h;
        }
	}

	return success;
}

#endif // LANIME_H

