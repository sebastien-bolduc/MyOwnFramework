/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.00
 * @since 2012-02-08
 */

#include <assert.h>

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
 * @param first  Pointer to a mof_Graphicelement object.
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
void mof_Graphicelement__add(mof_Graphicelement *first, double z, int x, int y, int width, int height, int red, int green, int blue, int alpha)
{
  /* check if we have a valid mof_Graphicelement object */
  mof_Graphicelement__check(first);
	
  mof_Graphicelement *graphicelement = malloc(sizeof(mof_Graphicelement));
  graphicelement->type = MOF_GRAPHICELEMENT_TYPE;
  
  /* call the constructor */
  mof_Graphicelement__construct(graphicelement, z, x, y, width, height, red, green, blue, alpha);
  
  /* do the Z-buffering stuff */
  mof_Graphicelement *fst = first->first;
  mof_Graphicelement *bkup = first->first;
  mof_Graphicelement *cur = NULL;
  for (cur = fst; cur != NULL; cur = bkup)
  {
	bkup = cur->next;
	if (bkup != NULL)
	{
	  if ((bkup->zIndex) < z)
	  {
		first->current = cur;
		break;
	  }
	}
	else 
	{
	  first->current = cur;
	}
  }
  
  /* set pointer for current element */
  graphicelement->first = NULL;
  graphicelement->last = NULL;
  graphicelement->previous = first->current;
  if (first->current->next != NULL)
  {
	graphicelement->next = first->current->next;
  }
  else 
  {
	graphicelement->next = NULL;
	first->last = graphicelement;
  }
  
  /* set pointer for previous element */
  first->current->next = graphicelement;
  
  /* set current pointer to newly created element */
  first->current = graphicelement;
}

/**
 * Remove a graphic element.
 * 
 * Remove a graphic element to the list already existing.  Only the master
 * (the first created graphic element) can access this list.
 * 
 * @param first Pointer to a mof_Graphicelement object.
 */
void mof_Graphicelement__remove(mof_Graphicelement *first)
{ 
  /* check if we have a valid mof_Graphicelement object */
  mof_Graphicelement__check(first);
  
  /* never remove the first element */
  if (first->current->previous == NULL) 
    return;
  
  /* set pointer for previous element */
  first->current->previous->next = first->current->next;
  
  /* set pointer for next element*/
  if (first->current->next != NULL)
  {
    first->current->next->previous = first->current->previous;
  }
  
  /* set type to 0 indicate this is no longer a mof_Graphicelement object */
  first->current->type = 0;
  
  /* remove element */
  mof_Graphicelement *tmp = first->current;
  mof_Graphicelement *bkup = first->current->previous;
  free(tmp);
  first->current = bkup;
}

/**
 * Render the graphic element.
 * 
 * @param first Pointer to a mof_Graphicelement object.
 */
void mof_Graphicelement__render(SDL_Surface *screen, mof_Graphicelement *first)
{
  /* check if we have a valid mof_Graphicelement object */
  mof_Graphicelement__check(first);
  
  /* draw graphic element to the screen */
  mof_Graphicelement *fst = first->next;
  mof_Graphicelement *bkup = first->next;
  mof_Graphicelement *cur = NULL;
  for (cur = fst; cur != NULL; cur = bkup)
  {
	bkup = cur->next;
	first->current = cur;
	
	/* draw element */
	boxRGBA(screen, cur->x, cur->y, (cur->x + cur->width), (cur->y + cur->height), cur->red, cur->green, cur->blue, cur->alpha);
	
	mof_Graphicelement__remove(first);
  }
}

#endif