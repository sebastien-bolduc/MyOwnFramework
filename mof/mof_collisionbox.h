/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.10
 * @since 2012-01-17
 */

#include <assert.h>

#ifndef MOF_COLLISIONBOX_H_
#define MOF_COLLISIONBOX_H_

#define MOF_COLLISIONBOX_TYPE (1<<4)		/* dynamic type checking */

/**
 * mof_Collisionbox class.
 */ 
typedef struct mof_CollisionboxList {
  unsigned int type;
  int x;
  int y;
  int width;
  int height;
  struct mof_CollisionboxList *first;
  struct mof_CollisionboxList *last;
  struct mof_CollisionboxList *current;
  struct mof_CollisionboxList *previous;
  struct mof_CollisionboxList *next;
} mof_Collisionbox;

/**
 * Constructor.
 *  
 * @param collisionbox Pointer to a mof_Collisionbox object.
 * @param x            Coordinate of collision box.
 * @param y            Coordinate of collision box.
 * @param width        Width of collision box.
 * @param height       Height of collision box.
 */
void mof_Collisionbox__construct(mof_Collisionbox *collisionbox, int x, int y, int width, int height)
{
  /* here OR the MOF_COLLISIONBOX_TYPE constant into the type */
  collisionbox->type |= MOF_COLLISIONBOX_TYPE;
   
  collisionbox->x = x;
  collisionbox->y = y;
  collisionbox->width = width;
  collisionbox->height = height;
}

/**
 * New.
 * 
 * @param x      Coordinate of collision box.
 * @param y      Coordinate of collision box.
 * @param width  Width of collision box.
 * @param height Height of collision box.
 * @return       An object mof_Collisionbox.
 */
mof_Collisionbox *mof_Collisionbox__new(int x, int y, int width, int height) 
{	
  mof_Collisionbox *collisionbox = malloc(sizeof(mof_Collisionbox));
  collisionbox->type = MOF_COLLISIONBOX_TYPE;
  
  /* call the constructor */
  mof_Collisionbox__construct(collisionbox, x, y, width, height);
  
  /* set pointer for previous and next element */
  collisionbox->first = collisionbox;
  collisionbox->last = collisionbox;
  collisionbox->current = collisionbox;
  collisionbox->previous = NULL;
  collisionbox->next = NULL;
  
  return collisionbox;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param collisionbox Pointer to a mof_Collisionbox object.
 */
void mof_Collisionbox__check(mof_Collisionbox *collisionbox)
{
  /* check if we have a valid mof_Collisionbox object */
  if (collisionbox == NULL || 
	  !(collisionbox->type & MOF_COLLISIONBOX_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param collisionbox Pointer to a mof_Collisionbox object.
 */ 
void mof_Collisionbox__destroy(mof_Collisionbox *collisionbox)
{
  /* check if we have a valid mof_Collisionbox object */
  mof_Collisionbox__check(collisionbox);

  /* set type to 0 indicate this is no longer a mof_Collisionbox object */
  collisionbox->type = 0;

  /* free the memory allocated for the object */
  mof_Collisionbox *fst = collisionbox->first;
  mof_Collisionbox *bkup = collisionbox->first;
  mof_Collisionbox *cur = NULL;
  for (cur = fst; cur != NULL; cur = bkup)
  {
	bkup = cur->next;
	free(cur);
  }
}

/**
 * Add a collision box.
 * 
 * Add a collision box to the list already existing.  Only the master
 * (the first created collision box) can access this list.
 * 
 * @param first  Pointer to a mof_Collisionbox object.
 * @param x      Coordinate of collision box.
 * @param y      Coordinate of collision box.
 * @param width  Width of collision box.
 * @param height Height of collision box.
 */
mof_Collisionbox__add(mof_Collisionbox *first, int x, int y, int width, int height)
{
  /* check if we have a valid mof_Collisionbox object */
  mof_Collisionbox__check(first);
	
  mof_Collisionbox *collisionbox = malloc(sizeof(mof_Collisionbox));
  collisionbox->type = MOF_COLLISIONBOX_TYPE;
  
  /* call the constructor */
  mof_Collisionbox__construct(collisionbox, x, y, width, height);
  
  /* set pointer for current element */
  collisionbox->first = NULL;
  collisionbox->last = NULL;
  collisionbox->previous = first->current;
  if (first->current->next != NULL)
  {
	collisionbox->next = first->current->next;
  }
  else 
  {
	collisionbox->next = NULL;
	first->last = collisionbox;
  }
  
  /* set pointer for previous element */
  first->current->next = collisionbox;
  
  /* set current pointer to newly created element */
  first->current = collisionbox;
}

/**
 * Remove a collision box.
 * 
 * Remove a collision box to the list already existing.  Only the master
 * (the first created collision box) can access this list.
 * 
 * @param first Pointer to a mof_Collisionbox object.
 */
void mof_Collisionbox__remove(mof_Collisionbox *first)
{ 
  /* check if we have a valid mof_Collisionbox object */
  mof_Collisionbox__check(first);
  
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
  
  /* set type to 0 indicate this is no longer a mof_Collisionbox object */
  first->current->type = 0;
  
  /* remove element */
  mof_Collisionbox *tmp = first->current;
  mof_Collisionbox *bkup = first->current->previous;
  free(tmp);
  first->current = bkup;
}

/**
 * Move collision box.
 * 
 * @param collisionbox Pointer to a mof_Collisionbox object.
 * @param x            New coordinate of collision box.
 * @param y 		   New coordinate of collision box.
 */
void mof_Collisionbox__move(mof_Collisionbox *collisionbox, int x, int y)
{
  /* check if we have a valid mof_Collisionbox object */
  mof_Collisionbox__check(collisionbox);
  
  collisionbox->x = x;
  collisionbox->y = y;
}

/**
 * Check for collision.
 * 
 * @param collisionbox1 Pointer to a mof_Collisionbox object.
 * @param collisionbox2 Pointer to a mof_Collisionbox object.
 * @return              True (1) for a collision, false (0) otherwise.
 */
int mof_Collisionbox__intersect(mof_Collisionbox *collisionbox1, mof_Collisionbox *collisionbox2)
{
  /* check if we have a valid mof_Collisionbox object */
  mof_Collisionbox__check(collisionbox1);
  mof_Collisionbox__check(collisionbox2);
	
  int _min = collisionbox1->x;
  int _max = collisionbox2->x + collisionbox2->width - 1;
  
  if (_min > _max)
	return 0;
	  
  int min_ = collisionbox2->x;
  int max_ = collisionbox1->x + collisionbox1->width - 1;
  
  if (min_ > max_)
	return 0;
	
  _min = collisionbox1->y;
  _max = collisionbox2->y + collisionbox2->height - 1;
  
  if (_min > _max)
	return 0;

  min_ = collisionbox2->y;
  max_ = collisionbox1->y + collisionbox1->height - 1;
  
  if (min_ > max_)
	return 0;
	
  return 1;
}

#endif