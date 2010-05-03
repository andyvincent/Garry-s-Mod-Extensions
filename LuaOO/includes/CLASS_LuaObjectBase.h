#ifndef _CLASS_LUAOBJECTBASE_H_
#define _CLASS_LUAOBJECTBASE_H_

#include "MODULE_LuaOO.h"
#include "CLASS_LuaClassInfo.h"
#include "CLASS_LuaBoundFunction.h"

#include <map>
#include <string>

#define LUA_OBJECT_FUNCTION( _function_ ) int _function_( lua_State* L )

/*!
  \brief Base object to extend from.
*/
class LuaObjectBase
{
public:
  /*!
    \brief Constructor.
    \param classInfo Class registration information.
    \param luaInterface Lua interface.
  */
  LuaObjectBase(const LuaClassInfo& classInfo, ILuaInterface* luaInterface);

  /*!
    \brief Destructor.
  */
  virtual ~LuaObjectBase(void);

  /*!
    \brief Override this if necessary, for the IsValid binding.
    \param error If true output an error message if object is invalid.
  */
  virtual bool isValid(bool error);

  /*!
    \brief Only override this function if you need to, the base implementation 
           should be enough
  */
  virtual int toString();

  /*!
    \brief Called every frame for the object.
  */
  virtual void poll();

  /*!
    \brief Run a callback function in, stored in this object.
    \param functionName Callback function name
    \param signiture Function signiture. 
                        f = number (float)
                        i = integer (int)
                        o = object (ILuaObject*)
                        b = boolean (bool)
                        F = function (CLuaFunction)
                        r = reference (int)
                        s = string (const char*)
  */
  void runCallback(const char* functionName, const char* signiture = 0, ...);

  /*!
    \brief Check an argument.
    \param stackPosition Stack item to check
    \param expectedType One of GLua::TYPE_*
  */
  bool checkArgument(int stackPosition, int expectedType);

  /*!
    \brief Get this object as an ILuaObject
  */
  ILuaObject* getAsObject();
  
  /*!
    \brief Push this on to the stack
  */
  void pushObject();
  
  /*!
    \brief Get an object from the stack
    \param luaInterface Lua interface pointer.
    \param position Stack position
    \param error Output a message if the object is invalid.
  */
  static LuaObjectBase* getFromStack(ILuaInterface* luaInterface, int position, bool error);

  /*!
    \brief Get an object from a ILuaObject
    \param luaInterface Lua interface pointer.
    \param object Object to query
    \param error Output a message if the object is invalid.
  */
  static LuaObjectBase* getFromObject(ILuaInterface* luaInterface, ILuaObject* object, bool error);

  /*!
    \brief Lua wrapper for isValid
  */
  LUA_FUNCTION( isValidWrapper );

  /*!
    \brief Lua wrapper for toString
  */
  LUA_FUNCTION( toStringWrapper );

  /*!
    \brief Lua wrapper for poll
  */
  LUA_FUNCTION( pollWrapper );

  /*!
    \brief Lua wrapper for delete & __gc
  */
  LUA_FUNCTION( deleteWrapper );

  /*!
    \brief Lua wrapper for __index
  */
  LUA_FUNCTION( indexWrapper );

  /*!
    \brief Lua wrapper for __newindex
  */
  LUA_FUNCTION( newIndexWrapper );

  /*!
    \brief Lua wrapper called when an object is attempted to be 
           obtained, but is not valid.
  */
  LUA_FUNCTION( failureFunction );
protected:
  ILuaInterface* m_luaInterface;

  /*!
    \brief Look up a function based on it's name
  */
  CLuaFunction lookupFunction(const char* name);
private:
  /*!
    \brief Check an objects validity
    \param luaInterface Lua interface pointer
    \param position Stack position to check
    \param error Output an error if invalid
  */
  static bool checkValidity(ILuaInterface* luaInterface, int position, bool error);

  /*!
    \brief Check an objects validity
    \param luaInterface Lua interface pointer
    \param type Type ID check for
    \param object Object to check
    \param error Output an error if invalid
  */
  static bool checkValidity(ILuaInterface* luaInterface, int type, LuaObjectBase* object, bool error);

  const LuaClassInfo& m_classInfo;
  std::map<std::string, int> m_userTable;
};

