#include "game.h"
#include "player.h"
#include "LTexture.h"
#include "Tile.h"
#include "iostream"
#include <string>

//Player's animation directory
std::string direc_player = "image/slime.png";

//Amount of player's animation frame
const int PLAYER_WALKING_ANIMATION_FRAMES = 7;

//Size of player
int player_x = 64;
int player_y = 0;
int player_w = 64;
int player_h = 64;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Screen resolution qHD (16:9)
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 540;

//Walking animation
LTexture gPlayerTexture;
LTexture gTileTexture;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];
SDL_Rect gPlayerClips[ PLAYER_WALKING_ANIMATION_FRAMES ];

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool game::checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

void close( Tile* tiles[] )
{
	//Deallocate tiles
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
		 if( tiles[ i ] == NULL )
		 {
			delete tiles[ i ];
			tiles[ i ] = NULL;
		 }
	}

	//Free loaded images
	gPlayerTexture.freemem();
	gTileTexture.freemem();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool game::touchesWall( SDL_Rect box, Tile* tiles[] )
{
    //Go through the tiles
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        //If the tile is a wall type tile
        if( ( tiles[ i ]->getType() >= TILE_GROUND_TOPLEFT ) && ( tiles[ i ]->getType() <= TILE_HIGHGROUND_RIGHT ) )
        {
            //If the collision box touches the wall tile
            if( checkCollision( box, tiles[ i ]->getBox() ) )
            {
                return true;
            }
        }
    }
    //If no wall tiles were touched
    return false;
}

