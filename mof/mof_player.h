/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.20
 * @since 2012-01-21
 */
 
#include <assert.h>
#include <math.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#include "mof_avatar.h"
#include "mof_map.h"

#ifndef MOF_PLAYER_H_
#define MOF_PLAYER_H_

#define MOF_PLAYER_TYPE (1<<5)		/* dynamic type checking */

/**
 * mof_Player class.
 */ 
typedef struct {
  mof_Avatar parent;
  mof_Collisionbox *collision;
  SDL_Surface *screen;		/* copy of the current SDL surface */
} mof_Player;

/**
 * Constructor.
 *  
 * @param player Pointer to a mof_Player object.
 * @param x      Coordinate of player.
 * @param y      Coordinate of player.
 * @param angle  Direction of player.
 */
void mof_Player__construct(mof_Player *player, double x, double y, int angle)
{
  /* here OR the MOF_PLAYER_TYPE constant into the type */
  ((mof_Avatar *)player)->type |= MOF_PLAYER_TYPE;
  
  /* call the constructor of the 'mof_Avatar' class */
  mof_Avatar__construct((mof_Avatar *)player, x, y, angle);
  
  player->collision = mof_Collisionbox__new(x - 10, y - 10, 20, 20);
}

/**
 * New.
 * 
 * @param screen A copy of the current SDL surface.
 * @param x      Coordinate of player.
 * @param y      Coordinate of player.
 * @param angle  Direction of player.
 * @return       An object mof_Player.
 */
