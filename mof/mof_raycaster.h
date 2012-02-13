/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.30
 * @since 2012-01-24
 * 
 * Raycasting using the method describe at that website:
 * {@link http://www.permadi.com/tutorial/raycast/} 
 */

#include <math.h>

#include "mof_graphicelement.h"
#include "mof_player.h"
#include "mof_map.h"

#ifndef MOF_RAYCASTER_H_
#define MOF_RAYCASTER_H_

/**
 * Checking the limit of the map.
 * 
 * @param map Pointer to a mof_Map object.
 * @param x   Coordinate of the point to check.
 * @param y   Coordinate of the point to check.
 * @return    True (1) if out of the limit of the map, false (0) otherwise.
 */
int mof_Raycaster__limit(mof_Map *map, double x, double y)
{
  if (x < 0 || x >= (map->width * map->unit))			/* width */
  {
	return 1;
  }
  else if (y < 0 || y >= (map->height * map->unit))		/* height */
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

  /* escaping the case that screw thing up! */
  if (angle == 135)
	angle += 0.5;

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
  int flag = 0;
  
  Px = ((mof_Avatar *)player)->x;
  Py = ((mof_Avatar *)player)->y;
  
  if (angle >= 0 && angle <= 180)
  {
	Ynew = floor(Py / map->unit) * map->unit;// - 1;
	flag = 1;											/* correct 'hack' for wall orientation */
  }
  else
	Ynew = floor(Py / map->unit) * map->unit + map->unit;
    
  if (angle == 90 || angle == 270)
    Xnew = Px;
  else
	Xnew = (Px + ((Py - Ynew) / tan(angle * M_PI / 180)));
	
  /* checking to see if we are not out of bound */
  if (mof_Raycaster__limit(map, Xnew, Ynew))
  {
	resultH[0] = -1;
	return resultH;  
  }
  
  /* find Ya */
  double Ya = 0;
        
  if (angle >= 0 && angle <= 180)
	Ya = -(map->unit);
  else
	Ya = map->unit;
	
  /* find Xa */
  double Xa = 0;
    
  if (angle == 90 || angle == 270)
	Xa = 0;
  else if (angle > 180)
	Xa = -((map->unit / tan(angle * M_PI / 180)));
  else
	Xa = (map->unit / tan(angle * M_PI / 180));

  /* check the grid at the intersection point for wall */
  while (map->map[(int)(floor(Xnew / map->unit) + ((flag) ? floor((Ynew - 1) / map->unit) : floor(Ynew / map->unit)) * map->width)] != 1)
  {   
	Ynew += Ya;
	Xnew += Xa;
        
	/* checking to see if we are not out of bound */
	if (mof_Raycaster__limit(map, Xnew, Ynew))
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
  
  /* escaping the case that screw thing up! */
  if (angle == 135)
	angle += 0.5;
  
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
  int flag = 0;
  
  Px = ((mof_Avatar *)player)->x;
  Py = ((mof_Avatar *)player)->y;
  
  if (angle > 90 && angle < 270)
  {
	Xnew = floor(Px / map->unit) * map->unit;// - 1;
	flag = 1;											/* correct 'hack' for wall orientation */
  }
  else
	Xnew = floor(Px / map->unit) * map->unit + map->unit;
    
  Ynew = (Py + ((Px - Xnew) * tan(angle * M_PI / 180)));
  
  /* checking to see if we are not out of bound */
  if (mof_Raycaster__limit(map, Xnew, Ynew))
  {
	resultV[0] = -1;
	return resultV;  
  }
  
  /* find Xa */
  double Xa = 0;
    
  if (angle > 90 && angle < 270)
	Xa = -(map->unit);
  else
	Xa = map->unit;
    
  /* find Ya */
  double Ya = 0;
  
  if (angle > 90 && angle < 270)
	Ya = (map->unit * tan(angle * M_PI / 180));
  else
	Ya = -((map->unit * tan(angle * M_PI / 180)));

  /* check the grid at the intersection point for wall */
  while (map->map[(int)(((flag) ? floor((Xnew - 1) / map->unit) : floor(Xnew / map->unit)) + floor(Ynew / map->unit) * map->width)] != 1)
  {   
	Ynew += Ya;
	Xnew += Xa;
        
	/* checking to see if we are not out of bound */
	if (mof_Raycaster__limit(map, Xnew, Ynew))
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
  for (i = 30; i >= -30; i -= 0.9375)
  {
    resultH = mof_Raycaster__horizontal(player, map, (double)((mof_Avatar *)player)->angle + i);
	resultV = mof_Raycaster__vertical(player, map, (double)((mof_Avatar *)player)->angle + i);
	
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

/**
 * Drawing the rays casted (3D).
 * 
 * @param scene  Pointer to a mof_Graphicelement object.
 * @param player Pointer to a mof_Player object.
 * @param map    Pointer to a mof_Map object.
 */
mof_Raycaster__draw3Dscene(mof_Graphicelement *scene, mof_Player *player, mof_Map *map)
{
  double *resultH, *resultV, *result;
  double i = 0;
  double distanceFromProjectionPlane = (640 / 2) / tan((60 / 2) * M_PI / 180);
  int bottom, top, position = 0;
  double orientation = 0;
  double zIndex = 0;
  
  mof_Graphicelement__add(scene, 10000.0, 0, 0, 640, 240, 106, 106, 106, 255);
  mof_Graphicelement__add(scene, 10000.0, 0, 240, 640, 480, 40, 40, 40 ,255);
  
  for (i = 30; i >= -30; i -= 0.9375)
  {
    resultH = mof_Raycaster__horizontal(player, map, (double)((mof_Avatar *)player)->angle + i);
	resultV = mof_Raycaster__vertical(player, map, (double)((mof_Avatar *)player)->angle + i);
	
	if (resultH[0] < 0)
	{
	  result = resultV;
	  orientation = 1;
	}
	else if (resultV[0] < 0)
	{
	  result = resultH;
	  orientation = 0;
	}
	else if (resultH[0] < resultV[0])
	{
	  result = resultH;
	  orientation = 0;
	}
	else
	{
	  result = resultV;
	  orientation = 1;
	}
	
	/* Z-buffering */
	zIndex = result[0];
	
	/* remove the viewing distortion */
    result[0] = result[0] * fabs(cos(i * M_PI / 180));
	
	/* get top and bottom of wall */
	bottom = (int)floor(32 * distanceFromProjectionPlane / result[0] + 240);
    top = (int)floor((32 - 64) * distanceFromProjectionPlane / result[0] + 240);

	/* draw wall slice */
	if (orientation)
	  mof_Graphicelement__add(scene, zIndex, position, top, 9, (bottom - top), 185 - (result[0] * 0.2), 0, 0, 255);
	else
	  mof_Graphicelement__add(scene, zIndex, position, top, 9, (bottom - top), 255 - (result[0] * 0.2), 0, 0, 255);
	
	position += 10;
  }
}

#endif