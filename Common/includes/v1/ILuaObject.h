//=============================================================================//
//  ___  ___   _   _   _    __   _   ___ ___ __ __
// |_ _|| __| / \ | \_/ |  / _| / \ | o \ o \\ V /
//  | | | _| | o || \_/ | ( |_n| o ||   /   / \ / 
//  |_| |___||_n_||_| |_|  \__/|_n_||_|\\_|\\ |_|  2006
//										 
//=============================================================================//

#ifndef ILUAOBJECT_H
#define ILUAOBJECT_H

#ifdef _WIN32
#pragma once
#endif

typedef struct lua_State lua_State;
typedef int (*CLuaFunction) (lua_State*);

#define SAFE_UNREF( _obj_ ) if ( _obj_ ){ _obj_->UnReference(); _obj_ = NULL; } 

class ILuaObject 
{
	public:

		virtual void	Set( ILuaObject* obj ) = 0;
		virtual void	SetFromStack( int i ) = 0;
		virtual void	UnReference() = 0;

		virtual int				GetType( void ) = 0;

		virtual const char*		GetString( void ) = 0;
		virtual float			GetFloat( void ) = 0;
		virtual int				GetInt( void ) = 0;
		virtual void*			GetUserData( void ) = 0;

		virtual void			SetMember( const char* name ) = 0;
		virtual void			SetMember( const char* name, ILuaObject* obj ) = 0; // ( This is also used to set nil by passing NULL )
		virtual void			SetMember( const char* name, float val ) = 0;
		virtual void			SetMember( const char* name, bool val ) = 0;
		virtual void			SetMember( const char* name, const char* val ) = 0;
		virtual void			SetMember( const char* name, CLuaFunction f ) = 0;

		virtual bool			GetMemberBool( const char* name, bool b = true ) = 0;
		virtual int				GetMemberInt( const char* name, int i = 0 ) = 0;
		virtual float			GetMemberFloat( const char* name, float f = 0.0f ) = 0;
		virtual const char*		GetMemberStr( const char* name, const char* = "" ) = 0;
		virtual void*			GetMemberUserData( const char* name, void* = 0 ) = 0;
		virtual void*			GetMemberUserData( float name, void* = 0 ) = 0;
		virtual ILuaObject* 	GetMember( const char* name ) = 0;
		virtual ILuaObject* 	GetMember( ILuaObject* ) = 0;
		

		virtual void			SetMetaTable( ILuaObject* obj ) = 0;
		virtual void			SetUserData( void* obj ) = 0;


		virtual void			Push( void ) = 0;

		virtual bool			isNil() = 0;
		virtual bool			isTable() = 0;
		virtual bool			isString() = 0;
		virtual bool			isNumber() = 0;
		virtual bool			isFunction() = 0;
		virtual bool			isUserData() = 0;

		// Added 7/Dec/2006
		virtual ILuaObject* 	GetMember( float fKey ) = 0;

		// Added 17/Dec/2006
		virtual void*			GetMemberEntity( const char* name, void* = 0 ) = 0; // This is useless to everyone but me - so don't bother trying to use it.
		virtual void			SetMember( float fKey ) = 0;
		virtual void			SetMember( float fKey, ILuaObject* obj ) = 0;
		virtual void			SetMember( float fKey, float val ) = 0;
		virtual void			SetMember( float fKey, bool val ) = 0;
		virtual void			SetMember( float fKey, const char* val ) = 0;
		virtual void			SetMember( float fKey, CLuaFunction f ) = 0;

		// Added 08/Jan/2007
		virtual const char*		GetMemberStr( float name, const char* = "" ) = 0;

		// Added 09/Jan/2007
		virtual void			SetMember( ILuaObject* k, ILuaObject* v ) = 0;
		virtual bool			GetBool( void ) = 0;

};

#endif // ILUAOBJECT_H
