//=============================================================================//
//  ___  ___   _   _   _    __   _   ___ ___ __ __
// |_ _|| __| / \ | \_/ |  / _| / \ | o \ o \\ V /
//  | | | _| | o || \_/ | ( |_n| o ||   /   / \ / 
//  |_| |___||_n_||_| |_|  \__/|_n_||_|\\_|\\ |_|  2006
//										 
//=============================================================================//

#ifndef ILUAINTERFACE_H
#define ILUAINTERFACE_H

#ifdef _WIN32
#pragma once
#endif

#include "ILuaObject.h"

typedef struct lua_State lua_State;
typedef int (*CLuaFunction) (lua_State*);
typedef void (*VoidFunction) ( void );

class LArgList;


namespace GLua
{
	enum
	{
		TYPE_INVALID = -1,
		TYPE_NIL, 
		TYPE_STRING, 
		TYPE_NUMBER, 
		TYPE_TABLE,
		TYPE_BOOL,
		TYPE_FUNCTION,
		TYPE_THREAD,

		// UserData
		TYPE_ENTITY, 
		TYPE_VECTOR, 
		TYPE_ANGLE,
		TYPE_PHYSOBJ,
		TYPE_SAVE,
		TYPE_RESTORE,
		TYPE_DAMAGEINFO,
		TYPE_EFFECTDATA,
		TYPE_MOVEDATA,
		TYPE_RECIPIENTFILTER,
		TYPE_USERCMD,
		TYPE_SCRIPTEDVEHICLE,

		// Client Only
		TYPE_MATERIAL,
		TYPE_PANEL,
		TYPE_PARTICLE,
		TYPE_PARTICLEEMITTER,
		TYPE_TEXTURE,
		TYPE_USERMSG,

		TYPE_COUNT
	};
}

class ILuaInterface 
{
	public:

		virtual int			GetIRef( void ) = 0;
		virtual bool		Init( void ) = 0;

		virtual void		Shutdown( void ) = 0;
		virtual void		Cycle( void ) = 0;
		virtual void*		GetLuaState() = 0;

		// Stack
		virtual void		Pop( int i=1 ) = 0;

		// Get
		virtual void		GetGlobal( ILuaObject* obj, const char* name ) = 0;
		virtual ILuaObject*	GetGlobal( const char* name ) = 0;

		virtual ILuaObject*	GetObject( int i = -1 ) = 0;
		virtual const char* GetString( int i = -1 ) = 0;
		virtual int			GetInteger( int i = -1 ) = 0;
		virtual float		GetNumber( int i = -1 ) = 0;
		virtual bool		GetBool( int i = -1 ) = 0;

		virtual void**		GetUserDataPtr( int i = -1 ) = 0;
		virtual void*		GetUserData( int i = -1 ) = 0;
		

		virtual void		GetTable( int i = -1 ) = 0;

		// References
		virtual int			GetReference( int i = -1, bool bPopValue = false ) = 0;
		virtual void		FreeReference( int i ) = 0;
		virtual void		PushReference( int i )	= 0;

		// Push
		virtual void		Push( ILuaObject* ) = 0;
		virtual void		Push( const char* str ) = 0;
		virtual void		PushVA( const char* str, ... ) = 0;
		virtual void		Push( float f ) = 0;
		virtual void		Push( bool b ) = 0;
		virtual void		Push( CLuaFunction f ) = 0;

		virtual void		SetGlobal( const char* namename, ILuaObject* obj = 0 ) = 0;
		virtual void		SetGlobal( const char* namename, bool b ) = 0;
		virtual void		SetGlobal( const char* namename, float f ) = 0;
		virtual void		SetGlobal( const char* namename, const char* s ) = 0;
		virtual void		SetGlobal( const char* namename, CLuaFunction f ) = 0;
		virtual void		NewTable( void ) = 0;

		virtual void			LuaError( const char*, int argument = -1 ) = 0;
		virtual void			TypeError( const char* name, int argument ) = 0;
		virtual int				GetType( int iStackPos ) = 0;
		virtual const char*		GetTypeName( int iType ) = 0;

		virtual bool			Call( int args, int returns = 0 ) = 0;
		virtual bool			Call( ILuaObject* func, LArgList* in, LArgList* out = 0 ) = 0;
		virtual bool			Call( ILuaObject* func, LArgList* in, ILuaObject* member ) = 0;

		virtual void			SetMember( ILuaObject* table, const char* name ) = 0;
		virtual void			SetMember( ILuaObject* table, const char* namename, ILuaObject* member ) = 0;

		virtual int				Top( void ) = 0;

		virtual ILuaObject*		NewUserData( ILuaObject* obj ) = 0;
		virtual void			PushUserData( ILuaObject* metatable, void * v ) = 0;

		virtual void			NewGlobalTable( const char* ) = 0;
		virtual ILuaObject*		NewTemporaryObject( void ) = 0;

		virtual bool			isUserData( int i = -1 ) = 0;

		// GetMetaTable creates the meta table if it doesn't exist. Make sure type is unique to your
		// meta type. The default types are defined above (TYPE_INVALID etc). You should ideally make your type
		// some random large number that is in the 1000's - to be sure that it won't conflict with the base
		// meta types when more are added at a later date. Name isn't so important - it's just used for GetTypeName
		// and in Lua using type().
		virtual ILuaObject*		GetMetaTable( const char* strName, int Type ) = 0;
		virtual ILuaObject*		GetMetaTable( int i ) = 0;
		virtual void			SetMetaTable( ILuaObject* obj, ILuaObject* metatable ) = 0;
		virtual void			CheckType( int i, int iType ) = 0;

		virtual ILuaObject*		GetReturn( int iNum ) = 0;

		// Added 10th December 2006
		virtual bool			IsServer( void ) = 0;
		virtual bool			IsClient( void ) = 0;
		virtual bool			IsDedicatedServer( void ) = 0;

		// Added 20th December 2006
		virtual void			SetMember( ILuaObject* table, float name ) = 0;
		virtual void			SetMember( ILuaObject* table, float namename, ILuaObject* member ) = 0;
		
		// Added 30th December 2006
		virtual ILuaObject*		GetNewTable( void ) = 0;		// Makes a new table and returns it

		// Added 09/Jan/2007
		virtual void			SetMember( ILuaObject* table, ILuaObject* key, ILuaObject* value ) = 0;

		// Added 12/01/2007
		virtual void			DebugPoint( void ) = 0;
};

ILuaInterface* CreateLuaInterface( void );
extern ILuaInterface* g_Lua;


// Gmod forward compatability so I don't have to re-write a whole lot of code...
#define Lua() g_Lua
#define PushLong( X )   Push( static_cast<float>(X) )

#endif // ILUAINTERFACE_H
