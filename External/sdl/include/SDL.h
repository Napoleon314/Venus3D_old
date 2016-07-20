/*
Simple DirectMedia Layer
Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

/**
*  \file SDL.h
*
*  Main include header for the SDL library
*/

#ifndef _SDL_H
#define _SDL_H

#include "SDL_stdinc.h"

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/* As of version 0.5, SDL is loaded dynamically into the application */

/**
*  \name SDL_INIT_*
*
*  These are the flags which may be passed to SDL_Init().  You should
*  specify the subsystems which you will be using in your application.
*/
/* @{ */
#define SDL_INIT_TIMER          0x00000001
#define SDL_INIT_AUDIO          0x00000010
#define SDL_INIT_VIDEO          0x00000020  /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
#define SDL_INIT_JOYSTICK       0x00000200  /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
#define SDL_INIT_HAPTIC         0x00001000
#define SDL_INIT_GAMECONTROLLER 0x00002000  /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
#define SDL_INIT_EVENTS         0x00004000
#define SDL_INIT_NOPARACHUTE    0x00100000  /**< compatibility; this flag is ignored. */
#define SDL_INIT_EVERYTHING ( \
                SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | \
                SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER \
            )
/* @} */

/**
*  This function initializes  the subsystems specified by \c flags
*/
extern SDL_API int SDLCALL SDL_Init(Uint32 flags);


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif

#endif /* _SDL_H */
