/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.10
 * @since 2012-02-08 
 * 
 * This class work just like a graphic pipeline.  Graphic elements are added
 * to a double linked list which is use for the final rendering of the scene.
 * Each element added to the list get the "Z-buffering" treatment, which means
 * that the objects farther are drawn first while the objects nearer are drawn 
 * last thus giving us the correct perspective.
 */

#include <assert.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#ifndef MOF_GRAPHICELEMENT_H_
#define MOF_GRAPHICELEMENT_H_

#define MOF_GRAPHICELEMENT_TYPE (1<<7)		/* dynamic type checking */

/**
 * mof_Graphicelement class.
 */ 
typedef struct mof_GraphicelementList {
  unsigned int type;
  double zIndex;
  int x;
  int y;
  int width;
  int height;
  int red;
  int green;
  int blue;
  int alpha;
  struct mof_GraphicelementList *first;
  struct mof_GraphicelementList *last;
  struct mof_GraphicelementList *current;
  struct mof_GraphicelementList *previous;
  struct mof_GraphicelementList *next;
} mof_Graphicelement;

/**
 * Constructor.
 *  
 * @param graphicelement Pointer to a mof_Graphicelement object.
 * @param z				 Z index for Z-buffering.
 * @param x              Coordinate of graphic element.
 * @param y              Coordinate of graphic element.
 * @param width          Width of graphic element.
 * @param height         Height of graphic element.
 * @param red			 Color of graphic element.
 * @param green			 Color of graphic element.
 * @param blue			 Color of graphic element.
 * @param alpha			 Color of graphic element.
 */
void mof_Graphicelement__construct(mof_Graphicelement *graphicelement, double z, int x, int y, int width, int height, int red, int green, int blue, int alpha)
{
  /* here OR the MOF_GRAPHICELEMENT_TYPE constant into the type */
  graphicelement->type |= MOF_GRAPHICELEMENT_TYPE;
  
  graphicelement->zIndex = z;
  graphicelement->x = x;
  graphicelement->y = y;
  graphicelement->width = width;
  graphicelement->height = height;
  graphicelement->red = red;
  graphicelement->green = green;
  graphicelement->blue = blue;
  graphicelement->alpha = alpha;
}

/**
 * New.
 * 
 * @param z		 Z index for Z-buffering.
 * @param x      Coordinate of graphic element.
 * @param y      Coordinate of graphic element.
 * @param width  Width of graphic element.
 * @param height Height of graphic element.
 * @param red	 Color of graphic element.
 * @param green	 Color of graphic element.
 * @param blue	 Color of graphic element.
 * @param alpha	 Color of graphic element.
 * @return       An object mof_Graphicelement.
 */
