/*
** Lua binding: ScriptMain
** Generated automatically by tolua++-1.0.92 on 04/14/14 17:06:25.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_ScriptMain_open (lua_State* tolua_S);

#include "../../misc/math/vector2.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Demi__DiVec2 (lua_State* tolua_S)
{
 Demi::DiVec2* self = (Demi::DiVec2*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Demi::DiVec2");
}

/* method: new of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2_new00
static int tolua_ScriptMain_Demi_DiVec2_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiVec2* tolua_ret = (Demi::DiVec2*)  Mtolua_new((Demi::DiVec2)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec2");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2_new00_local
static int tolua_ScriptMain_Demi_DiVec2_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiVec2* tolua_ret = (Demi::DiVec2*)  Mtolua_new((Demi::DiVec2)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'new'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2_new01
static int tolua_ScriptMain_Demi_DiVec2_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const float tolua_var_1 = ((const float)  tolua_tonumber(tolua_S,2,0));
  {
   Demi::DiVec2* tolua_ret = (Demi::DiVec2*)  Mtolua_new((Demi::DiVec2)(tolua_var_1));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec2");
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec2_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2_new01_local
static int tolua_ScriptMain_Demi_DiVec2_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const float tolua_var_1 = ((const float)  tolua_tonumber(tolua_S,2,0));
  {
   Demi::DiVec2* tolua_ret = (Demi::DiVec2*)  Mtolua_new((Demi::DiVec2)(tolua_var_1));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec2_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2_new02
static int tolua_ScriptMain_Demi_DiVec2_new02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const float tolua_var_2 = ((const float)  tolua_tonumber(tolua_S,2,0));
  const float tolua_var_3 = ((const float)  tolua_tonumber(tolua_S,3,0));
  {
   Demi::DiVec2* tolua_ret = (Demi::DiVec2*)  Mtolua_new((Demi::DiVec2)(tolua_var_2,tolua_var_3));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec2");
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec2_new01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2_new02_local
static int tolua_ScriptMain_Demi_DiVec2_new02_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const float tolua_var_2 = ((const float)  tolua_tonumber(tolua_S,2,0));
  const float tolua_var_3 = ((const float)  tolua_tonumber(tolua_S,3,0));
  {
   Demi::DiVec2* tolua_ret = (Demi::DiVec2*)  Mtolua_new((Demi::DiVec2)(tolua_var_2,tolua_var_3));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec2_new01_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: length of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2_length00
static int tolua_ScriptMain_Demi_DiVec2_length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec2* self = (const Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'length'", NULL);
#endif
  {
   float tolua_ret = (float)  self->length();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'length'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: squaredLength of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2_squaredLength00
static int tolua_ScriptMain_Demi_DiVec2_squaredLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec2",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec2* self = (const Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'squaredLength'", NULL);
#endif
  {
   float tolua_ret = (float)  self->squaredLength();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'squaredLength'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_ScriptMain_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"Demi",0);
  tolua_beginmodule(tolua_S,"Demi");
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"DiVec2","Demi::DiVec2","",tolua_collect_Demi__DiVec2);
   #else
   tolua_cclass(tolua_S,"DiVec2","Demi::DiVec2","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"DiVec2");
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiVec2_new00);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiVec2_new00_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiVec2_new00_local);
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiVec2_new01);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiVec2_new01_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiVec2_new01_local);
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiVec2_new02);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiVec2_new02_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiVec2_new02_local);
    tolua_function(tolua_S,"length",tolua_ScriptMain_Demi_DiVec2_length00);
    tolua_function(tolua_S,"squaredLength",tolua_ScriptMain_Demi_DiVec2_squaredLength00);
   tolua_endmodule(tolua_S);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_ScriptMain (lua_State* tolua_S) {
 return tolua_ScriptMain_open(tolua_S);
};
#endif

