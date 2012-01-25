/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.00
 * @since 2012-01-24
 */

#include <math.h>

#include "mof_player.h"
#include "mof_map.h"

#ifndef MOF_RAYCASTER_H_
#define MOF_RAYCASTER_H_

/**
 * Checking the limit of the map.
 * 
 * @param x Coordinate of the point to check.
 * @param y Coordinate of the point to check.
 * @return  True (1) if out of the limit of the map, false (0) otherwise.
 */
int mof_Raycaster__limit(double x, double y)
{
  if (x < 0 || x >= (10 * 64))			/* width */
  {
	return 1;
  }
  else if (y < 0 || y >= (10 * 64))		/* height */
  {
	return 1;
  }
  else 
  {
	return 0;
  }
}

/**
 * Horizontal intersection for the ray.
 * 
 * @param player Pointer to a mof_Player object.
 * @param map    Pointer to a mof_Map object.
 * @param angle  Angle of the ray casted.
 * @return       Distance and point of intersection.
 */
double *mof_Raycaster__horizontal(mof_Player *player, mof_Map *map, double angle)
{
  static double resultH[3] = {0, 0, 0};	
	
  if (angle == 0 || angle == 180)
  {
	resultH[0] = -1;
	return resultH;  
  }
	
  /* angle correction */
  if (angle < 0)
	angle += 360;
  else if (angle > 360)
	angle -= 360;
	
  /* find coordinate of the first intersection */
  double Px = 0;
  double Py = 0;
  double Xnew = 0;
  double Ynew = 0;
  
  Px = ((mof_Avatar *)player)->x;
  Py = ((mof_Avatar *)player)->y;
  
  if (angle >= 0 && angle <= 180)
	Ynew = floor(Py / 64) * 64 - 1;
  else
	Ynew = floor(Py / 64) * 64 + 64;
    
  if (angle == 90 || angle == 270)
    Xnew = Px;
  else
	Xnew = floor(Px + ((Py - Ynew) / tan(angle * M_PI / 180)));
	
  /* checking to see if we are not out of bound */
  if (mof_Raycaster__limit(Xnew, Ynew))
  {
	resultH[0] = -1;
	return resultH;  
  }
  
  /* find Ya */
  double Ya = 0;
        
  if (angle >= 0 && angle <= 180)
	Ya = -(64);
  else
	Ya = 64;
	
  /* find Xa */
  double Xa = 0;
    
  if (angle == 90 || angle == 270)
	Xa = 0;
  else if (angle >= 0 && angle < 90)				/* correction for ray to not cross wall */
	Xa = ceil(64 / tan(angle * M_PI / 180));
  else if (angle >= 90 && angle < 180)
	Xa = floor(64 / tan(angle * M_PI / 180));
  else if (angle >= 180 && angle < 270)
	Xa = -(ceil(64 / tan(angle * M_PI / 180)));
  else if (angle >= 270 && angle < 360)
	Xa = -(floor(64 / tan(angle * M_PI / 180)));
  /*else if (angle > 180)
	Xa = -(floor(64 / tan(angle * M_PI / 180)));
  else
	Xa = floor(64 / tan(angle * M_PI / 180));*/
	
  /* check the grid at the intersection point for wall */
  while (map->map[(int)(floor(Xnew / 64) + floor(Ynew / 64) * 10)] != 1)
  {   
	Ynew += Ya;
	Xnew += Xa;
        
	/* checking to see if we are not out of bound */
	if (mof_Raycaster__limit(Xnew, Ynew))
    {
	  resultH[0] = -1;
	  return resultH; 
    }
  }
  
  resultH[0] = sqrt(pow((Px - Xnew), 2) + pow((Py - Ynew), 2));
  resultH[1] = Xnew;
  resultH[2] = Ynew;
  
  return resultH;
}

/**
 * Vertical intersection for the ray.
 * 
 * @param player Pointer to a mof_Player object.
 * @param map    Pointer to a mof_Map object.
 * @param angle  Angle of the ray casted.
 * @return       Distance and point of intersection.
 */
