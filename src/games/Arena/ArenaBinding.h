/*
** Lua binding: arenaMain
** Generated automatically by tolua++-1.0.92 on 05/07/14 21:17:15.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_arenaMain_open (lua_State* tolua_S);

#include "../../games/Arena/ArenaScriptAPI.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: ar_openLevel */
#ifndef TOLUA_DISABLE_tolua_arenaMain_ar_openLevel00
static int tolua_arenaMain_ar_openLevel00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* configFile = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   ar_openLevel(configFile);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ar_openLevel'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ar_setHero */
#ifndef TOLUA_DISABLE_tolua_arenaMain_ar_setHero00
static int tolua_arenaMain_ar_setHero00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* configFile = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   ar_setHero(configFile);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ar_setHero'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* function: ar_spawnNPC */
#ifndef TOLUA_DISABLE_tolua_arenaMain_ar_spawnNPC00
static int tolua_arenaMain_ar_spawnNPC00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const char* configFile = ((const char*)  tolua_tostring(tolua_S,1,0));
  {
   ar_spawnNPC(configFile);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'ar_spawnNPC'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_arenaMain_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_function(tolua_S,"ar_openLevel",tolua_arenaMain_ar_openLevel00);
  tolua_function(tolua_S,"ar_setHero",tolua_arenaMain_ar_setHero00);
  tolua_function(tolua_S,"ar_spawnNPC",tolua_arenaMain_ar_spawnNPC00);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_arenaMain (lua_State* tolua_S) {
 return tolua_arenaMain_open(tolua_S);
};
#endif

