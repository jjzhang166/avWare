#ifndef _AV_LUA_H_
#define _AV_LUA_H_

#include "Lua/lua.hpp"
#define LuaHomeDir "lua"



template<std::size_t> struct int_{};

template <typename Functor, typename Tuple>
void tuple_visitor_impl(Functor&& functor, const Tuple& t, int_<1>) {
	functor(std::get<std::tuple_size<Tuple>::value - 1>(t));
}

template <typename Functor, typename Tuple, size_t Pos>
void tuple_visitor_impl(Functor&& functor, const Tuple& t, int_<Pos>) {
	functor(std::get<std::tuple_size<Tuple>::value - Pos>(t));
	tuple_visitor_impl(std::forward<Functor&&>(functor), t, int_<Pos - 1>());
}

template <typename Functor, typename... Args>
void tuple_visitor(Functor&& functor, const std::tuple<Args...>& t) {
	tuple_visitor_impl(std::forward<Functor&&>(functor), t, int_<sizeof...(Args)>());
}


class CAvArgsT
{
public:
	CAvArgsT() {};
	virtual ~CAvArgsT() {};

public:
//	template <typename T>
//	void operator()(T&& t) { std::cout << "unexpect type: " << typeid(std::forward<T&&>(t)).name() << std::endl; }
	virtual void operator()(int i) = 0;
	virtual void operator()(double d) = 0;
	virtual void operator()(const std::string& s) = 0;
	virtual void operator()(char *s) = 0;

};

inline void *lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize) {
	(void)ud;  (void)osize;  /* not used */
	if (nsize == 0) {
		free(ptr);
		//av_warning("free %p\n", ptr);
		return NULL;
	}
	else{
		char *RetPtr = NULL;
		RetPtr = (char *)realloc(ptr, nsize);
		//av_warning("reall %p old ptr = %p\n", RetPtr, ptr);
		return RetPtr;
	}
}


class CAvLua:public CAvArgsT
{
public:
	CAvLua(const char *LuaFile){
		m_LuaState = NULL;
		m_LuaState = lua_newstate(lua_alloc, NULL);
		assert(m_LuaState != NULL);
		char LuaFilePath[128];
		sprintf(LuaFilePath, "%s/%s", LuaHomeDir, LuaFile);
		if (luaL_loadfile(m_LuaState, LuaFilePath)){
			assert(0);
		}

	}
	CAvLua(){
		m_LuaState = NULL;
		m_LuaState = lua_newstate(lua_alloc, NULL);
		assert(m_LuaState != NULL);
	}
	~CAvLua(){
		if (NULL != m_LuaState){
			lua_close(m_LuaState);
		}
		m_LuaState = NULL;
	}
		
public:
	template <typename ... ARGS>
	int LuaCall(const char *LuaFucName, ARGS... args)
	{
		lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
		int r = lua_getglobal(m_LuaState, LuaFucName);

		int  sizeargs = sizeof...(args);
		if (sizeargs > 0){
			auto t = std::make_tuple(args...);
			tuple_visitor(*this, t);
		}
		lua_call(m_LuaState, sizeargs, 1);
		
		if (lua_isstring(m_LuaState, -1)){
			char *str = (char *)lua_tostring(m_LuaState, -1);
		}
		else if (lua_isnumber(m_LuaState, -1)){
			int sum = (int)lua_tonumber(m_LuaState, -1);
		}
		else if (lua_isboolean(m_LuaState, -1)){
			bool b = (bool)lua_toboolean(m_LuaState, -1);
		}
		lua_pop(m_LuaState, 1);
		return 0;
	}
	std::string LuaGlobal(const char *LuaGlobal)
	{
		std::string ret;
		lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
		int t = lua_getglobal(m_LuaState, LuaGlobal);

		if (lua_isstring(m_LuaState, -1)){
			ret.assign(std::string(lua_tostring(m_LuaState, -1)));
			lua_pop(m_LuaState, 1);
		}
		else if (lua_isnumber(m_LuaState, -1)){
			char numberstr[12] = { 0 };
			sprintf(numberstr, "%f", lua_tonumber(m_LuaState, -1));
			ret.assign(numberstr);
			lua_pop(m_LuaState, 1);
		}
		else if (lua_isboolean(m_LuaState, -1)){
			lua_toboolean(m_LuaState, -1) == 0 ? ret.assign("false") : ret.assign("true");
			lua_pop(m_LuaState, 1);
		}
		else{
			assert(0);
		}
		
		return ret;
	}
	std::list <std::string> LuaArray(const char *ArrayName){
		std::list<std::string> ret;
		ret.clear();
		//lua_settop(m_LuaState, 0);
		lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
		int iRet = lua_getglobal(m_LuaState, ArrayName);
		if (iRet <= 0){
			return ret;
		}
		int n = luaL_len(m_LuaState, -1);
		std::string mem;
		
		for (int i = 1; i <= n; i++){
			lua_pushnumber(m_LuaState, i);
			lua_gettable(m_LuaState, -2);
			mem.assign(lua_tostring(m_LuaState, -1));
			ret.push_back(mem);
			lua_pop(m_LuaState, 1);
		}
		return ret;
	}
	bool LuaLoadfile(const char *LuaFile){
		char LuaFilePath[128];
		sprintf(LuaFilePath, "%s/%s", LuaHomeDir, LuaFile);
		if (luaL_loadfile(m_LuaState, LuaFilePath)){
			assert(0);
		}
		return av_true;
	}

public:
	void operator()(int i){
		lua_pushnumber(m_LuaState, i);
	}
	void operator()(double d){
		lua_pushnumber(m_LuaState, d);
	}
	void operator()(const std::string& s){
		lua_pushstring(m_LuaState, (char *)s.c_str());
	}
	void operator()(char *s){
		lua_pushstring(m_LuaState, s);
	}
private:
	lua_State *m_LuaState;

};




#endif

