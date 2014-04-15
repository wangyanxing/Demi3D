/*
** Lua binding: ScriptMain
** Generated automatically by tolua++-1.0.92 on Tue Apr 15 14:25:34 2014.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_ScriptMain_open (lua_State* tolua_S);

#include "../misc/math/vector2.h"
#include "../misc/math/vector3.h"
#include "../misc/math/vector4.h"
#include "../misc/math/quaternion.h"
#include "../misc/math/matrix3.h"
#include "../misc/math/MathMisc.h"
#include "../misc/LogManager.h"

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

static int tolua_collect_Demi__DiQuat (lua_State* tolua_S)
{
 Demi::DiQuat* self = (Demi::DiQuat*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Demi__DiMat3 (lua_State* tolua_S)
{
 Demi::DiMat3* self = (Demi::DiMat3*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Demi__DiRadian (lua_State* tolua_S)
{
 Demi::DiRadian* self = (Demi::DiRadian*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}

static int tolua_collect_Demi__DiDegree (lua_State* tolua_S)
{
 Demi::DiDegree* self = (Demi::DiDegree*) tolua_tousertype(tolua_S,1,0);
	Mtolua_delete(self);
	return 0;
}
#endif


/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"Demi::DiVec2");
 tolua_usertype(tolua_S,"Demi::DiVec3");
 tolua_usertype(tolua_S,"Demi::DiQuat");
 tolua_usertype(tolua_S,"Demi::DiVec4");
 tolua_usertype(tolua_S,"Demi::DiMat3");
 tolua_usertype(tolua_S,"Demi::DiDegree");
 tolua_usertype(tolua_S,"Demi::DiRadian");
 tolua_usertype(tolua_S,"Demi::DiLogManager");
}

/* method: GetInstance of class  Demi::DiLogManager */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiLogManager_GetInstance00
static int tolua_ScriptMain_Demi_DiLogManager_GetInstance00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiLogManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiLogManager& tolua_ret = (Demi::DiLogManager&)  Demi::DiLogManager::GetInstance();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"Demi::DiLogManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'GetInstance'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Output of class  Demi::DiLogManager */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiLogManager_Output00
static int tolua_ScriptMain_Demi_DiLogManager_Output00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demi::DiLogManager",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isstring(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demi::DiLogManager* self = (Demi::DiLogManager*)  tolua_tousertype(tolua_S,1,0);
  Demi::LogLevel level = ((Demi::LogLevel) (int)  tolua_tonumber(tolua_S,2,0));
  const char* log = ((const char*)  tolua_tostring(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Output'", NULL);
#endif
  {
   self->Output(level,log);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Output'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

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

/* get function: x of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiVec2_x
static int tolua_get_Demi__DiVec2_x(lua_State* tolua_S)
{
  Demi::DiVec2* self = (Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiVec2_x
static int tolua_set_Demi__DiVec2_x(lua_State* tolua_S)
{
  Demi::DiVec2* self = (Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiVec2_y
static int tolua_get_Demi__DiVec2_y(lua_State* tolua_S)
{
  Demi::DiVec2* self = (Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Demi::DiVec2 */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiVec2_y
static int tolua_set_Demi__DiVec2_y(lua_State* tolua_S)
{
  Demi::DiVec2* self = (Demi::DiVec2*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
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

/* get function: x of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiVec3_x
static int tolua_get_Demi__DiVec3_x(lua_State* tolua_S)
{
  Demi::DiVec3* self = (Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiVec3_x
static int tolua_set_Demi__DiVec3_x(lua_State* tolua_S)
{
  Demi::DiVec3* self = (Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiVec3_y
static int tolua_get_Demi__DiVec3_y(lua_State* tolua_S)
{
  Demi::DiVec3* self = (Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiVec3_y
static int tolua_set_Demi__DiVec3_y(lua_State* tolua_S)
{
  Demi::DiVec3* self = (Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiVec3_z
static int tolua_get_Demi__DiVec3_z(lua_State* tolua_S)
{
  Demi::DiVec3* self = (Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Demi::DiVec3 */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiVec3_z
static int tolua_set_Demi__DiVec3_z(lua_State* tolua_S)
{
  Demi::DiVec3* self = (Demi::DiVec3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
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

/* get function: w of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiVec4_w
static int tolua_get_Demi__DiVec4_w(lua_State* tolua_S)
{
  Demi::DiVec4* self = (Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->w);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: w of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiVec4_w
static int tolua_set_Demi__DiVec4_w(lua_State* tolua_S)
{
  Demi::DiVec4* self = (Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->w = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiVec4_x
static int tolua_get_Demi__DiVec4_x(lua_State* tolua_S)
{
  Demi::DiVec4* self = (Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiVec4_x
static int tolua_set_Demi__DiVec4_x(lua_State* tolua_S)
{
  Demi::DiVec4* self = (Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiVec4_y
static int tolua_get_Demi__DiVec4_y(lua_State* tolua_S)
{
  Demi::DiVec4* self = (Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiVec4_y
static int tolua_set_Demi__DiVec4_y(lua_State* tolua_S)
{
  Demi::DiVec4* self = (Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiVec4_z
static int tolua_get_Demi__DiVec4_z(lua_State* tolua_S)
{
  Demi::DiVec4* self = (Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Demi::DiVec4 */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiVec4_z
static int tolua_set_Demi__DiVec4_z(lua_State* tolua_S)
{
  Demi::DiVec4* self = (Demi::DiVec4*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian_new00
static int tolua_ScriptMain_Demi_DiRadian_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiRadian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float tolua_var_11 = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Demi::DiRadian* tolua_ret = (Demi::DiRadian*)  Mtolua_new((Demi::DiRadian)(tolua_var_11));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiRadian");
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

/* method: new_local of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian_new00_local
static int tolua_ScriptMain_Demi_DiRadian_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiRadian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float tolua_var_11 = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Demi::DiRadian* tolua_ret = (Demi::DiRadian*)  Mtolua_new((Demi::DiRadian)(tolua_var_11));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiRadian");
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

/* method: new of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian_new01
static int tolua_ScriptMain_Demi_DiRadian_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiDegree* d = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
  {
   Demi::DiRadian* tolua_ret = (Demi::DiRadian*)  Mtolua_new((Demi::DiRadian)(*d));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiRadian");
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiRadian_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian_new01_local
static int tolua_ScriptMain_Demi_DiRadian_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiDegree* d = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
  {
   Demi::DiRadian* tolua_ret = (Demi::DiRadian*)  Mtolua_new((Demi::DiRadian)(*d));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiRadian_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueDegrees of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian_valueDegrees00
static int tolua_ScriptMain_Demi_DiRadian_valueDegrees00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueDegrees'", NULL);
#endif
  {
   float tolua_ret = (float)  self->valueDegrees();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueDegrees'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueRadians of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian_valueRadians00
static int tolua_ScriptMain_Demi_DiRadian_valueRadians00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueRadians'", NULL);
#endif
  {
   float tolua_ret = (float)  self->valueRadians();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueRadians'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__add00
static int tolua_ScriptMain_Demi_DiRadian__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   const Demi::DiRadian& tolua_ret = (const Demi::DiRadian&)  self->operator+();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Demi::DiRadian");
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

/* method: operator+ of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__add01
static int tolua_ScriptMain_Demi_DiRadian__add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiRadian* r = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Demi::DiRadian tolua_ret = (Demi::DiRadian)  self->operator+(*r);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiRadian)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiRadian));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiRadian__add00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__add02
static int tolua_ScriptMain_Demi_DiRadian__add02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiDegree* d = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Demi::DiRadian tolua_ret = (Demi::DiRadian)  self->operator+(*d);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiRadian)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiRadian));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiRadian__add01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__sub00
static int tolua_ScriptMain_Demi_DiRadian__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiRadian tolua_ret = (Demi::DiRadian)  self->operator-();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiRadian)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiRadian));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
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

/* method: operator- of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__sub01
static int tolua_ScriptMain_Demi_DiRadian__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiRadian* r = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiRadian tolua_ret = (Demi::DiRadian)  self->operator-(*r);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiRadian)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiRadian));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiRadian__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__sub02
static int tolua_ScriptMain_Demi_DiRadian__sub02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiDegree* d = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiRadian tolua_ret = (Demi::DiRadian)  self->operator-(*d);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiRadian)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiRadian));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiRadian__sub01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__mul00
static int tolua_ScriptMain_Demi_DiRadian__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiRadian tolua_ret = (Demi::DiRadian)  self->operator*(f);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiRadian)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiRadian));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
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

/* method: operator* of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__mul01
static int tolua_ScriptMain_Demi_DiRadian__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiRadian* f = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiRadian tolua_ret = (Demi::DiRadian)  self->operator*(*f);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiRadian)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiRadian));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiRadian__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__div00
static int tolua_ScriptMain_Demi_DiRadian__div00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'", NULL);
#endif
  {
   Demi::DiRadian tolua_ret = (Demi::DiRadian)  self->operator/(f);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiRadian)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiRadian));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiRadian");
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

/* method: operator< of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__lt00
static int tolua_ScriptMain_Demi_DiRadian__lt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiRadian* r = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<(*r);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.lt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator<= of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__le00
static int tolua_ScriptMain_Demi_DiRadian__le00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiRadian* r = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<='", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<=(*r);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.le'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Demi::DiRadian */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiRadian__eq00
static int tolua_ScriptMain_Demi_DiRadian__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiRadian",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiRadian* self = (const Demi::DiRadian*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiRadian* r = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*r);
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

/* method: new of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree_new00
static int tolua_ScriptMain_Demi_DiDegree_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiDegree",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float d = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Demi::DiDegree* tolua_ret = (Demi::DiDegree*)  Mtolua_new((Demi::DiDegree)(d));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiDegree");
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

/* method: new_local of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree_new00_local
static int tolua_ScriptMain_Demi_DiDegree_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiDegree",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  float d = ((float)  tolua_tonumber(tolua_S,2,0));
  {
   Demi::DiDegree* tolua_ret = (Demi::DiDegree*)  Mtolua_new((Demi::DiDegree)(d));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiDegree");
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

/* method: new of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree_new01
static int tolua_ScriptMain_Demi_DiDegree_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiRadian* r = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
  {
   Demi::DiDegree* tolua_ret = (Demi::DiDegree*)  Mtolua_new((Demi::DiDegree)(*r));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiDegree");
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiDegree_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree_new01_local
static int tolua_ScriptMain_Demi_DiDegree_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiRadian* r = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
  {
   Demi::DiDegree* tolua_ret = (Demi::DiDegree*)  Mtolua_new((Demi::DiDegree)(*r));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiDegree_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueDegrees of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree_valueDegrees00
static int tolua_ScriptMain_Demi_DiDegree_valueDegrees00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueDegrees'", NULL);
#endif
  {
   float tolua_ret = (float)  self->valueDegrees();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueDegrees'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: valueRadians of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree_valueRadians00
static int tolua_ScriptMain_Demi_DiDegree_valueRadians00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'valueRadians'", NULL);
#endif
  {
   float tolua_ret = (float)  self->valueRadians();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'valueRadians'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__add00
static int tolua_ScriptMain_Demi_DiDegree__add00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   const Demi::DiDegree& tolua_ret = (const Demi::DiDegree&)  self->operator+();
    tolua_pushusertype(tolua_S,(void*)&tolua_ret,"const Demi::DiDegree");
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

/* method: operator+ of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__add01
static int tolua_ScriptMain_Demi_DiDegree__add01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiDegree* d = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Demi::DiDegree tolua_ret = (Demi::DiDegree)  self->operator+(*d);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiDegree)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiDegree));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiDegree__add00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator+ of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__add02
static int tolua_ScriptMain_Demi_DiDegree__add02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiRadian* r = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator+'", NULL);
#endif
  {
   Demi::DiDegree tolua_ret = (Demi::DiDegree)  self->operator+(*r);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiDegree)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiDegree));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiDegree__add01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__sub00
static int tolua_ScriptMain_Demi_DiDegree__sub00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiDegree tolua_ret = (Demi::DiDegree)  self->operator-();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiDegree)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiDegree));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
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

/* method: operator- of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__sub01
static int tolua_ScriptMain_Demi_DiDegree__sub01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiDegree* d = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiDegree tolua_ret = (Demi::DiDegree)  self->operator-(*d);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiDegree)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiDegree));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiDegree__sub00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator- of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__sub02
static int tolua_ScriptMain_Demi_DiDegree__sub02(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiRadian",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiRadian* r = ((const Demi::DiRadian*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator-'", NULL);
#endif
  {
   Demi::DiDegree tolua_ret = (Demi::DiDegree)  self->operator-(*r);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiDegree)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiDegree));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiDegree__sub01(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator* of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__mul00
static int tolua_ScriptMain_Demi_DiDegree__mul00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiDegree tolua_ret = (Demi::DiDegree)  self->operator*(f);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiDegree)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiDegree));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
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

/* method: operator* of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__mul01
static int tolua_ScriptMain_Demi_DiDegree__mul01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Demi::DiDegree* self = (Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiDegree* f = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator*'", NULL);
#endif
  {
   Demi::DiDegree tolua_ret = (Demi::DiDegree)  self->operator*(*f);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiDegree)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiDegree));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiDegree__mul00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator/ of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__div00
static int tolua_ScriptMain_Demi_DiDegree__div00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  float f = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator/'", NULL);
#endif
  {
   Demi::DiDegree tolua_ret = (Demi::DiDegree)  self->operator/(f);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiDegree)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiDegree));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiDegree");
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

/* method: operator< of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__lt00
static int tolua_ScriptMain_Demi_DiDegree__lt00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiDegree* d = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<(*d);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.lt'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator<= of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__le00
static int tolua_ScriptMain_Demi_DiDegree__le00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiDegree* d = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator<='", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator<=(*d);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function '.le'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: operator== of class  Demi::DiDegree */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiDegree__eq00
static int tolua_ScriptMain_Demi_DiDegree__eq00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiDegree",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiDegree",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiDegree* self = (const Demi::DiDegree*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiDegree* d = ((const Demi::DiDegree*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'operator=='", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->operator==(*d);
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

/* method: new of class  Demi::DiMat3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiMat3_new00
static int tolua_ScriptMain_Demi_DiMat3_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiMat3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiMat3* tolua_ret = (Demi::DiMat3*)  Mtolua_new((Demi::DiMat3)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiMat3");
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

/* method: new_local of class  Demi::DiMat3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiMat3_new00_local
static int tolua_ScriptMain_Demi_DiMat3_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiMat3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiMat3* tolua_ret = (Demi::DiMat3*)  Mtolua_new((Demi::DiMat3)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiMat3");
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

/* method: new of class  Demi::DiMat3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiMat3_new01
static int tolua_ScriptMain_Demi_DiMat3_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiMat3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,10,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float fEntry00 = ((float)  tolua_tonumber(tolua_S,2,0));
  float fEntry01 = ((float)  tolua_tonumber(tolua_S,3,0));
  float fEntry02 = ((float)  tolua_tonumber(tolua_S,4,0));
  float fEntry10 = ((float)  tolua_tonumber(tolua_S,5,0));
  float fEntry11 = ((float)  tolua_tonumber(tolua_S,6,0));
  float fEntry12 = ((float)  tolua_tonumber(tolua_S,7,0));
  float fEntry20 = ((float)  tolua_tonumber(tolua_S,8,0));
  float fEntry21 = ((float)  tolua_tonumber(tolua_S,9,0));
  float fEntry22 = ((float)  tolua_tonumber(tolua_S,10,0));
  {
   Demi::DiMat3* tolua_ret = (Demi::DiMat3*)  Mtolua_new((Demi::DiMat3)(fEntry00,fEntry01,fEntry02,fEntry10,fEntry11,fEntry12,fEntry20,fEntry21,fEntry22));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiMat3");
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiMat3_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demi::DiMat3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiMat3_new01_local
static int tolua_ScriptMain_Demi_DiMat3_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiMat3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,6,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,7,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,8,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,9,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,10,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,11,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float fEntry00 = ((float)  tolua_tonumber(tolua_S,2,0));
  float fEntry01 = ((float)  tolua_tonumber(tolua_S,3,0));
  float fEntry02 = ((float)  tolua_tonumber(tolua_S,4,0));
  float fEntry10 = ((float)  tolua_tonumber(tolua_S,5,0));
  float fEntry11 = ((float)  tolua_tonumber(tolua_S,6,0));
  float fEntry12 = ((float)  tolua_tonumber(tolua_S,7,0));
  float fEntry20 = ((float)  tolua_tonumber(tolua_S,8,0));
  float fEntry21 = ((float)  tolua_tonumber(tolua_S,9,0));
  float fEntry22 = ((float)  tolua_tonumber(tolua_S,10,0));
  {
   Demi::DiMat3* tolua_ret = (Demi::DiMat3*)  Mtolua_new((Demi::DiMat3)(fEntry00,fEntry01,fEntry02,fEntry10,fEntry11,fEntry12,fEntry20,fEntry21,fEntry22));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiMat3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiMat3_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: Transpose of class  Demi::DiMat3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiMat3_Transpose00
static int tolua_ScriptMain_Demi_DiMat3_Transpose00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiMat3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiMat3* self = (const Demi::DiMat3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Transpose'", NULL);
#endif
  {
   Demi::DiMat3 tolua_ret = (Demi::DiMat3)  self->Transpose();
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiMat3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiMat3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiMat3));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiMat3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Transpose'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Inverse of class  Demi::DiMat3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiMat3_Inverse00
static int tolua_ScriptMain_Demi_DiMat3_Inverse00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiMat3",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"Demi::DiMat3",0,&tolua_err)) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,4,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiMat3* self = (const Demi::DiMat3*)  tolua_tousertype(tolua_S,1,0);
  Demi::DiMat3* rkInverse = ((Demi::DiMat3*)  tolua_tousertype(tolua_S,2,0));
  float fTolerance = ((float)  tolua_tonumber(tolua_S,3,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Inverse'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->Inverse(*rkInverse,fTolerance);
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'Inverse'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: Inverse of class  Demi::DiMat3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiMat3_Inverse01
static int tolua_ScriptMain_Demi_DiMat3_Inverse01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiMat3",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  const Demi::DiMat3* self = (const Demi::DiMat3*)  tolua_tousertype(tolua_S,1,0);
  float fTolerance = ((float)  tolua_tonumber(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'Inverse'", NULL);
#endif
  {
   Demi::DiMat3 tolua_ret = (Demi::DiMat3)  self->Inverse(fTolerance);
   {
#ifdef __cplusplus
    void* tolua_obj = Mtolua_new((Demi::DiMat3)(tolua_ret));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiMat3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#else
    void* tolua_obj = tolua_copy(tolua_S,(void*)&tolua_ret,sizeof(Demi::DiMat3));
     tolua_pushusertype(tolua_S,tolua_obj,"Demi::DiMat3");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
#endif
   }
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiMat3_Inverse00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: setIdentity of class  Demi::DiMat3 */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiMat3_setIdentity00
static int tolua_ScriptMain_Demi_DiMat3_setIdentity00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demi::DiMat3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demi::DiMat3* self = (Demi::DiMat3*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setIdentity'", NULL);
#endif
  {
   self->setIdentity();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setIdentity'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: new of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiQuat_new00
static int tolua_ScriptMain_Demi_DiQuat_new00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiQuat",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiQuat* tolua_ret = (Demi::DiQuat*)  Mtolua_new((Demi::DiQuat)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiQuat");
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

/* method: new_local of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiQuat_new00_local
static int tolua_ScriptMain_Demi_DiQuat_new00_local(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiQuat",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   Demi::DiQuat* tolua_ret = (Demi::DiQuat*)  Mtolua_new((Demi::DiQuat)());
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiQuat");
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

/* method: new of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiQuat_new01
static int tolua_ScriptMain_Demi_DiQuat_new01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiQuat",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float fW = ((float)  tolua_tonumber(tolua_S,2,0));
  float fX = ((float)  tolua_tonumber(tolua_S,3,0));
  float fY = ((float)  tolua_tonumber(tolua_S,4,0));
  float fZ = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   Demi::DiQuat* tolua_ret = (Demi::DiQuat*)  Mtolua_new((Demi::DiQuat)(fW,fX,fY,fZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiQuat");
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiQuat_new00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: new_local of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiQuat_new01_local
static int tolua_ScriptMain_Demi_DiQuat_new01_local(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"Demi::DiQuat",0,&tolua_err) ||
     !tolua_isnumber(tolua_S,2,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,3,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,4,0,&tolua_err) ||
     !tolua_isnumber(tolua_S,5,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,6,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  float fW = ((float)  tolua_tonumber(tolua_S,2,0));
  float fX = ((float)  tolua_tonumber(tolua_S,3,0));
  float fY = ((float)  tolua_tonumber(tolua_S,4,0));
  float fZ = ((float)  tolua_tonumber(tolua_S,5,0));
  {
   Demi::DiQuat* tolua_ret = (Demi::DiQuat*)  Mtolua_new((Demi::DiQuat)(fW,fX,fY,fZ));
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"Demi::DiQuat");
    tolua_register_gc(tolua_S,lua_gettop(tolua_S));
  }
 }
 return 1;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiQuat_new00_local(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: FromAxes of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiQuat_FromAxes00
static int tolua_ScriptMain_Demi_DiQuat_FromAxes00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demi::DiQuat",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"const Demi::DiVec3",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec3* akAxis = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FromAxes'", NULL);
#endif
  {
   self->FromAxes(akAxis);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'FromAxes'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: FromAxes of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiQuat_FromAxes01
static int tolua_ScriptMain_Demi_DiQuat_FromAxes01(lua_State* tolua_S)
{
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"Demi::DiQuat",0,&tolua_err) ||
     (tolua_isvaluenil(tolua_S,2,&tolua_err) || !tolua_isusertype(tolua_S,2,"const Demi::DiVec3",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,3,&tolua_err) || !tolua_isusertype(tolua_S,3,"const Demi::DiVec3",0,&tolua_err)) ||
     (tolua_isvaluenil(tolua_S,4,&tolua_err) || !tolua_isusertype(tolua_S,4,"const Demi::DiVec3",0,&tolua_err)) ||
     !tolua_isnoobj(tolua_S,5,&tolua_err)
 )
  goto tolua_lerror;
 else
 {
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
  const Demi::DiVec3* xAxis = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,2,0));
  const Demi::DiVec3* yAxis = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,3,0));
  const Demi::DiVec3* zAxis = ((const Demi::DiVec3*)  tolua_tousertype(tolua_S,4,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'FromAxes'", NULL);
#endif
  {
   self->FromAxes(*xAxis,*yAxis,*zAxis);
  }
 }
 return 0;
tolua_lerror:
 return tolua_ScriptMain_Demi_DiQuat_FromAxes00(tolua_S);
}
#endif //#ifndef TOLUA_DISABLE

/* method: xAxis of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiQuat_xAxis00
static int tolua_ScriptMain_Demi_DiQuat_xAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiQuat",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiQuat* self = (const Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'xAxis'", NULL);
#endif
  {
   Demi::DiVec3 tolua_ret = (Demi::DiVec3)  self->xAxis();
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
 tolua_error(tolua_S,"#ferror in function 'xAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: yAxis of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiQuat_yAxis00
static int tolua_ScriptMain_Demi_DiQuat_yAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiQuat",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiQuat* self = (const Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'yAxis'", NULL);
#endif
  {
   Demi::DiVec3 tolua_ret = (Demi::DiVec3)  self->yAxis();
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
 tolua_error(tolua_S,"#ferror in function 'yAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: zAxis of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_ScriptMain_Demi_DiQuat_zAxis00
static int tolua_ScriptMain_Demi_DiQuat_zAxis00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"const Demi::DiQuat",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  const Demi::DiQuat* self = (const Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'zAxis'", NULL);
#endif
  {
   Demi::DiVec3 tolua_ret = (Demi::DiVec3)  self->zAxis();
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
 tolua_error(tolua_S,"#ferror in function 'zAxis'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* get function: w of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiQuat_w
static int tolua_get_Demi__DiQuat_w(lua_State* tolua_S)
{
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->w);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: w of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiQuat_w
static int tolua_set_Demi__DiQuat_w(lua_State* tolua_S)
{
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'w'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->w = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: x of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiQuat_x
static int tolua_get_Demi__DiQuat_x(lua_State* tolua_S)
{
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->x);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: x of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiQuat_x
static int tolua_set_Demi__DiQuat_x(lua_State* tolua_S)
{
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'x'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->x = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: y of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiQuat_y
static int tolua_get_Demi__DiQuat_y(lua_State* tolua_S)
{
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->y);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: y of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiQuat_y
static int tolua_set_Demi__DiQuat_y(lua_State* tolua_S)
{
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'y'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->y = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
}
#endif //#ifndef TOLUA_DISABLE

/* get function: z of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_get_Demi__DiQuat_z
static int tolua_get_Demi__DiQuat_z(lua_State* tolua_S)
{
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
#endif
  tolua_pushnumber(tolua_S,(lua_Number)self->z);
 return 1;
}
#endif //#ifndef TOLUA_DISABLE

/* set function: z of class  Demi::DiQuat */
#ifndef TOLUA_DISABLE_tolua_set_Demi__DiQuat_z
static int tolua_set_Demi__DiQuat_z(lua_State* tolua_S)
{
  Demi::DiQuat* self = (Demi::DiQuat*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  tolua_Error tolua_err;
  if (!self) tolua_error(tolua_S,"invalid 'self' in accessing variable 'z'",NULL);
  if (!tolua_isnumber(tolua_S,2,0,&tolua_err))
   tolua_error(tolua_S,"#vinvalid type in variable assignment.",&tolua_err);
#endif
  self->z = ((float)  tolua_tonumber(tolua_S,2,0))
;
 return 0;
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
   tolua_constant(tolua_S,"LOG_LEVEL_ERROR",Demi::LOG_LEVEL_ERROR);
   tolua_constant(tolua_S,"LOG_LEVEL_WARNING",Demi::LOG_LEVEL_WARNING);
   tolua_constant(tolua_S,"LOG_LEVEL_LOG",Demi::LOG_LEVEL_LOG);
   tolua_constant(tolua_S,"LOG_LEVEL_DEBUG",Demi::LOG_LEVEL_DEBUG);
   tolua_cclass(tolua_S,"DiLogManager","Demi::DiLogManager","",NULL);
   tolua_beginmodule(tolua_S,"DiLogManager");
    tolua_function(tolua_S,"GetInstance",tolua_ScriptMain_Demi_DiLogManager_GetInstance00);
    tolua_function(tolua_S,"Output",tolua_ScriptMain_Demi_DiLogManager_Output00);
   tolua_endmodule(tolua_S);
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
    tolua_variable(tolua_S,"x",tolua_get_Demi__DiVec2_x,tolua_set_Demi__DiVec2_x);
    tolua_variable(tolua_S,"y",tolua_get_Demi__DiVec2_y,tolua_set_Demi__DiVec2_y);
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
    tolua_variable(tolua_S,"x",tolua_get_Demi__DiVec3_x,tolua_set_Demi__DiVec3_x);
    tolua_variable(tolua_S,"y",tolua_get_Demi__DiVec3_y,tolua_set_Demi__DiVec3_y);
    tolua_variable(tolua_S,"z",tolua_get_Demi__DiVec3_z,tolua_set_Demi__DiVec3_z);
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
    tolua_variable(tolua_S,"w",tolua_get_Demi__DiVec4_w,tolua_set_Demi__DiVec4_w);
    tolua_variable(tolua_S,"x",tolua_get_Demi__DiVec4_x,tolua_set_Demi__DiVec4_x);
    tolua_variable(tolua_S,"y",tolua_get_Demi__DiVec4_y,tolua_set_Demi__DiVec4_y);
    tolua_variable(tolua_S,"z",tolua_get_Demi__DiVec4_z,tolua_set_Demi__DiVec4_z);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"DiRadian","Demi::DiRadian","",tolua_collect_Demi__DiRadian);
   #else
   tolua_cclass(tolua_S,"DiRadian","Demi::DiRadian","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"DiRadian");
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiRadian_new00);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiRadian_new00_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiRadian_new00_local);
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiRadian_new01);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiRadian_new01_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiRadian_new01_local);
    tolua_function(tolua_S,"valueDegrees",tolua_ScriptMain_Demi_DiRadian_valueDegrees00);
    tolua_function(tolua_S,"valueRadians",tolua_ScriptMain_Demi_DiRadian_valueRadians00);
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiRadian__add00);
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiRadian__add01);
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiRadian__add02);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiRadian__sub00);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiRadian__sub01);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiRadian__sub02);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiRadian__mul00);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiRadian__mul01);
    tolua_function(tolua_S,".div",tolua_ScriptMain_Demi_DiRadian__div00);
    tolua_function(tolua_S,".lt",tolua_ScriptMain_Demi_DiRadian__lt00);
    tolua_function(tolua_S,".le",tolua_ScriptMain_Demi_DiRadian__le00);
    tolua_function(tolua_S,".eq",tolua_ScriptMain_Demi_DiRadian__eq00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"DiDegree","Demi::DiDegree","",tolua_collect_Demi__DiDegree);
   #else
   tolua_cclass(tolua_S,"DiDegree","Demi::DiDegree","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"DiDegree");
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiDegree_new00);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiDegree_new00_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiDegree_new00_local);
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiDegree_new01);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiDegree_new01_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiDegree_new01_local);
    tolua_function(tolua_S,"valueDegrees",tolua_ScriptMain_Demi_DiDegree_valueDegrees00);
    tolua_function(tolua_S,"valueRadians",tolua_ScriptMain_Demi_DiDegree_valueRadians00);
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiDegree__add00);
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiDegree__add01);
    tolua_function(tolua_S,".add",tolua_ScriptMain_Demi_DiDegree__add02);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiDegree__sub00);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiDegree__sub01);
    tolua_function(tolua_S,".sub",tolua_ScriptMain_Demi_DiDegree__sub02);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiDegree__mul00);
    tolua_function(tolua_S,".mul",tolua_ScriptMain_Demi_DiDegree__mul01);
    tolua_function(tolua_S,".div",tolua_ScriptMain_Demi_DiDegree__div00);
    tolua_function(tolua_S,".lt",tolua_ScriptMain_Demi_DiDegree__lt00);
    tolua_function(tolua_S,".le",tolua_ScriptMain_Demi_DiDegree__le00);
    tolua_function(tolua_S,".eq",tolua_ScriptMain_Demi_DiDegree__eq00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"DiMat3","Demi::DiMat3","",tolua_collect_Demi__DiMat3);
   #else
   tolua_cclass(tolua_S,"DiMat3","Demi::DiMat3","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"DiMat3");
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiMat3_new00);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiMat3_new00_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiMat3_new00_local);
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiMat3_new01);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiMat3_new01_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiMat3_new01_local);
    tolua_function(tolua_S,"Transpose",tolua_ScriptMain_Demi_DiMat3_Transpose00);
    tolua_function(tolua_S,"Inverse",tolua_ScriptMain_Demi_DiMat3_Inverse00);
    tolua_function(tolua_S,"Inverse",tolua_ScriptMain_Demi_DiMat3_Inverse01);
    tolua_function(tolua_S,"setIdentity",tolua_ScriptMain_Demi_DiMat3_setIdentity00);
   tolua_endmodule(tolua_S);
   #ifdef __cplusplus
   tolua_cclass(tolua_S,"DiQuat","Demi::DiQuat","",tolua_collect_Demi__DiQuat);
   #else
   tolua_cclass(tolua_S,"DiQuat","Demi::DiQuat","",NULL);
   #endif
   tolua_beginmodule(tolua_S,"DiQuat");
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiQuat_new00);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiQuat_new00_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiQuat_new00_local);
    tolua_function(tolua_S,"new",tolua_ScriptMain_Demi_DiQuat_new01);
    tolua_function(tolua_S,"new_local",tolua_ScriptMain_Demi_DiQuat_new01_local);
    tolua_function(tolua_S,".call",tolua_ScriptMain_Demi_DiQuat_new01_local);
    tolua_function(tolua_S,"FromAxes",tolua_ScriptMain_Demi_DiQuat_FromAxes00);
    tolua_function(tolua_S,"FromAxes",tolua_ScriptMain_Demi_DiQuat_FromAxes01);
    tolua_function(tolua_S,"xAxis",tolua_ScriptMain_Demi_DiQuat_xAxis00);
    tolua_function(tolua_S,"yAxis",tolua_ScriptMain_Demi_DiQuat_yAxis00);
    tolua_function(tolua_S,"zAxis",tolua_ScriptMain_Demi_DiQuat_zAxis00);
    tolua_variable(tolua_S,"w",tolua_get_Demi__DiQuat_w,tolua_set_Demi__DiQuat_w);
    tolua_variable(tolua_S,"x",tolua_get_Demi__DiQuat_x,tolua_set_Demi__DiQuat_x);
    tolua_variable(tolua_S,"y",tolua_get_Demi__DiQuat_y,tolua_set_Demi__DiQuat_y);
    tolua_variable(tolua_S,"z",tolua_get_Demi__DiQuat_z,tolua_set_Demi__DiQuat_z);
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

