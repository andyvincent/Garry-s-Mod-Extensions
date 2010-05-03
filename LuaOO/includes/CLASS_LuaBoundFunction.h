#ifndef _CLASS_LUABOUNDFUNCTION_H_
#define _CLASS_LUABOUNDFUNCTION_H_

#include "MODULE_LuaOO.h"

/*!
  \brief Container to link function name to function pointer
*/
class LuaBoundFunction
{
  const char* m_functionName;
  CLuaFunction m_function;
  bool m_isMetaFunction;
public:
  /*!
    \brief Constructor
    \param name Function name.
    \param fptr Function pointer.
    \param meta Indicates if the function is a meta function.
  */
  LuaBoundFunction(const char* name, CLuaFunction fptr, bool meta);

  /*!
    \brief Obtain the function name.
  */
  inline const char* functionName() const { return m_functionName; }

  /*!
    \brief Check if the function is a meta function
  */
  inline bool isMetaFunction() const { return m_isMetaFunction; }

  /*!
    \brief Obtain the function pointer.
  */
  inline CLuaFunction function() const { return m_function; }
};

#endif // _CLASS_LUABOUNDFUNCTION_H_
