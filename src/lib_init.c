/*
** Library initialization.
** Copyright (C) 2005-2017 Mike Pall. See Copyright Notice in luajit.h
**
** Major parts taken verbatim from the Lua interpreter.
** Copyright (C) 1994-2008 Lua.org, PUC-Rio. See Copyright Notice in lua.h
*/

#define lib_init_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "lj_arch.h"

static const luaL_Reg lj_lib_load[] = {
  { "",			luaopen_base },
  { LUA_LOADLIBNAME,	luaopen_package },
  { LUA_TABLIBNAME,	luaopen_table },
  { LUA_IOLIBNAME,	luaopen_io },
  { LUA_OSLIBNAME,	luaopen_os },
  { LUA_STRLIBNAME,	luaopen_string },
  { LUA_MATHLIBNAME,	luaopen_math },
  { LUA_DBLIBNAME,	luaopen_debug },
  { LUA_BITLIBNAME,	luaopen_bit },
  { LUA_JITLIBNAME,	luaopen_jit },
  { NULL,		NULL }
};

static const luaL_Reg lj_lib_preload[] = {
#if LJ_HASFFI
  { LUA_FFILIBNAME,	luaopen_ffi },
#endif
  { NULL,		NULL }
};

static const char * CN_LIB = "_G['导入'] = require; _G['标准输出'] = print; _G['迭代'] = pairs; _G['数组迭代'] = ipairs; _G['运行文件'] = dofile; _G['加载'] = load; _G['加载文件'] = loadfile; _G['加载文本'] = loadstring; _G['判断'] = rawequal; _G['取哈希表值'] = rawget; _G['置哈希表值'] = rawset; _G['选择'] =  select; _G['取函数环境'] =  getfenv ; _G['置函数环境'] =   setfenv; _G['取元表'] =   getmetatable; _G['置元表'] =   setmetatable; _G['到数字'] =   tonumber; _G['到文本'] =   tostring; _G['取类型'] =   type;  字符串 = {}; 字符串.到大写 = string.upper; 字符串.到小写 = string.lower; 字符串.处理 = string.gsub; 字符串.查找 = string.find; 字符串.反转 = string.reverse; 字符串.格式化 = table.insert; 字符串.char = string.char; 字符串.byte = string.byte; 字符串.取长度 = string.len; 字符串.重复字符 = string.rep;  文件读写 = {};  文件读写.打开 = io.open; 文件读写.打开并读入行 = io.lines; 文件读写.句柄是否有效 = io.type; 文件读写.打开临时文件 =  io.tmpfile ();  函数 文件读写.读写位置(file,...) 	返回 io.input(file):seek(...); 结束  函数 文件读写.置缓冲模式(file,...) 	返回 io.input(file):setvbuf(...); 结束  函数 文件读写.写入(file,...) 	返回 io.input(file):write(...); 结束  函数 文件读写.读取(file,...) 	返回 io.input(file):read(...); 结束  函数 文件读写.存盘(file) 	返回 io.input(file):flush(); 结束  函数 文件读写.关闭(file) 	返回 io.input(file):close(); 结束  函数 文件读写.读入行(file) 	返回 io.input(file):lines(); 结束  哈希表 = {}; 哈希表.插入= table.insert; 哈希表.连接元素= table.concat; 哈希表.连接元素= table.concat; 哈希表.取最大索引= table.maxn; 哈希表.删除元素= table.remove; 哈希表.排序 = table.sort;协程 = {} ;协程.创建= coroutine.create;协程.重启= coroutine.resume;协程.挂起= coroutine.yield;协程.取状态= coroutine.running; 协程.创建挂起= coroutine.wrap";
LUALIB_API void luaL_openlibs(lua_State *L)
{
  const luaL_Reg *lib;
  for (lib = lj_lib_load; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring(L, lib->name);
    lua_call(L, 1, 0);
  }
  luaL_findtable(L, LUA_REGISTRYINDEX, "_PRELOAD",
		 sizeof(lj_lib_preload)/sizeof(lj_lib_preload[0])-1);
  for (lib = lj_lib_preload; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_setfield(L, -2, lib->name);
  }
  lua_pop(L, 1);
  luaL_dostring(L,CN_LIB);
}

