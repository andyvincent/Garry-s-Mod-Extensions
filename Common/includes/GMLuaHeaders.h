#ifndef _GMLUAHEADER_H
#define _GMLUAHEADER_H

#define GMOD_VERSION 2

#if GMOD_VERSION == 1
# include "v1/GMLuaModule.h"
// Gmod forward compatability so I don't have to re-write a whole lot of code...
# define Lua()           g_Lua
# define PushLong( X )   Push( static_cast<float>(X) )
# define PushNil()       Push( static_cast<ILuaObject*>(0) )
# define GMOD_FUNCTION( _function_ )  int _function_( void )
#elif GMOD_VERSION == 2
# define NO_SOURCE_SDK
# include "v2/GMLuaModule.h"
# define GMOD_FUNCTION( _function_ )  LUA_FUNCTION( _function_ )
#endif

#endif