bool setTiles( Tile* tiles[] )
{
	//Success flag
	bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "image/STAGE3.map" );

    //If the map couldn't be loaded
    if( map == NULL )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		//Initialize the tiles
		for( int i = 0; i < TOTAL_TILES; ++i )
		{
			//Determines what kind of tile will be made
			int tileType = -1;

			//Read tile from map file
			map >> tileType;

			//If the was a problem in reading the map
			if( map.fail() )
			{
				//Stop loading map
				printf( "Error loading map: Unexpected end of file!\n" );
				tilesLoaded = false;
				break;
			}

			//If the number is a valid tile number
			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
			{
				tiles[ i ] = new Tile( x, y, tileType );
			}
			//If we don't recognize the tile type
			else
			{
				//Stop loading map
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			//Move to next tile spot
			x += TILE_WIDTH;

			//If we've gone too far
			if( x >= LEVEL_WIDTH )
			{
				//Move back
				x = 0;

				//Move to the next row
				y += TILE_HEIGHT;
			}
		}

		//Clip the sprite sheet
		if( tilesLoaded )
		{
			gTileClips[ TILE_TRANSPAENT ].x = 0;
			gTileClips[ TILE_TRANSPAENT ].y = 0;
			gTileClips[ TILE_TRANSPAENT ].w = TILE_WIDTH;
			gTileClips[ TILE_TRANSPAENT ].h = TILE_HEIGHT;

			gTileClips[ TILE_TREE_LOWEST ].x = 0;
			gTileClips[ TILE_TREE_LOWEST ].y = 384;
			gTileClips[ TILE_TREE_LOWEST ].w = TILE_WIDTH;
			gTileClips[ TILE_TREE_LOWEST ].h = TILE_HEIGHT;

			gTileClips[ TILE_TREE_LOW ].x = 0;
			gTileClips[ TILE_TREE_LOW ].y = 320;
			gTileClips[ TILE_TREE_LOW ].w = TILE_WIDTH;
			gTileClips[ TILE_TREE_LOW ].h = TILE_HEIGHT;

			gTileClips[ TILE_TREE_MID ].x = 0;
			gTileClips[ TILE_TREE_MID ].y = 256;
			gTileClips[ TILE_TREE_MID ].w = TILE_WIDTH;
			gTileClips[ TILE_TREE_MID ].h = TILE_HEIGHT;

			gTileClips[ TILE_TREE_HIGH ].x = 0;
			gTileClips[ TILE_TREE_HIGH ].y = 192;
			gTileClips[ TILE_TREE_HIGH ].w = TILE_WIDTH;
			gTileClips[ TILE_TREE_HIGH ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_LEFT_LOWEST ].x = 0;
			gTileClips[ TILE_LEAF_LEFT_LOWEST ].y = 128;
			gTileClips[ TILE_LEAF_LEFT_LOWEST ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_LEFT_LOWEST ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_LEFT_LOW ].x = 0;
			gTileClips[ TILE_LEAF_LEFT_LOW ].y = 64;
			gTileClips[ TILE_LEAF_LEFT_LOW ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_LEFT_LOW ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_LEFT_HIGH ].x = 384;
			gTileClips[ TILE_LEAF_LEFT_HIGH ].y = 576;
			gTileClips[ TILE_LEAF_LEFT_HIGH ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_LEFT_HIGH ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_LEFT_HIGHEST ].x = 384;
			gTileClips[ TILE_LEAF_LEFT_HIGHEST ].y = 512;
			gTileClips[ TILE_LEAF_LEFT_HIGHEST ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_LEFT_HIGHEST ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_MID_HIGH ].x = 448;
			gTileClips[ TILE_LEAF_MID_HIGH ].y = 576;
			gTileClips[ TILE_LEAF_MID_HIGH ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_MID_HIGH ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_MID_HIGHEST ].x = 448;
			gTileClips[ TILE_LEAF_MID_HIGHEST ].y = 512;
			gTileClips[ TILE_LEAF_MID_HIGHEST ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_MID_HIGHEST ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_RIGHT_LOWEST ].x = 128;
			gTileClips[ TILE_LEAF_RIGHT_LOWEST ].y = 128;
			gTileClips[ TILE_LEAF_RIGHT_LOWEST ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_RIGHT_LOWEST ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_RIGHT_LOW ].x = 128;
			gTileClips[ TILE_LEAF_RIGHT_LOW ].y = 64;
			gTileClips[ TILE_LEAF_RIGHT_LOW ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_RIGHT_LOW ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_RIGHT_HIGH ].x = 512;
			gTileClips[ TILE_LEAF_RIGHT_HIGH ].y = 576;
			gTileClips[ TILE_LEAF_RIGHT_HIGH ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_RIGHT_HIGH ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEAF_RIGHT_HIGHEST ].x = 512;
			gTileClips[ TILE_LEAF_RIGHT_HIGHEST ].y = 512;
			gTileClips[ TILE_LEAF_RIGHT_HIGHEST ].w = TILE_WIDTH;
			gTileClips[ TILE_LEAF_RIGHT_HIGHEST ].h = TILE_HEIGHT;

			gTileClips[ TILE_CEMETARY ].x = 128;
			gTileClips[ TILE_CEMETARY ].y = 576;
			gTileClips[ TILE_CEMETARY ].w = TILE_WIDTH;
			gTileClips[ TILE_CEMETARY ].h = TILE_HEIGHT;

			gTileClips[ TILE_ONE_SMALLTREE ].x = 192;
			gTileClips[ TILE_ONE_SMALLTREE ].y = 576;
			gTileClips[ TILE_ONE_SMALLTREE ].w = TILE_WIDTH;
			gTileClips[ TILE_ONE_SMALLTREE ].h = TILE_HEIGHT;

			gTileClips[ TILE_TWO_SMALLTREE ].x = 320;
			gTileClips[ TILE_TWO_SMALLTREE ].y = 576;
			gTileClips[ TILE_TWO_SMALLTREE ].w = TILE_WIDTH;
			gTileClips[ TILE_TWO_SMALLTREE ].h = TILE_HEIGHT;

			gTileClips[ TILE_ROCK_LEFT ].x = 0;
			gTileClips[ TILE_ROCK_LEFT ].y = 576;
			gTileClips[ TILE_ROCK_LEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_ROCK_LEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_ROCK_RIGHT ].x = 64;
			gTileClips[ TILE_ROCK_RIGHT ].y = 576;
			gTileClips[ TILE_ROCK_RIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_ROCK_RIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_ROCK_HIGH ].x = 64;
			gTileClips[ TILE_ROCK_HIGH ].y = 512;
			gTileClips[ TILE_ROCK_HIGH ].w = TILE_WIDTH;
			gTileClips[ TILE_ROCK_HIGH ].h = TILE_HEIGHT;

			gTileClips[ TILE_GROUND_TOPLEFT ].x = 256;
			gTileClips[ TILE_GROUND_TOPLEFT ].y = 0;
			gTileClips[ TILE_GROUND_TOPLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_GROUND_TOPLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_GROUND_TOPMID ].x = 320;
			gTileClips[ TILE_GROUND_TOPMID ].y = 0;
			gTileClips[ TILE_GROUND_TOPMID ].w = TILE_WIDTH;
			gTileClips[ TILE_GROUND_TOPMID ].h = TILE_HEIGHT;

			gTileClips[ TILE_GROUND_TOPRIGHT ].x = 448;
			gTileClips[ TILE_GROUND_TOPRIGHT ].y = 0;
			gTileClips[ TILE_GROUND_TOPRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_GROUND_TOPRIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_GROUND_LEFT ].x = 256;
			gTileClips[ TILE_GROUND_LEFT ].y = 256;
			gTileClips[ TILE_GROUND_LEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_GROUND_LEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_GROUND_RIGHT ].x = 320;
			gTileClips[ TILE_GROUND_RIGHT ].y = 256;
			gTileClips[ TILE_GROUND_RIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_GROUND_RIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_NEARHIGH_LEFT ].x = 448;
			gTileClips[ TILE_NEARHIGH_LEFT ].y = 256;
			gTileClips[ TILE_NEARHIGH_LEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_NEARHIGH_LEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_NEARHIGH_RIGHT ].x = 512;
			gTileClips[ TILE_NEARHIGH_RIGHT ].y = 256;
			gTileClips[ TILE_NEARHIGH_RIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_NEARHIGH_RIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_HIGHGROUND_TOPLEFT ].x = 256;
			gTileClips[ TILE_HIGHGROUND_TOPLEFT ].y = 192;
			gTileClips[ TILE_HIGHGROUND_TOPLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_HIGHGROUND_TOPLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_HIGHGROUND_TOPMID ].x = 320;
			gTileClips[ TILE_HIGHGROUND_TOPMID ].y = 192;
			gTileClips[ TILE_HIGHGROUND_TOPMID ].w = TILE_WIDTH;
			gTileClips[ TILE_HIGHGROUND_TOPMID ].h = TILE_HEIGHT;

			gTileClips[ TILE_HIGHGROUND_TOPRIGHT ].x = 448;
			gTileClips[ TILE_HIGHGROUND_TOPRIGHT ].y = 192;
			gTileClips[ TILE_HIGHGROUND_TOPRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_HIGHGROUND_TOPRIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_HIGHGROUND_LEFT ].x = 256;
			gTileClips[ TILE_HIGHGROUND_LEFT ].y = 320;
			gTileClips[ TILE_HIGHGROUND_LEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_HIGHGROUND_LEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_HIGHGROUND_RIGHT ].x = 320;
			gTileClips[ TILE_HIGHGROUND_RIGHT ].y = 320;
			gTileClips[ TILE_HIGHGROUND_RIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_HIGHGROUND_RIGHT ].h = TILE_HEIGHT;
		}
	}

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}

