/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 0.01
 * @since 2012-01-15
 * 
 * gcc myownframework.c `sdl-config --cflags --libs` -lSDL_gfx -lSDL_ttf -o myownframework
 */

#include <math.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_ttf.h"

#include "mof/mof_collisionbox.h"
#include "mof/mof_font.h"
#include "mof/mof_keyboard.h"
#include "mof/mof_map.h"
#include "mof/mof_player.h"
#include "mof/mof_raycaster.h"
#include "mof/mof_time.h"

SDL_Surface *screen;
SDL_Event event;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char *WINDOW_TITLE = "My Own Framework";
const char *WINDOW_FONT = "/home/user/Downloads/arial.ttf";

mof_Player *player = NULL;
mof_Font *text = NULL;
mof_Map *level = NULL;
mof_Time *timer = NULL;

char test[100] = {"/0"};
int mapflag = 0;
int release_m = 1;

/**
 * Initialization.
 */
void mof__init()
{
  /* video */
  SDL_Init(SDL_INIT_VIDEO);
  
  /* window */
  screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
  SDL_WM_SetCaption(WINDOW_TITLE, 0);

  /* keyboard */
  //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  level = mof_Map__new(screen);
  player = mof_Player__new(screen, 320, 320, 90);
  text = mof_Font__new(screen, WINDOW_FONT);
  timer = mof_Time__new(); 
}

/**
 * Loading.
 */
void mof__load()
{
}

/**
 * Unloading.
 */
void mof__unload()
{
}

/**
 * Updating.
 * 
 * @param running_loop Determine if the loop still have to be executed.
 */
void mof__update(int *running_loop)
{	
  if (SDL_PollEvent(&event))
  {
	/* handling the SDL window */
	if (event.type == SDL_QUIT)
	{
	  *running_loop = 0;
	}
	if (event.type == SDL_VIDEORESIZE)
	{
	  screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
	}
	
	/* handling the mouse */
	if (event.type == SDL_MOUSEMOTION)
	{
	  if (event.motion.xrel > 0)
	  {
		mof_Avatar__rotate(((mof_Avatar *)player), 1);
	  }
	  if (event.motion.xrel < 0)
	  {
		mof_Avatar__rotate(((mof_Avatar *)player), -1);
	  }
	}
	
	/* handling the keyboard (game-type input) */
	mof_Keyboard__pollevent(&event);
  }
  
  /* taking care of the keyboard (game-type input) */
  if (mof_Keyboard__checkkey(SDLK_LEFT))
  {
	mof_Player__moveleft(player, level);
  }
  if (mof_Keyboard__checkkey(SDLK_RIGHT))
  {
	mof_Player__moveright(player, level);
  }
  if (mof_Keyboard__checkkey(SDLK_UP))
  {
	mof_Player__moveforward(player, level);
  }
  if (mof_Keyboard__checkkey(SDLK_DOWN))
  {
	mof_Player__movebackward(player, level);
  }
  
  if (mof_Keyboard__checkkey(SDLK_m))
  {
	if (release_m)
	{
	  mapflag = (mapflag) ? 0 : 1;
	  release_m = 0;
	}
  }
  else 
  {
	release_m = 1;
  }
}

/**
 * Drawing.
 */
void mof__draw()
{	
  /* clear the screen */
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  
  int offsetX = mof_Player__offsetX(player, level, 320);
  int offsetY = mof_Player__offsetY(player, level, 240);
  
  if (mapflag)
  {
	mof_Map__draw(level, offsetX, offsetY);
    mof_Player__draw(player, offsetX, offsetY);
    mof_Raycaster__draw(player, level, offsetX, offsetY);
  }
  else 
  {
    mof_Raycaster__draw3D(player, level);
  }
}

/**
 * Main function of the application.
 * 
 * @param argc Arguments passed on the command line (number).
 * @param argv Arguments passed on the command line (values).
 * @return     0 (meaning we're done)
 */
int main(int argc, char **argv)
{
  mof__init();
	
  int running_loop = 1;
  while(running_loop)
  {
    mof__update(&running_loop);
	   
	mof_Time__start(timer); 

      mof__draw();
	  
	  /* print to bottom of screen */
	  sprintf(test, "(elapsed) milli: %3.3lld -- micro: %6.6lld", mof_Time__gettime_elapsed_msec(timer), mof_Time__gettime_elapsed_usec(timer));
	  mof_Font__printf(text, test, 20, screen->h - 40);
	  
	  SDL_Delay(1);
	
	mof_Time__stop(timer);
	  
	/* print to bottom of screen */
	sprintf(test, "(drawing) milli: %3.3lld -- micro: %6.6lld", mof_Time__gettime_msec(timer), mof_Time__gettime_usec(timer));
	mof_Font__printf(text, test, 20, screen->h - 20);

	SDL_Flip(screen);
  }

  mof_Map__destroy(level);
  mof_Player__destroy(player);
  mof_Font__destroy(text);
  mof_Time__destroy(timer);
  SDL_Quit();

  return 0;
}