double *mof_Raycaster__vertical(mof_Player *player, mof_Map *map, double angle)
{
  static double resultV[3] = {0, 0, 0};	
  
  if (angle == 90 || angle == 270)
  {
	resultV[0] = -1;
	return resultV;  
  }
	
  /* angle correction */
  if (angle < 0)
	angle += 360;
  else if (angle > 360)
	angle -= 360;
	
  /* find coordinate of the first intersection */
  double Px = 0;
  double Py = 0;
  double Xnew = 0;
  double Ynew = 0;
  
  Px = ((mof_Avatar *)player)->x;
  Py = ((mof_Avatar *)player)->y;
  
  if (angle > 90 && angle < 270)
	Xnew = floor(Px / 64) * 64 - 1;
  else
	Xnew = floor(Px / 64) * 64 + 64;
    
  Ynew = floor(Py + ((Px - Xnew) * tan(angle * M_PI / 180)));
  
  /* checking to see if we are not out of bound */
  if (mof_Raycaster__limit(Xnew, Ynew))
  {
	resultV[0] = -1;
	return resultV;  
  }
  
  /* find Xa */
  double Xa = 0;
    
  if (angle > 90 && angle < 270)
	Xa = -(64);
  else
	Xa = 64;
    
  /* find Ya */
  double Ya = 0;
  
  if (angle >= 0 && angle < 90)						/* correction for ray to not cross wall */
	Ya = -(ceil(64 * tan(angle * M_PI / 180)));
  else if (angle >= 90 && angle < 180)
	Ya = floor(64 * tan(angle * M_PI / 180));
  else if (angle >= 180 && angle < 270)
	Ya = ceil(64 * tan(angle * M_PI / 180));
  else if (angle >= 270 && angle < 360)
	Ya = -(floor(64 * tan(angle * M_PI / 180)));
  /*if (angle > 90 && angle < 270)
	Ya = floor(64 * tan(angle * M_PI / 180));
  else
	Ya = -(floor(64 * tan(angle * M_PI / 180)));*/
	
  /* check the grid at the intersection point for wall */
  while (map->map[(int)(floor(Xnew / 64) + floor(Ynew / 64) * 10)] != 1)
  {   
	Ynew += Ya;
	Xnew += Xa;
        
	/* checking to see if we are not out of bound */
	if (mof_Raycaster__limit(Xnew, Ynew))
    {
	  resultV[0] = -1;
	  return resultV; 
    }
  }
  
  resultV[0] = sqrt(pow((Px - Xnew), 2) + pow((Py - Ynew), 2));
  resultV[1] = Xnew;
  resultV[2] = Ynew;
  
  return resultV;
}

/**
 * Drawing the rays casted.
 * 
 * @param player  Pointer to a mof_Player object.
 * @param map     Pointer to a mof_Map object.
 * @param offsetX Offset for the X coordinate.
 * @param offsetY Offset for the Y coordinate.
 */
mof_Raycaster__draw(mof_Player *player, mof_Map *map, int offsetX, int offsetY)
{
  double *resultH, *resultV, *result;
  double i = 0;
  for (i = 30; i >= -30; i -= 1)
  {
    resultH = mof_Raycaster__horizontal(player, map, 180 - (double)((mof_Avatar *)player)->angle + i);
	resultV = mof_Raycaster__vertical(player, map, 180 - (double)((mof_Avatar *)player)->angle + i);
	
	if (resultH[0] < 0)
	  result = resultV;
	else if (resultV[0] < 0)
	  result = resultH;
	else if (resultH[0] < resultV[0])
	  result = resultH;
	else
	  result = resultV;
    
	lineRGBA(player->screen, (int)((mof_Avatar *)player)->x - offsetX, (int)((mof_Avatar *)player)->y - offsetY, (int)result[1] - offsetX, (int)result[2] - offsetY, 255, 255, 0, 50);
  }
}

#endif