mof_Graphicelement *mof_Graphicelement__new(double z, int x, int y, int width, int height, int red, int green, int blue, int alpha) 
{	
  mof_Graphicelement *graphicelement = malloc(sizeof(mof_Graphicelement));
  graphicelement->type = MOF_GRAPHICELEMENT_TYPE;
  
  /* call the constructor */
  mof_Graphicelement__construct(graphicelement, z, x, y, width, height, red, green, blue, alpha);
  
  /* set pointer for previous and next element */
  graphicelement->first = graphicelement;
  graphicelement->last = graphicelement;
  graphicelement->current = graphicelement;
  graphicelement->previous = NULL;
  graphicelement->next = NULL;
  
  return graphicelement;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param graphicelement Pointer to a mof_Graphicelement object.
 */
void mof_Graphicelement__check(mof_Graphicelement *graphicelement)
{
  /* check if we have a valid mof_Graphicelement object */
  if (graphicelement == NULL || 
	  !(graphicelement->type & MOF_GRAPHICELEMENT_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param graphicelement Pointer to a mof_Graphicelement object.
 */ 
void mof_Graphicelement__destroy(mof_Graphicelement *graphicelement)
{
  /* check if we have a valid mof_Graphicelement object */
  mof_Graphicelement__check(graphicelement);

  /* set type to 0 indicate this is no longer a mof_Graphicelement object */
  graphicelement->type = 0;

  /* free the memory allocated for the object */
  mof_Graphicelement *fst = graphicelement->first;
  mof_Graphicelement *bkup = graphicelement->first;
  mof_Graphicelement *cur = NULL;
  for (cur = fst; cur != NULL; cur = bkup)
  {
	bkup = cur->next;
	free(cur);
  }
}

/**
 * Add a graphic element.
 * 
 * Add a graphic element to the list already existing.  Only the master
 * (the first created graphic element) can access this list.
 * 
 * @param master Pointer to a mof_Graphicelement object.
 * @param z		 Z index for Z-buffering.
 * @param x      Coordinate of graphic element.
 * @param y      Coordinate of graphic element.
 * @param width  Width of graphic element.
 * @param height Height of graphic element.
 * @param red	 Color of graphic element.
 * @param green	 Color of graphic element.
 * @param blue	 Color of graphic element.
 * @param alpha	 Color of graphic element.
 */
void mof_Graphicelement__add(mof_Graphicelement *master, double z, int x, int y, int width, int height, int red, int green, int blue, int alpha)
{
  /* check if we have a valid mof_Graphicelement object */
  mof_Graphicelement__check(master);
	
  mof_Graphicelement *graphicelement = malloc(sizeof(mof_Graphicelement));
  graphicelement->type = MOF_GRAPHICELEMENT_TYPE;
  
  /* call the constructor */
  mof_Graphicelement__construct(graphicelement, z, x, y, width, height, red, green, blue, alpha);
  
  /* do the Z-buffering stuff */
  master->current = master->first;
  while((master->current->next) != NULL)
  {
	if ((master->current->next->zIndex) < z)
	{
	  break;
	}
	master->current = master->current->next;
  }
  
  /* set pointer for current element */
  graphicelement->first = NULL;
  graphicelement->last = NULL;
  graphicelement->current = NULL;
  graphicelement->previous = master->current;
  if (master->current->next != NULL)
  {
	graphicelement->next = master->current->next;
  }
  else 
  {
	graphicelement->next = NULL;
	master->last = graphicelement;
  }
  
  /* set pointer for previous element */
  master->current->next = graphicelement;
  
  /* set current pointer to newly created element */
  master->current = graphicelement;
}

/**
 * Remove a graphic element.
 * 
 * Remove a graphic element to the list already existing (the current one).  
 * Only the master (the first created graphic element) can access this list.
 * 
 * @param master Pointer to a mof_Graphicelement object.
 */
void mof_Graphicelement__remove(mof_Graphicelement *master)
{ 
  /* check if we have a valid mof_Graphicelement object */
  mof_Graphicelement__check(master);
  
  /* never remove the first element (the master) */
  if (master->current->previous == NULL) 
    return;
  
  /* set pointer for previous element */
  master->current->previous->next = master->current->next;
  
  /* set pointer for next element*/
  if (master->current->next != NULL)
  {
    master->current->next->previous = master->current->previous;
  }
  
  /* set type to 0 indicate this is no longer a mof_Graphicelement object */
  master->current->type = 0;
  
  /* remove element */
  mof_Graphicelement *tmp = master->current;
  mof_Graphicelement *bkup = master->current->previous;
  free(tmp);
  master->current = bkup;
}

/**
 * Render the graphic element.
 * 
 * @param master Pointer to a mof_Graphicelement object.
 */
void mof_Graphicelement__render(SDL_Surface *screen, mof_Graphicelement *master)
{
  /* check if we have a valid mof_Graphicelement object */
  mof_Graphicelement__check(master);
  
  /* draw graphic element to the screen */
  mof_Graphicelement *fst = master->next;
  mof_Graphicelement *bkup = master->next;
  mof_Graphicelement *cur = NULL;
  for (cur = fst; cur != NULL; cur = bkup)
  {
	bkup = cur->next;
	master->current = cur;
	
	/* draw element */
	boxRGBA(screen, cur->x, cur->y, (cur->x + cur->width), (cur->y + cur->height), cur->red, cur->green, cur->blue, cur->alpha);
	
	mof_Graphicelement__remove(master);
  }
}

#endif