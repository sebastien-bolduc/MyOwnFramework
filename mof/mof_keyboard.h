/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.00
 * @since 2012-01-16
 */
 
#ifndef MOF_KEYBOARD_H_
#define MOF_KEYBOARD_H_

int MOF_KEYBOARD__SDLK_UP_FLAG = 0;
int MOF_KEYBOARD__SDLK_DOWN_FLAG = 0;
int MOF_KEYBOARD__SDLK_RIGHT_FLAG = 0;
int MOF_KEYBOARD__SDLK_LEFT_FLAG = 0;

/**
 * Set the flag of a keyboard's key.
 * 
 * @param key   Key stroke on the keyboard.
 * @param value Value to affecte to the flag.
 */
void mof_Keyboard__setflag(SDLKey key, int value)
{
  switch (key)
  {
	case SDLK_UP:
	  MOF_KEYBOARD__SDLK_UP_FLAG = value;
	  break;
	case SDLK_DOWN:
	  MOF_KEYBOARD__SDLK_DOWN_FLAG = value;
	  break;
	case SDLK_RIGHT:
	  MOF_KEYBOARD__SDLK_RIGHT_FLAG = value;
	  break;
	case SDLK_LEFT:
	  MOF_KEYBOARD__SDLK_LEFT_FLAG = value;
	  break;
	default:
	  break;
  }
}

/**
 * Poll event related to keyboard.
 * 
 * Poll for event happening with the keyboard and set 'off' or
 * 'on' the flag for the key stroke.
 * 
 * @param event SDL_Event typedef.
 */
void mof_Keyboard__pollevent(SDL_Event *event)
{
  if (event->type == SDL_KEYDOWN)
  {
	mof_Keyboard__setflag(event->key.keysym.sym, 1);
  }
  if (event->type == SDL_KEYUP)
  {
	mof_Keyboard__setflag(event->key.keysym.sym, 0);
  }
}

/**
 * Check the flag of a key.
 * 
 * @param key Key to check for.
 * @return    Value of the key's flag.
 */
int mof_Keyboard__checkkey(SDLKey key)
{
  switch (key)
  {
	case SDLK_UP:
	  return MOF_KEYBOARD__SDLK_UP_FLAG;
	  break;
	case SDLK_DOWN:
	  return MOF_KEYBOARD__SDLK_DOWN_FLAG;
	  break;
	case SDLK_RIGHT:
	  return MOF_KEYBOARD__SDLK_RIGHT_FLAG;
	  break;
	case SDLK_LEFT:
	  return MOF_KEYBOARD__SDLK_LEFT_FLAG;
	  break;
	default:
	  return 0;
	  break;
  }
}

#endif