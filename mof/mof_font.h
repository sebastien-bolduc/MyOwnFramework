/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.00
 * @since 2012-01-15
 */

#include <assert.h>
#include "SDL.h"
#include "SDL_ttf.h"

#ifndef MOF_FONT_H_
#define MOF_FONT_H_

#define MOF_FONT_TYPE (1<<1)		/* dynamic type checking */

/**
 * mof_Font class.
 */ 
typedef struct {
  unsigned int type;
  SDL_Surface *fontSurface;
  SDL_Color fColor;
  SDL_Rect fontRect;
  TTF_Font *font;
  SDL_Surface *screen;		/* copy of the current SDL surface */
} mof_Font;

/**
 * Load a font.
 * 
 * @param path Path to the 'true type font' to use.
 * @return     A 'True Type Font' pointer.
 */
TTF_Font *mof_Font__loadfont(const char *path)
{
	return TTF_OpenFont(path, 12);
}

/**
 * Constructor.
 * 
 * Initialize the font, set to white.
 * 
 * @param font Pointer to a mof_Font object.
 * @param path Path to the 'true type font' to use.
 */
void mof_Font__construct(mof_Font *font, const char *path){
  /* here OR the MOF_FONT_TYPE constant into the type */
  font->type |= MOF_FONT_TYPE;
	
  /* check to see if TTF was initialized, if not do it... */
  if(!TTF_WasInit())
  {
	TTF_Init();
  }
		
  font->font = mof_Font__loadfont(path);
  font->fColor.r = 255;
  font->fColor.g = 255;
  font->fColor.b = 255;
}

/**
 * New.
 * 
 * @param screen A copy of the current SDL surface.
 * @param path   Path to the 'true type font' to use.
 * @return       An object mof_Font.
 */
mof_Font *mof_Font__new(SDL_Surface *screen, const char *path) 
{	
  mof_Font *font = malloc(sizeof(mof_Font));
  font->type = MOF_FONT_TYPE;
  font->screen = screen;
  
  /* call the constructor */
  mof_Font__construct(font, path);
  
  return font;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param font Pointer to a mof_Font object.
 */
void mof_Font__check(mof_Font *font)
{
  /* check if we have a valid mof_Font object */
  if (font == NULL || 
	  !(font->type & MOF_FONT_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param font Pointer to a mof_Font object.
 */ 
void mof_Font__destroy(mof_Font *font)
{
  /* check if we have a valid mof_Font object */
  mof_Font__check(font);

  /* set type to 0 indicate this is no longer a mof_Font object */
  font->type = 0;

  /* free the memory allocated for the object */
  free(font);
}

/**
 * Print the designated string at the specified coordinates.
 * 
 * @param font Pointer to a mof_Font object.
 * @param c    Text to render.
 * @param x    At width.
 * @param y    At height.
 */
void mof_Font__printf(mof_Font *font, char *c, int x, int y){
  /* check if we have a valid mof_Font object */
  mof_Font__check(font);
	
  font->fontSurface = TTF_RenderText_Solid(font->font, c, font->fColor);
  font->fontRect.x = x;
  font->fontRect.y = y;
  SDL_BlitSurface(font->fontSurface, NULL, font->screen, &font->fontRect);
  
  /* avoid memory leakage (should think of a bettet solution) */
  SDL_FreeSurface(font->fontSurface);
}

#endif