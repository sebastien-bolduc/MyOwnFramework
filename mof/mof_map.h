/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.30
 * @since 2012-01-17
 */

#include <assert.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#include "mof_collisionbox.h"

#ifndef MOF_MAP_H_
#define MOF_MAP_H_

#define MOF_MAP_TYPE (1<<3)		/* dynamic type checking */

/**
 * mof_Map class.
 */ 
typedef struct {
  unsigned int type;
  int *map;
  int width;						/* dimension in square(s) */
  int height;
  int unit;
  mof_Collisionbox *collision;
  SDL_Surface *screen;				/* copy of the current SDL surface */
} mof_Map;

/**
 * Load map.
 * 
 * @return A pointer to the map array.
 */
int *mof_Map__loadmap(void)
{
  static int map[120] = {
						 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
						 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
						 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
						 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1,
						 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
						 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
						 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1,
						 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1,
						 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
						 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
					   };
					
  return map;
}

/**
 * Create list of collision box.
 * 
 * @param map Pointer to a mof_Map object.
 */
void mof_Map__createCollisionbox(mof_Map *map)
{
  int i, j;
  for (i = 0; i < map->height; i++)
  {
	for (j = 0; j < map->width; j++)
	{
	  if (map->map[(i * map->width) + j])
	  {
		mof_Collisionbox__add(map->collision, j * map->unit, i * map->unit, map->unit, map->unit);
	  }
	}
  }
}

/**
 * Constructor.
 *  
 * @param map Pointer to a mof_Map object.
 */
void mof_Map__construct(mof_Map *map)
{
  /* here OR the MOF_MAP_TYPE constant into the type */
  map->type |= MOF_MAP_TYPE;
   
  map->map = mof_Map__loadmap();
  map->width = 12;
  map->height = 10;
  map->unit = 64;
  map->collision = mof_Collisionbox__new(0, 0, map->unit, map->unit);
  
  /* create collision box for map */
  mof_Map__createCollisionbox(map);
}

/**
 * New.
 * 
 * @param screen A copy of the current SDL surface.
 * @return       An object mof_Map.
 */
mof_Map *mof_Map__new(SDL_Surface *screen) 
{	
  mof_Map *map = malloc(sizeof(mof_Map));
  map->type = MOF_MAP_TYPE;
  map->screen = screen;
  
  /* call the constructor */
  mof_Map__construct(map);
  
  return map;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param map Pointer to a mof_Map object.
 */
void mof_Map__check(mof_Map *map)
{
  /* check if we have a valid mof_Map object */
  if (map == NULL || 
	  !(map->type & MOF_MAP_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param map Pointer to a mof_Map object.
 */ 
void mof_Map__destroy(mof_Map *map)
{
  /* check if we have a valid mof_Map object */
  mof_Map__check(map);

  /* set type to 0 indicate this is no longer a mof_Map object */
  map->type = 0;

  /* free the memory allocated for the object */
  mof_Collisionbox__destroy(map->collision);
  free(map);
}

/**
 * Drawing map.
 * 
 * @param map     Pointer to a mof_Map object.
 * @param offsetX Offset for the X coordinate.
 * @param offsetY Offset for the Y coordinate.
 */
void mof_Map__draw(mof_Map *map, int offsetX, int offsetY)
{
  /* check if we have a valid mof_Map object */
  mof_Map__check(map);
  
  int i, j;
  for (i = 0; i < map->height; i++)
  {
	for (j = 0; j < map->width; j++)
	{
	  if (map->map[(i * map->width) + j])
	  {
		boxRGBA(map->screen, j * map->unit - offsetX, i * map->unit - offsetY, (j * map->unit) + map->unit - offsetX, (i * map->unit) + map->unit - offsetY, 0, 0, 255, 255);
	  }
	}
  }
}

#endif