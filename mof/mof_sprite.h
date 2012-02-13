/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.10
 * @since 2012-01-31
 */
 
#include <assert.h>
#include <math.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#include "mof_avatar.h"
#include "mof_graphicelement.h"

#ifndef MOF_SPRITE_H_
#define MOF_SPRITE_H_

#define MOF_SPRITE_TYPE (1<<6)		/* dynamic type checking */

/**
 * mof_Sprite class.
 */ 
typedef struct {
  mof_Avatar parent;
  SDL_Surface *screen;		/* copy of the current SDL surface */
} mof_Sprite;

/**
 * Constructor.
 *  
 * @param sprite Pointer to a mof_Sprite object.
 * @param x      Coordinate of sprite.
 * @param y      Coordinate of sprite.
 */
void mof_Sprite__construct(mof_Sprite *sprite, double x, double y)
{
  /* here OR the MOF_SPRITE_TYPE constant into the type */
  ((mof_Avatar *)sprite)->type |= MOF_SPRITE_TYPE;
  
  /* call the constructor of the 'mof_Avatar' class */
  mof_Avatar__construct((mof_Avatar *)sprite, x, y, 0);
}

/**
 * New.
 * 
 * @param screen A copy of the current SDL surface.
 * @param x      Coordinate of sprite.
 * @param y      Coordinate of sprite.
 * @return       An object mof_Sprite.
 */
mof_Sprite *mof_Sprite__new(SDL_Surface *screen, double x, double y) 
{	
  mof_Sprite *sprite = malloc(sizeof(mof_Sprite));
  ((mof_Avatar *)sprite)->type = MOF_SPRITE_TYPE;
  sprite->screen = screen;
  
  /* call the constructor */
  mof_Sprite__construct(sprite, x, y);
  
  return sprite;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param sprite Pointer to a mof_Sprite object.
 */
void mof_Sprite__check(mof_Sprite *sprite)
{
  /* check if we have a valid mof_Sprite object */
  if (sprite == NULL || 
	  !(((mof_Avatar *)sprite)->type & MOF_SPRITE_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param sprite Pointer to a mof_Sprite object.
 */ 
void mof_Sprite__destroy(mof_Sprite *sprite)
{
  /* check if we have a valid mof_Sprite object */
  mof_Sprite__check(sprite);

  /* set type to 0 indicate this is no longer a mof_Sprite object */
  ((mof_Avatar *)sprite)->type = 0;

  /* free the memory allocated for the object */
  free(sprite);
}

/**
 * Drawing sprite.
 * 
 * @param sprite Pointer to a mof_Sprite object.
 * @param offsetX Offset for the X coordinate.
 * @param offsetY Offset for the Y coordinate.
 */
void mof_Sprite__draw(mof_Sprite *sprite, int offsetX, int offsetY)
{
  boxRGBA(sprite->screen, ((mof_Avatar *)sprite)->x - 5 - offsetX, ((mof_Avatar *)sprite)->y - 5 - offsetY, 
						  ((mof_Avatar *)sprite)->x + 5 - offsetX, ((mof_Avatar *)sprite)->y + 5 - offsetY, 0, 255, 0, 255);
}

/**
 * Drawing sprite (3D).
 * 
 * @param scene  Pointer to a mof_Graphicelement object.
 * @param sprite Pointer to a mof_Sprite object.
 * @param player Pointer to a mof_Player object.
 */
void mof_Sprite__draw3Dscene(mof_Graphicelement *scene, mof_Sprite *sprite, mof_Player *player)
{
  /* check if we have a valid mof_Sprite object */
  mof_Sprite__check(sprite);
  
  double distanceFromProjectionPlane = (640 / 2) / tan((60 / 2) * M_PI / 180);
  
  /* angle relative to player */
  double angle;
  double opp = ((mof_Avatar *)sprite)->y - ((mof_Avatar *)player)->y;
  double adj = ((mof_Avatar *)sprite)->x - ((mof_Avatar *)player)->x;
  
  if (adj == 0 && opp < 0)
	angle = 90;
  else if (adj == 0)
	angle = 270;	
  else if (adj > 0 && opp <= 0)
	angle = fabs(atan(opp / adj)) * 180 / M_PI;
  else if (adj < 0 && opp <= 0)
	angle = 180 - fabs(atan(opp / adj)) * 180 / M_PI;
  else if (adj < 0 && opp > 0)
	angle = 180 + fabs(atan(opp / adj)) * 180 / M_PI;
  else
	angle = 360 - fabs(atan(opp / adj)) * 180 / M_PI;

  /* fuck this (working finally) */
  angle = (fabs(angle - (double)((mof_Avatar *)player)->angle) > 90) ? 
		  ((angle > (double)((mof_Avatar *)player)->angle) ? -(360 - fabs(angle - (double)((mof_Avatar *)player)->angle)) : (360 - fabs(angle - (double)((mof_Avatar *)player)->angle))) : 
		  (angle - (double)((mof_Avatar *)player)->angle);
  
  /* distance from player */
  double Sx = ((mof_Avatar *)sprite)->x;
  double Sy = ((mof_Avatar *)sprite)->y;
  double Px = ((mof_Avatar *)player)->x;
  double Py = ((mof_Avatar *)player)->y;
  double distance = sqrt(pow((Sx - Px), 2) + pow((Sy - Py), 2));
  
  /* get top and bottom of sprite */
  int bottom = (int)floor(10 * distanceFromProjectionPlane / distance + 240);
  int top = (int)floor((-10) * distanceFromProjectionPlane / distance + 240);
  double ratio = (double)(bottom - top) / 20;
  
  int Ps = 0;
  Ps = -floor(320 * angle / 30);
  mof_Graphicelement__add(scene, distance, (320 - (int)(10 * ratio) + Ps), top, ((320 + (int)(10 * ratio) + Ps) - (320 - (int)(10 * ratio) + Ps)), (bottom - top), 
							0, 255, 0, 255);
}

#endif