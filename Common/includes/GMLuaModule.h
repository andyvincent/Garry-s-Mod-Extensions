//=============================================================================//
//  ___  ___   _   _   _    __   _   ___ ___ __ __
// |_ _|| __| / \ | \_/ |  / _| / \ | o \ o \\ V /
//  | | | _| | o || \_/ | ( |_n| o ||   /   / \ / 
//  |_| |___||_n_||_| |_|  \__/|_n_||_|\\_|\\ |_|  2008
//										 
//=============================================================================//
#ifndef _GMLUAMODULE_H
#define _GMLUAMODULE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef WIN32
#define GMOD_EXPORT __declspec(dllexport)
#elif LINUX
#define GMOD_EXPORT __attribute__ ((visibility("default")))
#else
#error Unhandled Platform!
#endif

#include "ILuaInterface.h"
#include "ILuaObject.h"

#include "ILuaModuleManager.h"


// You should place this at the top of your module
#define GMOD_MODULE( _startfunction_, _closefunction_ ) \
	ILuaModuleManager* modulemanager = NULL;\
	int _startfunction_( lua_State* L );\
	int _closefunction_( lua_State* L );\
	extern "C" int GMOD_EXPORT gmod_open( ILuaInterface* i ) \
	{ \
		modulemanager = i->GetModuleManager();\
		return _startfunction_( (lua_State*)(i->GetLuaState()) );\
	}\
	extern "C" int GMOD_EXPORT gmod_close( lua_State* L ) \
	{\
		_closefunction_( L );\
		return 0;\
	}\

#define LUA_FUNCTION( _function_ ) static int _function_( lua_State* L )

#endif // _GMLUAMODULE_H
