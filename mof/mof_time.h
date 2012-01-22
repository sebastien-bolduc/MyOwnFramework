/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.10
 * @since 2012-01-12
 */

#include <sys/time.h>
#include <assert.h>
#include <stdlib.h>

#ifndef MOF_TIME_H_
#define MOF_TIME_H_

#define MOF_TIME_TYPE (1<<0)		/* dynamic type checking */

/**
 * mof_Time class.
 */ 
typedef struct {
  unsigned int type;
  struct timeval start;		/* Start timing */
  struct timeval stop;		/* Stop timing */
} mof_Time;

/**
 * Constructor.
 * 
 * @param time Pointer to a mof_Time object.
 */
void mof_Time__construct(mof_Time *time)
{
  /* here OR the MOF_TIME_TYPE constant into the type */
  time->type |= MOF_TIME_TYPE;
   
  gettimeofday(&time->start, NULL);
  gettimeofday(&time->stop, NULL);
}

/**
 * New.
 * 
 * @return An object mof_Time.
 */
mof_Time *mof_Time__new(void) 
{	
  mof_Time *time = malloc(sizeof(mof_Time));
  time->type = MOF_TIME_TYPE;
  
  /* call the constructor */
  mof_Time__construct(time);
  
  return time;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param time Pointer to a mof_Time object.
 */
void mof_Time__check(mof_Time *time)
{
  /* check if we have a valid mof_Time object */
  if (time == NULL || 
	  !(time->type & MOF_TIME_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param time Pointer to a mof_Time object.
 */ 
void mof_Time__destroy(mof_Time *time)
{
  /* check if we have a valid mof_Time object */
  mof_Time__check(time);

  /* set type to 0 indicate this is no longer a mof_Time object */
  time->type = 0;

  /* free the memory allocated for the object */
  free(time);
}

/**
 * Return time in microseconds.
 * 
 * @param time Pointer to a mof_Time object.
 */
long long mof_Time__gettime_usec(mof_Time *time)
{
  /* check if we have a valid mof_Time object */
  mof_Time__check(time);	

  struct timeval result;
  timersub(&time->stop, &time->start, &result);
  return result.tv_usec;
}

/**
 * Return time in milliseconds.
 * 
 * @param time Pointer to a mof_Time object.
 */
long long mof_Time__gettime_msec(mof_Time *time)
{
  /* check if we have a valid mof_Time object */
  mof_Time__check(time);	

  struct timeval result;
  timersub(&time->stop, &time->start, &result);
  return result.tv_usec / 1000;
}

/**
 * Return elapsed time in microseconds.
 * 
 * @param time Pointer to a mof_Time object.
 */
long long mof_Time__gettime_elapsed_usec(mof_Time *time)
{
  /* check if we have a valid mof_Time object */
  mof_Time__check(time);
  
  struct timeval now;
  struct timeval result;
  gettimeofday(&now, NULL);
  timersub(&now, &time->start, &result);
  return result.tv_usec;
}

/**
 * Return elapsed time in milliseconds.
 * 
 * @param time Pointer to a mof_Time object.
 */
long long mof_Time__gettime_elapsed_msec(mof_Time *time)
{
  /* check if we have a valid mof_Time object */
  mof_Time__check(time);
  
  struct timeval now;
  struct timeval result;
  gettimeofday(&now, NULL);
  timersub(&now, &time->start, &result);
  return result.tv_usec / 1000;
}

/**
 * Start timing.
 * 
 * @param time Pointer to a mof_Time object.
 */ 
void mof_Time__start(mof_Time *time)
{
  /* check if we have a valid mof_Time object */
  mof_Time__check(time);
	
  gettimeofday(&time->start, NULL);
}

/**
 * Stop timing.
 * 
 * @param time Pointer to a mof_Time object.
 */ 
void mof_Time__stop(mof_Time *time)
{
  /* check if we have a valid mof_Time object */
  mof_Time__check(time);

  gettimeofday(&time->stop, NULL);
}

#endif