mof_Player *mof_Player__new(SDL_Surface *screen, double x, double y, int angle) 
{	
  mof_Player *player = malloc(sizeof(mof_Player));
  ((mof_Avatar *)player)->type = MOF_PLAYER_TYPE;
  player->screen = screen;
  
  /* call the constructor */
  mof_Player__construct(player, x, y, angle);
  
  return player;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param player Pointer to a mof_Player object.
 */
void mof_Player__check(mof_Player *player)
{
  /* check if we have a valid mof_Player object */
  if (player == NULL || 
	  !(((mof_Avatar *)player)->type & MOF_PLAYER_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param player Pointer to a mof_Player object.
 */ 
void mof_Player__destroy(mof_Player *player)
{
  /* check if we have a valid mof_Player object */
  mof_Player__check(player);

  /* set type to 0 indicate this is no longer a mof_Player object */
  ((mof_Avatar *)player)->type = 0;

  /* free the memory allocated for the object */
  mof_Collisionbox__destroy(player->collision);
  free(player);
}

/**
 * Move player forward.
 * 
 * @param player Pointer to a mof_Player object.
 * @param level  Pointer to a mof_Map object.
 */
void mof_Player__moveforward(mof_Player *player, mof_Map *level)
{
  /* check if we have a valid mof_Player object */
  mof_Player__check(player);	
	
  mof_Avatar__moveforward(((mof_Avatar *)player));
  
  mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
  
  /* handling collision */
  int flag = 0;
  mof_Collisionbox *bkup = NULL;
  for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
  {
	if (mof_Collisionbox__intersect(player->collision, level->collision->current))
	{
		  
	  /* resolve collision */
	  mof_Avatar__movebackward(((mof_Avatar *)player));
	  ((mof_Avatar *)player)->x += (cos(((mof_Avatar *)player)->angle * M_PI / 180) * 1);
	  mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
	  bkup = level->collision->current;
	  for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
	  {
		if (mof_Collisionbox__intersect(player->collision, level->collision->current))
		{
		  flag = 1;
		}
	  }
		
	  if (flag)
	  {
		flag = 0;
		((mof_Avatar *)player)->x -= (cos(((mof_Avatar *)player)->angle * M_PI / 180) * 1);
		((mof_Avatar *)player)->y -= (sin(((mof_Avatar *)player)->angle * M_PI / 180) * 1);
		mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
		for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
		{	  
		  if (mof_Collisionbox__intersect(player->collision, level->collision->current))
		  {
			flag = 1;  
		  }
		}
	  }
		
	  if (flag)
	  {
		flag = 0;
		((mof_Avatar *)player)->y += (sin(((mof_Avatar *)player)->angle * M_PI / 180) * 1);
		mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);	
	  }
		
	  level->collision->current = bkup;
	  /* resolve collision */
		
    }
  }
  /* handling collision */
}

/**
 * Move player backward.
 * 
 * @param player Pointer to a mof_Player object.
 * @param level  Pointer to a mof_Map object.
 */
void mof_Player__movebackward(mof_Player *player, mof_Map *level)
{
  /* check if we have a valid mof_Player object */
  mof_Player__check(player);	
	
  mof_Avatar__movebackward(((mof_Avatar *)player));
  
  mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
  
  /* handling collision */
  int flag = 0;
  mof_Collisionbox *bkup = NULL;
  for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
  {
	if (mof_Collisionbox__intersect(player->collision, level->collision->current))
	{
		  
	  /* resolve collision */
	  mof_Avatar__moveforward(((mof_Avatar *)player));
	  ((mof_Avatar *)player)->x -= (cos(((mof_Avatar *)player)->angle * M_PI / 180) * 1);
	  mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
	  bkup = level->collision->current;
	  for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
	  {
		if (mof_Collisionbox__intersect(player->collision, level->collision->current))
		{
		  flag = 1;
		}
	  }
		
	  if (flag)
	  {
		flag = 0;
		((mof_Avatar *)player)->x += (cos(((mof_Avatar *)player)->angle * M_PI / 180) * 1);
		((mof_Avatar *)player)->y += (sin(((mof_Avatar *)player)->angle * M_PI / 180) * 1);
		mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
		for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
		{	  
		  if (mof_Collisionbox__intersect(player->collision, level->collision->current))
		  {
			flag = 1;  
		  }
		}
	  }
		
	  if (flag)
	  {
		flag = 0;
		((mof_Avatar *)player)->y -= (sin(((mof_Avatar *)player)->angle * M_PI / 180) * 1);
		mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);	
	  }
		
	  level->collision->current = bkup;
	  /* resolve collision */
		
    }
  }
  /* handling collision */
}

/**
 * Move player left (of facing direction).
 * 
 * @param player Pointer to a mof_Player object.
 * @param level  Pointer to a mof_Map object.
 */
void mof_Player__moveleft(mof_Player *player, mof_Map *level)
{
  /* check if we have a valid mof_Player object */
  mof_Player__check(player);	
	
  mof_Avatar__moveleft(((mof_Avatar *)player));
  
  mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
  
  /* handling collision */
  int flag = 0;
  mof_Collisionbox *bkup = NULL;
  for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
  {
	if (mof_Collisionbox__intersect(player->collision, level->collision->current))
	{
		  
	  /* resolve collision */
	  mof_Avatar__moveright(((mof_Avatar *)player));
	  ((mof_Avatar *)player)->x += (cos((((mof_Avatar *)player)->angle + 90) * M_PI / 180) * 1);
	  mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
	  bkup = level->collision->current;
	  for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
	  {
		if (mof_Collisionbox__intersect(player->collision, level->collision->current))
		{
		  flag = 1;
		}
	  }
		
	  if (flag)
	  {
		flag = 0;
		((mof_Avatar *)player)->x -= (cos((((mof_Avatar *)player)->angle + 90) * M_PI / 180) * 1);
		((mof_Avatar *)player)->y -= (sin((((mof_Avatar *)player)->angle + 90) * M_PI / 180) * 1);
		mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
		for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
		{	  
		  if (mof_Collisionbox__intersect(player->collision, level->collision->current))
		  {
			flag = 1;  
		  }
		}
	  }
		
	  if (flag)
	  {
		flag = 0;
		((mof_Avatar *)player)->y += (sin((((mof_Avatar *)player)->angle + 90) * M_PI / 180) * 1);
		mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);	
	  }
		
	  level->collision->current = bkup;
	  /* resolve collision */
		
    }
  }
  /* handling collision */
}

/**
 * Move player right (of facing direction).
 * 
 * @param player Pointer to a mof_Player object.
 * @param level  Pointer to a mof_Map object.
 */
void mof_Player__moveright(mof_Player *player, mof_Map *level)
{
  /* check if we have a valid mof_Player object */
  mof_Player__check(player);	
	
  mof_Avatar__moveright(((mof_Avatar *)player));
  
  mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
  
  /* handling collision */
  int flag = 0;
  mof_Collisionbox *bkup = NULL;
  for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
  {
	if (mof_Collisionbox__intersect(player->collision, level->collision->current))
	{
		  
	  /* resolve collision */
	  mof_Avatar__moveleft(((mof_Avatar *)player));
	  ((mof_Avatar *)player)->x += (cos((((mof_Avatar *)player)->angle - 90) * M_PI / 180) * 1);
	  mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
	  bkup = level->collision->current;
	  for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
	  {
		if (mof_Collisionbox__intersect(player->collision, level->collision->current))
		{
		  flag = 1;
		}
	  }
		
	  if (flag)
	  {
		flag = 0;
		((mof_Avatar *)player)->x -= (cos((((mof_Avatar *)player)->angle - 90) * M_PI / 180) * 1);
		((mof_Avatar *)player)->y -= (sin((((mof_Avatar *)player)->angle - 90) * M_PI / 180) * 1);
		mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);
		for (level->collision->current = level->collision->first->next; level->collision->current != NULL; level->collision->current = level->collision->current->next)
		{	  
		  if (mof_Collisionbox__intersect(player->collision, level->collision->current))
		  {
			flag = 1;  
		  }
		}
	  }
		
	  if (flag)
	  {
		flag = 0;
		((mof_Avatar *)player)->y += (sin((((mof_Avatar *)player)->angle - 90) * M_PI / 180) * 1);
		mof_Collisionbox__move(player->collision, (int)((mof_Avatar *)player)->x - 10, (int)((mof_Avatar *)player)->y - 10);	
	  }
		
	  level->collision->current = bkup;
	  /* resolve collision */
		
    }
  }
  /* handling collision */
}

