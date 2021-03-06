
-- Load dependency from local directory
local CodeGen = require "lua-CodeGen.src.CodeGen"

return CodeGen{
wholeFile = [[
/** @file
	@brief Implementation GENERATED BY generate-operators.lua - Do not edit by hand!

	@date 2013

	@author
	Ryan Pavlik
	<rpavlik@iastate.edu> and <abiryan@ryand.net>
	http://academic.cleardefinition.com/
	Iowa State University Virtual Reality Applications Center
	Human-Computer Interaction Graduate Program
	
*/

//           Copyright Iowa State University 2013.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


// Internal Includes
#include "RegisterMathMetamethods.h"
#include "UsableAs.h"
#include "MissingOperators.h"

#include "LuaIncludeFull.h"

// Library/third-party includes

#include <osgLua/Value>

#include <osgLua/introspection/ExtendedTypeInfo>
#include <osgLua/introspection/Value>
#include <osgLua/introspection/Type>
#include <osgLua/introspection/variant_cast>

${types/includes(); separator='\n'}

// Standard includes
// - none


namespace osgLua {
  // Anonymous namespace for tag types
  namespace {
    ${operators/operatorTag()}
    ${unaryoperators/operatorTag()}
  } // end of namespace

  template<typename Operator, typename T>
  struct AttemptOperator;

  ${unaryattempts/singleTypeUnaryAttempt()}

  ${attempts/singleTypeAttempt()}

  bool registerMathMetamethods(lua_State * L, introspection::Type const& t) {
    ${types/singleTypePush()}
    return false;
  }
} // end of namespace osgLua

]];
  
includes = [[#include <osg/${baretypename}>]];

operatorTag = [[struct ${name};
]];

singleTypeUnaryAttempt = [[
template<>
struct AttemptOperator<${operator}, ${typename}> {
  static int attempt(lua_State * L) {
   if (lua_isnil(L, -1)) {
     return luaL_error(L, "[%s:%d] Could not ${operator}: operand is nil", __FILE__, __LINE__);
   }
   ${typename} a = introspection::variant_cast<${typename}>(getValue(L, -1));
   introspection::Value r = ${perform};
   Value::push(L, r);
   return 1;
  }
};

]];

singleTypeAttempt = [[
template<>
struct AttemptOperator<${operator}, ${typename}> {
  static int attempt(lua_State * L) {
    if (lua_isnil(L, -2) || lua_isnil(L, -1)) {
      return luaL_error(L, "[%s:%d] Could not ${operator}: %s operand is nil", __FILE__, __LINE__, (lua_isnil(L, -2) ? "first" : "second"));
    }
    ${asFirst?attemptFirst()}
    ${asSecond?attemptSecond()}
    
    return luaL_error(L, "[%s:%d] Could not ${operator} instances of %s, %s", __FILE__, __LINE__,
      getValue(L, -2).getType().getQualifiedName().c_str(), getValue(L, -1).getType().getQualifiedName().c_str());
  }
};

]];

singleTypePush = [[
if (introspection::Reflection::getType(extended_typeid<${typename}>()) == t) {
  ${operators/singleOperatorPush()}
  return true;
}
]];
singleOperatorPush = [[
lua_pushcfunction(L, &(AttemptOperator<${operator}, ${typename}>::attempt));
lua_setfield(L, -2, "__${operator}");
]];

attemptFirst = [[
if (osgLuaValueUsableAs<${typename}>(L, -2)) {
  ${asFirst/bothArgAttemptFirst()}
}
]];

bothArgAttemptFirst = [[
if (osgLuaValueUsableAs<${other}>(L, -1)) {
  ${bothArgPerform()}
}
]];

attemptSecond = [[
if (osgLuaValueUsableAs<${typename}>(L, -1)) {
  ${asSecond/bothArgAttemptSecond()}
}
]];

bothArgAttemptSecond = [[
if (osgLuaValueUsableAs<${other}>(L, -2)) {
  ${bothArgPerform()}
}
]];

bothArgPerform = [[
${aType} a = introspection::variant_cast<${aType}>(getValue(L, -2));
${bType} b = introspection::variant_cast<${bType}>(getValue(L, -1));
introspection::Value r = ${perform};
Value::push(L, r);
return 1;

]];

}
