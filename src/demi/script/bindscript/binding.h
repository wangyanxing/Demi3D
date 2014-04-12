/*
** Lua binding: ScriptMain
** Generated automatically by tolua++-1.0.92 on Sat Apr 12 00:03:31 2014.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_ScriptMain_open (lua_State* tolua_S);

#include "../../misc/math/vector2.h"
#include "../../misc/math/vector3.h"
#include "../../misc/math/vector4.h"

/* function to release collected object via destructor */
#ifdef __cplusplus

static int tolua_collect_Demi__DiVec2 (lua_State* tolua_S)
{
 Demi::DiVec2* self = (Demi::DiVec2*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Demi__DiVec3 (lua_State* tolua_S)
{
 Demi::DiVec3* self = (Demi::DiVec3*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Demi__DiVec4 (lua_State* tolua_S)
{
 Demi::DiVec4* self = (Demi::DiVec4*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Demi::DiVec2");
 tolua_usertype(tolua_S,"Demi::DiVec3");
 tolua_usertype(tolua_S,"Demi::DiVec4");
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

/* method: operator+ of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2__add00
static int tolua_ScriptMain_Demi_DiVec2__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec2",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec2",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec2* self = (const Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec2* vec = ((const Demi::DiVec2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Demi::DiVec2 tolua_ret = (Demi::DiVec2)  self->operator+(*vec);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec2)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec2));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2__sub00
static int tolua_ScriptMain_Demi_DiVec2__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec2",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec2",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec2* self = (const Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec2* vec = ((const Demi::DiVec2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiVec2 tolua_ret = (Demi::DiVec2)  self->operator-(*vec);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec2)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec2));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2__mul00
static int tolua_ScriptMain_Demi_DiVec2__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec2",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec2",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec2* self = (const Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec2* vec = ((const Demi::DiVec2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiVec2 tolua_ret = (Demi::DiVec2)  self->operator*(*vec);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec2)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec2));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2__mul01
static int tolua_ScriptMain_Demi_DiVec2__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec2",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiVec2* self = (const Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
  const float fScalar = ((const float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiVec2 tolua_ret = (Demi::DiVec2)  self->operator*(fScalar);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec2)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec2));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec2");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec2__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec2__eq00
static int tolua_ScriptMain_Demi_DiVec2__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec2",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec2",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec2* self = (const Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec2* vec = ((const Demi::DiVec2*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*vec);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
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

/* method: new of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3_new00
static int tolua_ScriptMain_Demi_DiVec3_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiVec3* tolua_ret = (Demi::DiVec3*)  Mtolua_new((Demi::DiVec3)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec3");
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

/* method: new_local of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3_new00_local
static int tolua_ScriptMain_Demi_DiVec3_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiVec3* tolua_ret = (Demi::DiVec3*)  Mtolua_new((Demi::DiVec3)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec3");
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

/* method: new of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3_new01
static int tolua_ScriptMain_Demi_DiVec3_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const float tolua_var_4 = ((const float)  tolua_tonumber(tolua_S,2,0));
  const float tolua_var_5 = ((const float)  tolua_tonumber(tolua_S,3,0));
  const float tolua_var_6 = ((const float)  tolua_tonumber(tolua_S,4,0));
  {
   Demi::DiVec3* tolua_ret = (Demi::DiVec3*)  Mtolua_new((Demi::DiVec3)(tolua_var_4,tolua_var_5,tolua_var_6));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec3");
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec3_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3_new01_local
static int tolua_ScriptMain_Demi_DiVec3_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const float tolua_var_4 = ((const float)  tolua_tonumber(tolua_S,2,0));
  const float tolua_var_5 = ((const float)  tolua_tonumber(tolua_S,3,0));
  const float tolua_var_6 = ((const float)  tolua_tonumber(tolua_S,4,0));
  {
   Demi::DiVec3* tolua_ret = (Demi::DiVec3*)  Mtolua_new((Demi::DiVec3)(tolua_var_4,tolua_var_5,tolua_var_6));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec3_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3__add00
static int tolua_ScriptMain_Demi_DiVec3__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec3* vec = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Demi::DiVec3 tolua_ret = (Demi::DiVec3)  self->operator+(*vec);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec3));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3__sub00
static int tolua_ScriptMain_Demi_DiVec3__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec3* vec = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiVec3 tolua_ret = (Demi::DiVec3)  self->operator-(*vec);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec3));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3__mul00
static int tolua_ScriptMain_Demi_DiVec3__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec3* vec = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiVec3 tolua_ret = (Demi::DiVec3)  self->operator*(*vec);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec3));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3__mul01
static int tolua_ScriptMain_Demi_DiVec3__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
  const float fScalar = ((const float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiVec3 tolua_ret = (Demi::DiVec3)  self->operator*(fScalar);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec3));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec3__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3__div00
static int tolua_ScriptMain_Demi_DiVec3__div00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
  const float fScalar = ((const float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'", NULL);
#endif
  {
   Demi::DiVec3 tolua_ret = (Demi::DiVec3)  self->operator/(fScalar);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec3));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.div'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3__eq00
static int tolua_ScriptMain_Demi_DiVec3__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec3* vec = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*vec);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3__add01
static int tolua_ScriptMain_Demi_DiVec3__add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   const Demi::DiVec3& tolua_ret = (const Demi::DiVec3&)  self->operator+();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Demi::DiVec3");
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec3__add00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3__sub01
static int tolua_ScriptMain_Demi_DiVec3__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiVec3 tolua_ret = (Demi::DiVec3)  self->operator-();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec3));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec3__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: distance of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3_distance00
static int tolua_ScriptMain_Demi_DiVec3_distance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec3* rhs = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'distance'", NULL);
#endif
  {
   float tolua_ret = (float)  self->distance(*rhs);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'distance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: squaredDistance of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3_squaredDistance00
static int tolua_ScriptMain_Demi_DiVec3_squaredDistance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec3* rhs = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'squaredDistance'", NULL);
#endif
  {
   float tolua_ret = (float)  self->squaredDistance(*rhs);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'squaredDistance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dotProduct of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3_dotProduct00
static int tolua_ScriptMain_Demi_DiVec3_dotProduct00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec3* vec = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dotProduct'", NULL);
#endif
  {
   float tolua_ret = (float)  self->dotProduct(*vec);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dotProduct'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: length of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3_length00
static int tolua_ScriptMain_Demi_DiVec3_length00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
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

/* method: squaredLength of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec3_squaredLength00
static int tolua_ScriptMain_Demi_DiVec3_squaredLength00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec3* self = (const Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
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

/* method: new of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4_new00
static int tolua_ScriptMain_Demi_DiVec4_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiVec4* tolua_ret = (Demi::DiVec4*)  Mtolua_new((Demi::DiVec4)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec4");
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

/* method: new_local of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4_new00_local
static int tolua_ScriptMain_Demi_DiVec4_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec4",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiVec4* tolua_ret = (Demi::DiVec4*)  Mtolua_new((Demi::DiVec4)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec4");
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

/* method: new of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4_new01
static int tolua_ScriptMain_Demi_DiVec4_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec4",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const float tolua_var_7 = ((const float)  tolua_tonumber(tolua_S,2,0));
  const float tolua_var_8 = ((const float)  tolua_tonumber(tolua_S,3,0));
  const float tolua_var_9 = ((const float)  tolua_tonumber(tolua_S,4,0));
  const float tolua_var_10 = ((const float)  tolua_tonumber(tolua_S,5,0));
  {
   Demi::DiVec4* tolua_ret = (Demi::DiVec4*)  Mtolua_new((Demi::DiVec4)(tolua_var_7,tolua_var_8,tolua_var_9,tolua_var_10));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec4");
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec4_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4_new01_local
static int tolua_ScriptMain_Demi_DiVec4_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiVec4",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const float tolua_var_7 = ((const float)  tolua_tonumber(tolua_S,2,0));
  const float tolua_var_8 = ((const float)  tolua_tonumber(tolua_S,3,0));
  const float tolua_var_9 = ((const float)  tolua_tonumber(tolua_S,4,0));
  const float tolua_var_10 = ((const float)  tolua_tonumber(tolua_S,5,0));
  {
   Demi::DiVec4* tolua_ret = (Demi::DiVec4*)  Mtolua_new((Demi::DiVec4)(tolua_var_7,tolua_var_8,tolua_var_9,tolua_var_10));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiVec4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec4_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4__add00
static int tolua_ScriptMain_Demi_DiVec4__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec4",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec4",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec4* self = (const Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec4* vec = ((const Demi::DiVec4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Demi::DiVec4 tolua_ret = (Demi::DiVec4)  self->operator+(*vec);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec4)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec4));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.add'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4__sub00
static int tolua_ScriptMain_Demi_DiVec4__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec4",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec4",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec4* self = (const Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec4* vec = ((const Demi::DiVec4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiVec4 tolua_ret = (Demi::DiVec4)  self->operator-(*vec);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec4)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec4));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.sub'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4__mul00
static int tolua_ScriptMain_Demi_DiVec4__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec4",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec4",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec4* self = (const Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec4* vec = ((const Demi::DiVec4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiVec4 tolua_ret = (Demi::DiVec4)  self->operator*(*vec);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec4)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec4));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.mul'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4__mul01
static int tolua_ScriptMain_Demi_DiVec4__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec4",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiVec4* self = (const Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
  const float fScalar = ((const float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiVec4 tolua_ret = (Demi::DiVec4)  self->operator*(fScalar);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiVec4)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiVec4));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiVec4");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiVec4__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4__eq00
static int tolua_ScriptMain_Demi_DiVec4__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec4",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec4",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec4* self = (const Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec4* vec = ((const Demi::DiVec4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*vec);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.eq'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: dotProduct of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiVec4_dotProduct00
static int tolua_ScriptMain_Demi_DiVec4_dotProduct00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiVec4",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec4",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiVec4* self = (const Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec4* vec = ((const Demi::DiVec4*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'dotProduct'", NULL);
#endif
  {
   float tolua_ret = (float)  self->dotProduct(*vec);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'dotProduct'.",&tolua_err);
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
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiVec2__add00);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiVec2__sub00);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiVec2__mul00);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiVec2__mul01);
    tolua_function(tolua_S,".eq",tolua_ScriptMain_Demi_DiVec2__eq00);
    tolua_function(tolua_S,"length",tolua_ScriptMain_Demi_DiVec2_length00);
    tolua_function(tolua_S,"squaredLength",tolua_ScriptMain_Demi_DiVec2_squaredLength00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"DiVec3","Demi::DiVec3","",tolua_collect_Demi__DiVec3);
   #else
   tolua_cclass(tolua_S,"DiVec3","Demi::DiVec3","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"DiVec3");
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiVec3_new00);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiVec3_new00_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiVec3_new00_local);
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiVec3_new01);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiVec3_new01_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiVec3_new01_local);
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiVec3__add00);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiVec3__sub00);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiVec3__mul00);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiVec3__mul01);
    tolua_function(tolua_S,".div",tolua_ScriptMain_Demi_DiVec3__div00);
    tolua_function(tolua_S,".eq",tolua_ScriptMain_Demi_DiVec3__eq00);
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiVec3__add01);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiVec3__sub01);
    tolua_function(tolua_S,"distance",tolua_ScriptMain_Demi_DiVec3_distance00);
    tolua_function(tolua_S,"squaredDistance",tolua_ScriptMain_Demi_DiVec3_squaredDistance00);
    tolua_function(tolua_S,"dotProduct",tolua_ScriptMain_Demi_DiVec3_dotProduct00);
    tolua_function(tolua_S,"length",tolua_ScriptMain_Demi_DiVec3_length00);
    tolua_function(tolua_S,"squaredLength",tolua_ScriptMain_Demi_DiVec3_squaredLength00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"DiVec4","Demi::DiVec4","",tolua_collect_Demi__DiVec4);
   #else
   tolua_cclass(tolua_S,"DiVec4","Demi::DiVec4","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"DiVec4");
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiVec4_new00);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiVec4_new00_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiVec4_new00_local);
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiVec4_new01);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiVec4_new01_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiVec4_new01_local);
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiVec4__add00);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiVec4__sub00);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiVec4__mul00);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiVec4__mul01);
    tolua_function(tolua_S,".eq",tolua_ScriptMain_Demi_DiVec4__eq00);
    tolua_function(tolua_S,"dotProduct",tolua_ScriptMain_Demi_DiVec4_dotProduct00);
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