/**
 * Offset X for scrolling.
 * 
 * @param player Pointer to a mof_Player object.
 * @param map    Pointer to a mof_Map object.
 * @param limit  Limit for the player offset.
 * @return       Offset base on the player position.
 */
int mof_Player__offsetX(mof_Player *player, mof_Map *map, int limit)
{
  int max_offset = (map->width * map->unit) - player->screen->w;
  int offset = ((mof_Avatar *)player)->x - limit;
  
  if (max_offset < 0)
	max_offset = 0;
  
  if (offset < 0)
	return 0;
  else if (offset >= max_offset)
	return max_offset;
  else
	return offset;
}

/**
 * Offset Y for scrolling.
 * 
 * @param player Pointer to a mof_Player object.
 * @param map    Pointer to a mof_Map object.
 * @param limit  Limit for the player offset.
 * @return       Offset base on the player position.
 */
int mof_Player__offsetY(mof_Player *player, mof_Map *map, int limit)
{
  int max_offset = (map->height * map->unit) - player->screen->h;
  int offset = ((mof_Avatar *)player)->y - limit;
  
  if (max_offset < 0)
	max_offset = 0;
  
  if (offset < 0)
	return 0;
  else if (offset >= max_offset)
	return max_offset;
  else
	return offset;
}

/**
 * Drawing player.
 * 
 * @param player  Pointer to a mof_Player object.
 * @param offsetX Offset for the X coordinate.
 * @param offsetY Offset for the Y coordinate.
 */
void mof_Player__draw(mof_Player *player, int offsetX, int offsetY)
{
  /* check if we have a valid mof_Player object */
  mof_Player__check(player);
  
  filledEllipseRGBA(player->screen, ((mof_Avatar *)player)->x - offsetX, ((mof_Avatar *)player)->y - offsetY, 10, 10, 255, 0, 0, 255);
  int lineX = ((mof_Avatar *)player)->x - offsetX + (int)(cos((double)((mof_Avatar *)player)->angle * M_PI / 180) * 20);
  int lineY = ((mof_Avatar *)player)->y - offsetY - (int)(sin((double)((mof_Avatar *)player)->angle * M_PI / 180) * 20);
  int arrow1X = ((mof_Avatar *)player)->x - offsetX + (int)(cos((double)(((mof_Avatar *)player)->angle + 30) * M_PI / 180) * 14);
  int arrow1Y = ((mof_Avatar *)player)->y - offsetY - (int)(sin((double)(((mof_Avatar *)player)->angle + 30) * M_PI / 180) * 14);
  int arrow2X = ((mof_Avatar *)player)->x - offsetX + (int)(cos((double)(((mof_Avatar *)player)->angle - 30) * M_PI / 180) * 14);
  int arrow2Y = ((mof_Avatar *)player)->y - offsetY - (int)(sin((double)(((mof_Avatar *)player)->angle - 30) * M_PI / 180) * 14); 
  lineRGBA(player->screen, (int)((mof_Avatar *)player)->x - offsetX, (int)((mof_Avatar *)player)->y - offsetY, lineX, lineY, 0, 255, 0, 255);
  lineRGBA(player->screen, arrow1X, arrow1Y, lineX, lineY, 0, 255, 0, 255);
  lineRGBA(player->screen, arrow2X, arrow2Y, lineX, lineY, 0, 255, 0, 255);
}

#endif