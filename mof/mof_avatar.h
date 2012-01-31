/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.10
 * @since 2012-01-17
 */

#include <assert.h>
#include <math.h>

#include "mof_collisionbox.h"

#ifndef MOF_AVATAR_H_
#define MOF_AVATAR_H_

#define MOF_AVATAR_TYPE (1<<2)		/* dynamic type checking */

/**
 * mof_Avatar class.
 */ 
typedef struct {
  unsigned int type;
  double x;
  double y;
  int angle;				/* angle's degree */ 
} mof_Avatar;

/**
 * Constructor.
 *  
 * @param avatar Pointer to a mof_Time object.
 * @param x      Coordinate of avatar.
 * @param y      Coordinate of avatar.
 * @param angle  Direction of avatar.
 */
void mof_Avatar__construct(mof_Avatar *avatar, double x, double y, int angle)
{
  /* here OR the MOF_AVATAR_TYPE constant into the type */
  avatar->type |= MOF_AVATAR_TYPE;
   
  avatar->x = x;
  avatar->y = y;
  avatar->angle = angle;
}

/**
 * New.
 * 
 * @param screen A copy of the current SDL surface.
 * @param x      Coordinate of avatar.
 * @param y      Coordinate of avatar.
 * @param angle  Direction of avatar.
 * @return       An object mof_Avatar.
 */
mof_Avatar *mof_Avatar__new(SDL_Surface *screen, double x, double y, int angle) 
{	
  mof_Avatar *avatar = malloc(sizeof(mof_Avatar));
  avatar->type = MOF_AVATAR_TYPE;
  
  /* call the constructor */
  mof_Avatar__construct(avatar, x, y, angle);
  
  return avatar;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param avatar Pointer to a mof_Avatar object.
 */
void mof_Avatar__check(mof_Avatar *avatar)
{
  /* check if we have a valid mof_Avatar object */
  if (avatar == NULL || 
	  !(avatar->type & MOF_AVATAR_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param avatar Pointer to a mof_Avatar object.
 */ 
void mof_Avatar__destroy(mof_Avatar *avatar)
{
  /* check if we have a valid mof_Avatar object */
  mof_Avatar__check(avatar);

  /* set type to 0 indicate this is no longer a mof_Avatar object */
  avatar->type = 0;

  /* free the memory allocated for the object */
  free(avatar);
}

/**
 * Move avatar forward.
 * 
 * @param avatar Pointer to a mof_Avatar object.
 */
void mof_Avatar__moveforward(mof_Avatar *avatar)
{
  /* check if we have a valid mof_Avatar object */
  mof_Avatar__check(avatar);	
	
  /* don't forget to convert degree to rad */
  avatar->x += (cos(avatar->angle * M_PI / 180) * 1);
  avatar->y -= (sin(avatar->angle * M_PI / 180) * 1);
}

/**
 * Move avatar backward.
 * 
 * @param avatar Pointer to a mof_Avatar object.
 */
void mof_Avatar__movebackward(mof_Avatar *avatar)
{
  /* check if we have a valid mof_Avatar object */
  mof_Avatar__check(avatar);	

  /* don't forget to convert degree to rad */
  avatar->x -= (cos(avatar->angle * M_PI / 180) * 1);
  avatar->y += (sin(avatar->angle * M_PI / 180) * 1);
}

/**
 * Move avatar left (of facing direction).
 * 
 * @param avatar Pointer to a mof_Avatar object.
 */
void mof_Avatar__moveleft(mof_Avatar *avatar)
{
  /* check if we have a valid mof_Avatar object */
  mof_Avatar__check(avatar);	
	
  /* don't forget to convert degree to rad */
  avatar->x += (cos((avatar->angle + 90) * M_PI / 180) * 1);
  avatar->y -= (sin((avatar->angle + 90) * M_PI / 180) * 1);
}

/**
 * Move avatar right (of facing direction).
 * 
 * @param avatar Pointer to a mof_Avatar object.
 */
void mof_Avatar__moveright(mof_Avatar *avatar)
{
  /* check if we have a valid mof_Avatar object */
  mof_Avatar__check(avatar);
	
  /* don't forget to convert degree to rad */
  avatar->x += (cos((avatar->angle - 90) * M_PI / 180) * 1);
  avatar->y -= (sin((avatar->angle - 90) * M_PI / 180) * 1);
}

/**
 * Rotate direction of avatar.
 * 
 * @param avatar Pointer to a mof_Avatar object.
 * @param value  How much to rotate avatar.
 */
void mof_Avatar__rotate(mof_Avatar *avatar, int value)
{
  /* check if we have a valid mof_Avatar object */
  mof_Avatar__check(avatar);
  
  avatar->angle += value;
  /* check angle limits */
  if (avatar->angle < 0)
  {
	avatar->angle = 359;
  }
  if (avatar->angle >= 360)
  {
	avatar->angle = 0;
  }
}

#endif