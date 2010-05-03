#ifndef _CLASS_LUACLASSINFO_H_
#define _CLASS_LUACLASSINFO_H_

#include "MODULE_LuaOO.h"
#include <vector>

class LuaObjectBase;
class LuaBoundFunction;

/*!
  \brief Container to link class name, id to a list of functions.
*/
class LuaClassInfo
{
public:
  const char* m_className;
  int m_typeId;
  std::vector<LuaBoundFunction*> m_functionList;
public:
  /*!
    \brief Constructor
    \param className Class name.
    \param typeId Unique type id.
    \param functions List of functions.
  */
  LuaClassInfo(const char* className, int typeId, std::vector<LuaBoundFunction*> functions);

  /*!
    \brief Obtain the class name.
  */
  inline const char* className() const { return m_className; }

  /*!
    \brief Obtain the type id.
  */
  inline const int typeId() const { return m_typeId; }

  /*!
    \brief Obtain the function list.
  */
  inline const std::vector<LuaBoundFunction*>& functionList() const { return m_functionList; }
};

#endif
