#include "player.h"
#include "LTexture.h"
#include "game.h"
#include <math.h>
#include <iostream>
#include <unistd.h>

extern LTexture gDotTexture;
game gTileCollsion;

Player::Player()
{
    //Initialize the collision box
    mBox.x = 300;
    mBox.y = 0;
	mBox.w = PLAYER_WIDTH;
	mBox.h = PLAYER_HEIGHT;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    startTime = 0;

    noKeyup = 1;

}

void Player::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= PLAYER_VEL; noKeyup = 0; break;
            case SDLK_DOWN: mVelY += PLAYER_VEL; break;
            case SDLK_LEFT: mVelX -= PLAYER_VEL; break;
            case SDLK_RIGHT: mVelX += PLAYER_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += PLAYER_VEL; noKeyup = 1; break;
            case SDLK_DOWN: mVelY -= PLAYER_VEL; break;
            case SDLK_LEFT: mVelX += PLAYER_VEL; break;
            case SDLK_RIGHT: mVelX -= PLAYER_VEL; break;
        }
    }
}

void Player::move( Tile *tiles[] )
{
    //Move the dot left or right
    mBox.x += mVelX;

    //If the dot went too far to the left or right or touched a wall
    if( ( mBox.x < 0 ) || ( mBox.x + PLAYER_WIDTH > LEVEL_WIDTH ) || gTileCollsion.touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.x -= mVelX;
    }

    //Move the dot up or down
    mBox.y += mVelY;

    //If the dot went too far up or down or touched a wall
    if( ( mBox.y < 0 ) || ( mBox.y + PLAYER_HEIGHT > LEVEL_HEIGHT ) || gTileCollsion.touchesWall( mBox, tiles ) )
    {
        //move back
        mBox.y -= mVelY;
    }
}

void Player::gravity( Tile *tiles[] )
{
    //While character should go down more
    if( ( !gTileCollsion.touchesWall( mBox, tiles )) && ( noKeyup == 1 ) )
    {
        //move down
        mBox.y += 10;

        //If the character went too far up or down or touched a wall
        if( ( mBox.y < 0 ) || ( mBox.y + PLAYER_HEIGHT > LEVEL_HEIGHT ) || gTileCollsion.touchesWall( mBox, tiles ) )
        {
            //move back
            mBox.y -= 10;
        }
    }
}

void Player::setCamera( SDL_Rect& camera )
{
	//Center the camera over the player
	camera.x = ( mBox.x + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = ( mBox.y + PLAYER_HEIGHT/ 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if( camera.x < 0 )
	{
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > LEVEL_WIDTH - camera.w )
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT - camera.h )
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Player::render( SDL_Rect& camera , SDL_Renderer* gRenderer )
{
    //Show the dot
	gDotTexture.render( mBox.x - camera.x, mBox.y - camera.y , gRenderer );
}
