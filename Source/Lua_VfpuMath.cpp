// PSP Lua VfpuMath. ( FileName: Lua_VfpuMath..cpp )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2014 M4MSOFT
// ------------------------------------------------------------------------

// Include C/C++ header files
#include <pspmath.h>
#include <math.h>

// Include the Lua header files for C++.
#include "luaplayer.hpp"
#include "Lua_VfpuMath.hpp"

// Include basic vfpu math functions
#include "vfpu.h"

#undef PI
#define PI	(3.1415926535897932384626433832795f)
#define RADIANS_PER_DEGREE	(PI/180.0f)

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes
static int lua_VfpuMath_abs (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_acos (lua_State *L);
static int lua_VfpuMath_asin (lua_State *L);
static int lua_VfpuMath_atan2 (lua_State *L);
static int lua_VfpuMath_atan (lua_State *L);
static int lua_VfpuMath_ceil (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_cosh (lua_State *L);
static int lua_VfpuMath_cos (lua_State *L);
static int lua_VfpuMath_deg (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_exp (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_floor (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_fmod (lua_State *L);
static int lua_VfpuMath_frexp (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_ldexp (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_log (lua_State *L);
#if defined(LUA_COMPAT_LOG10)
static int lua_VfpuMath_log10 (lua_State *L);
#endif
static int lua_VfpuMath_min (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_max (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_modf (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_pow (lua_State *L);
static int lua_VfpuMath_rad (lua_State *L);
static int lua_VfpuMath_random (lua_State *L);
static int lua_VfpuMath_randomseed (lua_State *L);
static int lua_VfpuMath_sinh (lua_State *L);
static int lua_VfpuMath_sin (lua_State *L);
static int lua_VfpuMath_sqrt (lua_State *L);	// Same as lua math function.
static int lua_VfpuMath_tanh (lua_State *L);
static int lua_VfpuMath_tan (lua_State *L);

static int lua_VfpuMath_add (lua_State *L);
static int lua_VfpuMath_sub (lua_State *L);
static int lua_VfpuMath_mul (lua_State *L);
static int lua_VfpuMath_div (lua_State *L);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_abs (lua_State *L)
{
  lua_pushnumber(L, fabsf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_acos (lua_State *L)
{
  lua_pushnumber(L, vfpu_acosf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_asin (lua_State *L)
{
  lua_pushnumber(L, vfpu_asinf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_atan2 (lua_State *L)
{
  lua_pushnumber(L, vfpu_atan2f(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_atan (lua_State *L)
{
  lua_pushnumber(L, vfpu_atanf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_ceil (lua_State *L)
{
  lua_pushnumber(L, ceilf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_cosh (lua_State *L)
{
  lua_pushnumber(L, vfpu_coshf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_cos (lua_State *L)
{
  lua_pushnumber(L, vfpu_cosf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_deg (lua_State *L)
{
  lua_pushnumber(L, fdiv(luaL_checknumber(L, 1), RADIANS_PER_DEGREE));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_exp (lua_State *L)
{
  lua_pushnumber(L, vfpu_expf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_floor (lua_State *L)
{
  lua_pushnumber(L, floorf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_fmod (lua_State *L)
{
  lua_pushnumber(L, vfpu_fmodf(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_frexp (lua_State *L)
{
  int e;
  lua_pushnumber(L, frexpf(luaL_checknumber(L, 1), &e));
  lua_pushinteger(L, e);
  return 2;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_ldexp (lua_State *L)
{
  lua_pushnumber(L, ldexpf(luaL_checknumber(L, 1),
							luaL_checkint(L, 2)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_log (lua_State *L)
{
  lua_Number x = luaL_checknumber(L, 1);
  if (x <= 0.0f) luaL_error(L, "Math error: math.log(x [,base]) x Number can't be zero or smaller");
  lua_Number res;
  if (lua_isnoneornil(L, 2))
    res = vfpu_logf(x);
  else {
    lua_Number base = luaL_checknumber(L, 2);
    if (base == 10.0f) res = fdiv(vfpu_logf(x), vfpu_logf(10.0));
    else if (base > 0.0f && base != 1.0f) res = fdiv(vfpu_logf(x), vfpu_logf(base));
	else return luaL_error(L, "Math error: math.log(x [,base]) base Number can't be zero or smaller or be one");
  }
  lua_pushnumber(L, res);
  return 1;
}
// -----------------------------------------------------------------------------------------------------
#if defined(LUA_COMPAT_LOG10)
static int lua_VfpuMath_log10 (lua_State *L)
{
  lua_Number x = luaL_checknumber(L, 1);
  if (x <= 0.0f) return luaL_error(L, "Math error: math.log10(x) Number can't be zero or smaller");
	lua_pushnumber(L, fdiv(vfpu_logf(x), vfpu_logf(10.0f)));
  return 1;
}
#endif
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_min (lua_State *L)
{
  int n = lua_gettop(L);  /* number of arguments */
  lua_Number dmin = luaL_checknumber(L, 1);
  int i;
  for (i=2; i<=n; i++) {
    lua_Number d = luaL_checknumber(L, i);
    if (d < dmin)
      dmin = d;
  }
  lua_pushnumber(L, dmin);
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_max (lua_State *L)
{
  int n = lua_gettop(L);  /* number of arguments */
  lua_Number dmax = luaL_checknumber(L, 1);
  int i;
  for (i=2; i<=n; i++) {
    lua_Number d = luaL_checknumber(L, i);
    if (d > dmax)
      dmax = d;
  }
  lua_pushnumber(L, dmax);
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_modf (lua_State *L)
{
  lua_Number ip;
  lua_Number fp = modff(luaL_checknumber(L, 1), &ip);
  lua_pushnumber(L, ip);
  lua_pushnumber(L, fp);
  return 2;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_pow (lua_State *L)
{
  lua_pushnumber(L, vfpu_powf(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_rad (lua_State *L)
{
  lua_pushnumber(L, fmul(luaL_checknumber(L, 1), RADIANS_PER_DEGREE));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_random (lua_State *L)
{
  switch (lua_gettop(L)) {
    case 0: {
      lua_pushnumber(L, vfpu_randf(0.0f, 1.0f));  /* Number between 0 and 1 */
      break;
    }
    case 1: {
      lua_pushnumber(L, vfpu_randf(0, luaL_checkint(L, 1)));  /* int between 0 and `u' */
      break;
    }
    case 2: {
      lua_pushnumber(L, vfpu_randf(luaL_checkint(L, 1), luaL_checkint(L, 2)));  /* int between `l' and `u' */
      break;
    }
  }
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_randomseed (lua_State *L)
{
  vfpu_srand(luaL_checkint(L, 1));
  return 0;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_sinh (lua_State *L)
{
  lua_pushnumber(L, vfpu_sinhf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_sin (lua_State * L)
{
  lua_pushnumber(L, vfpu_sinf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_sqrt (lua_State *L)
{
  lua_pushnumber(L, sqrtf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_tanh (lua_State *L)
{
  lua_pushnumber(L, vfpu_tanhf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_tan (lua_State *L)
{
  lua_pushnumber(L, vfpu_tanf(luaL_checknumber(L, 1)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_add (lua_State *L)
{
  lua_pushnumber(L, fadd(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_sub (lua_State * L)
{
  lua_pushnumber(L, fsub(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_mul (lua_State *L)
{
  lua_pushnumber(L, fmul(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
  return 1;
}
// -----------------------------------------------------------------------------------------------------
static int lua_VfpuMath_div (lua_State *L)
{
  lua_Number b = luaL_checknumber(L, 2);
  if (b == 0.0f)
	lua_pushnumber(L, 0.0f);
  else
    lua_pushnumber(L, fdiv(luaL_checknumber(L, 1), b));
  return 1;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Define the module table.
static const luaL_Reg VfpuMath_lib[] =
{
  {"abs", lua_VfpuMath_abs},
  {"acos", lua_VfpuMath_acos},
  {"asin", lua_VfpuMath_asin},
  {"atan2", lua_VfpuMath_atan2},
  {"atan", lua_VfpuMath_atan},
  {"ceil", lua_VfpuMath_ceil},
  {"cosh", lua_VfpuMath_cosh},
  {"cos", lua_VfpuMath_cos},
  {"deg", lua_VfpuMath_deg},
  {"exp", lua_VfpuMath_exp},
  {"floor", lua_VfpuMath_floor},
  {"fmod", lua_VfpuMath_fmod},
  {"frexp", lua_VfpuMath_frexp},
  {"ldexp", lua_VfpuMath_ldexp},
  {"log", lua_VfpuMath_log},
#if defined(LUA_COMPAT_LOG10)
  {"log10", lua_VfpuMath_log10},
#endif
  {"max", lua_VfpuMath_max},
  {"min", lua_VfpuMath_min},
  {"modf", lua_VfpuMath_modf},
  {"pow", lua_VfpuMath_pow},
  {"rad", lua_VfpuMath_rad},
  {"random", lua_VfpuMath_random},
  {"randomseed", lua_VfpuMath_randomseed},
  {"sinh", lua_VfpuMath_sinh},
  {"sin", lua_VfpuMath_sin},
  {"sqrt", lua_VfpuMath_sqrt},
  {"tanh", lua_VfpuMath_tanh},
  {"tan", lua_VfpuMath_tan},

  {"add", lua_VfpuMath_add},
  {"sub", lua_VfpuMath_sub},
  {"mul", lua_VfpuMath_mul},
  {"div", lua_VfpuMath_div},

  {NULL, NULL}
};
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Open the PSP Lua VfpuMath library.
LUAMOD_API int luaopen_VfpuMath(lua_State * L)
{
	// Create the module.
	luaL_newlib(L, VfpuMath_lib);

	// Set constand values.
	SetFieldValue("pi", PI)
	SetFieldValue("huge", HUGE_VAL)
	SetFieldValue("vfpu", 1)

	return 1; // Number of results.
}
// -----------------------------------------------------------------------------------------------------