bool loadMedia( Tile* tiles[], SDL_Renderer* gRenderer )
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gPlayerTexture.loadFromFile( "image/slime.png" , gRenderer ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}
	else
    {
        //Set player sprite clips
        gPlayerClips[ 0 ].x = 0;
		gPlayerClips[ 0 ].y = 0;
		gPlayerClips[ 0 ].w = 64;
		gPlayerClips[ 0 ].h = 64;

		gPlayerClips[ 1 ].x = 64;
		gPlayerClips[ 1 ].y = 0;
		gPlayerClips[ 1 ].w = 64;
		gPlayerClips[ 1 ].h = 64;

		gPlayerClips[ 2 ].x = 128;
		gPlayerClips[ 2 ].y = 0;
		gPlayerClips[ 2 ].w = 64;
		gPlayerClips[ 2 ].h = 64;

		gPlayerClips[ 3 ].x = 192;
		gPlayerClips[ 3 ].y = 0;
		gPlayerClips[ 3 ].w = 64;
		gPlayerClips[ 3 ].h = 64;

		gPlayerClips[ 4 ].x = 256;
		gPlayerClips[ 4 ].y = 0;
		gPlayerClips[ 4 ].w = 64;
		gPlayerClips[ 4 ].h = 64;

		gPlayerClips[ 5 ].x = 320;
		gPlayerClips[ 5 ].y = 0;
		gPlayerClips[ 5 ].w = 64;
		gPlayerClips[ 5 ].h = 64;

		gPlayerClips[ 6 ].x = 384;
		gPlayerClips[ 6 ].y = 0;
		gPlayerClips[ 6 ].w = 64;
		gPlayerClips[ 6 ].h = 64;
	}

	//Load tile texture
	if( !gTileTexture.loadFromFile( "image/TILE PNG3.png" , gRenderer ) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	//Load tile map
	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}



	return success;
}

//bool loadAnimation(  )

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//The level tiles
		Tile* tileSet[ TOTAL_TILES ];

		//Load media
		if( !loadMedia( tileSet , gRenderer ) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The player that will be moving around on the screen
			Player slime;

			//Current animation frame
			int frame = 0;

			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the dot
					slime.handleEvent( e );
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Move the slime
                slime.move( tileSet );
                slime.setCamera( camera );

				//Render level
				for( int i = 0; i < TOTAL_TILES; ++i )
				{
					tileSet[ i ]->render( camera , gRenderer , gTileClips );
				}

				//Render slime
				SDL_Rect* currentClip = &gPlayerClips[ frame / 7 ];
				slime.render( camera , gRenderer, currentClip );

				//Update screen
				SDL_RenderPresent( gRenderer );

				//Go to next frame
				++frame;

				//Cycle animation
				if( frame / 7 >= PLAYER_WALKING_ANIMATION_FRAMES )
				{
					frame = 0;
				}
			}
		}

		//Free resources and close SDL
		close( tileSet );
	}

	return 0;
}