/*!
  \brief Template to create objects from.
  \param CLASS Class name
  \param UID Unique ID
*/
template<class CLASS, unsigned UID = CLASS::TypeID> class LuaObjectBaseTemplate : public LuaObjectBase
{
  static LuaClassInfo classInfo;
public:
  enum { TypeID = UID };

  /*!
    \brief Constructor
    \param luaInterface Lua interface
  */
  LuaObjectBaseTemplate(ILuaInterface* luaInterface)
    : LuaObjectBase(classInfo, luaInterface)
  {
  }

  /*
    \brief Internal use only
  */
  template<int (CLASS::*fpt)(), CLuaFunction FAILURE> class WrappedFunction
  {
  public:
    static int call(lua_State* L)
    {
      ILuaInterface* gLua = Lua();
      CLASS* pObject = dynamic_cast<CLASS*>( getFromStack(gLua, 1, true) );
      if (!pObject)
        return FAILURE(L);
      return (pObject->*fpt)();
    }
  };
};

/*!
  \brief Defines an object as a lua object.
*/
#define LUA_OBJECT                                                         \
  public:                                                                  \
    static std::vector<LuaBoundFunction*>& setupBindings();                \
  private:
      
/*!
  \brief Setup some class info
*/
#define LUA_CLASS_INFO(_CLASS_)                                            \
  LuaClassInfo LuaObjectBaseTemplate<_CLASS_>::classInfo( #_CLASS_,        \
                             _CLASS_::TypeID, _CLASS_ ::setupBindings() );

/*!
  \brief Setup some class info
*/
#define __BIND_FUNCTION(_FUNCTION_,_DEF_,_META_)   \
  result.push_back( new LuaBoundFunction(#_FUNCTION_,_DEF_,_META_) );

/*!
  \brief Begin binding functions for this class.
*/
#define BEGIN_BINDING(_CLASS_)                                             \
  LUA_CLASS_INFO(_CLASS_)                                                  \
  std::vector<LuaBoundFunction*>& _CLASS_::setupBindings()                 \
  {                                                                        \
    static std::vector<LuaBoundFunction*> result;                          \
    if (result.empty())                                                    \
    {                                                                      \
      __BIND_FUNCTION(isValid, LuaObjectBase::isValidWrapper, false)       \
      __BIND_FUNCTION(delete, LuaObjectBase::deleteWrapper, false)         \
      __BIND_FUNCTION(poll, LuaObjectBase::pollWrapper, false)

/*!
  \brief Bind a function for this class
  \param _FUNCTION_ function to bind in the following specification:
              int _CLASS_::_FUNCTION_();
*/
#define BIND_FUNCTION(_FUNCTION_)                   __BIND_FUNCTION( _FUNCTION_, (WrappedFunction<&_FUNCTION_, failureFunction>::call), false )

/*!
  \brief Bind a meta function for this class
  \param _FUNCTION_ function to bind in the following specification:
              int _CLASS_::_FUNCTION_();
*/
#define BIND_META_FUNCTION(_FUNCTION_)              __BIND_FUNCTION( _FUNCTION_, (WrappedFunction<&_FUNCTION_, failureFunction>::call), true )

/*!
  \brief Bind a function for this class
  \param _FUNCTION_ function to bind in the following specification:
              int _CLASS_::_FUNCTION_();
  \param _FAILURE_ function to call if failed to get the object from the stack
*/
#define BIND_FUNCTION_EX(_FUNCTION_,_FAILURE_)      __BIND_FUNCTION( _FUNCTION_, (WrappedFunction<&_FUNCTION_, _FAILURE_>::call), false )

/*!
  \brief Bind a meta function for this class
  \param _FUNCTION_ function to bind in the following specification:
              int _CLASS_::_FUNCTION_();
  \param _FAILURE_ function to call if failed to get the object from the stack
*/
#define BIND_META_FUNCTION_EX(_FUNCTION_,_FAILURE_) __BIND_FUNCTION( _FUNCTION_, (WrappedFunction<&_FUNCTION_, _FAILURE_>::call), true )
    
/*!
  \brief Finish binding functions for this class.
*/
#define END_BINDING() } return result; }


#endif //_CLASS_LUAOBJECTBASE_H_