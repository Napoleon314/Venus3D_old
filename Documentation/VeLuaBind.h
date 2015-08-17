////////////////////////////////////////////////////////////////////////////
//
//  Venus Engine Header File.
//  Copyright (C), Venus Interactive Entertainment.2012
// -------------------------------------------------------------------------
//  Module:      VePower
//  File name:   VeLuaBind.h
//  Created:     2015/03/17 by Napoleon
//  Description: 
// -------------------------------------------------------------------------
//  History:
//  http://www.venusie.com
////////////////////////////////////////////////////////////////////////////

#pragma once

#define VE_LUA_INTEGER(ty)											\
	template <>														\
	struct default_value<ty>										\
	{																\
		static ty make() noexcept									\
		{															\
			return ty(0);											\
		}															\
	};																\
	template <>														\
	struct can_get<ty> : std::true_type {};							\
	template <>														\
	struct can_push<ty> : std::true_type {};						\
	template <>														\
	struct param_getter<ty>											\
	{																\
		static constexpr VeInt32 stack_count = 1;					\
		static bool test(lua_State *L, VeInt32 idx) noexcept		\
		{															\
			return (VeLua::type(L, idx) == VeLua::VAL_TNUMBER		\
				&& VeLua::isinteger(L, idx));						\
		}															\
		static ty get(lua_State *L, VeInt32 idx) noexcept			\
		{															\
			return (ty)VeLua::tointeger(L, idx);					\
		}															\
	};																\
	template <>														\
	struct param_pusher<ty>											\
	{																\
		static VeInt32 push(lua_State *L, ty val) noexcept			\
		{															\
			VeLua::pushinteger(L, (VeInt64)val);					\
			return 1;												\
		}															\
	}

#define VE_LUA_FLOAT(ty)											\
	template <>														\
	struct default_value<ty>										\
	{																\
		static ty make() noexcept									\
		{															\
			return ty(0);											\
		}															\
	};																\
	template <>														\
	struct can_get<ty> : std::true_type{};							\
	template <>														\
	struct can_push<ty> : std::true_type{};							\
	template <>														\
	struct param_getter<ty>											\
	{																\
		static constexpr VeInt32 stack_count = 1;					\
		static bool test(lua_State *L, VeInt32 idx) noexcept		\
		{															\
			return (VeLua::type(L, idx) == VeLua::VAL_TNUMBER);		\
		}															\
		static ty get(lua_State *L, VeInt32 idx) noexcept			\
		{															\
			return (ty)VeLua::tonumber(L, idx);						\
		}															\
	};																\
	template <>														\
	struct param_pusher<ty>											\
	{																\
		static VeInt32 push(lua_State *L, ty val) noexcept			\
		{															\
			VeLua::pushnumber(L, (VeFloat64)val);					\
			return 1;												\
		}															\
	}

#define VE_LUA_STACK_HOLDER(L) stack_holder _holder(L)
#ifdef VE_DEBUG
#	define VE_LUA_STACK_CHECKER(L) stack_checker _checker(L)
#else
#	define VE_LUA_STACK_CHECKER(L)
#endif

namespace venus
{
	struct stack_holder
	{
		stack_holder(lua_State* L) noexcept
			: m_pkState(L), m_i32Top(VeLua::gettop(L))
		{

		}

		~stack_holder() noexcept
		{
			VeLua::settop(m_pkState, m_i32Top);
		}

	private:
		lua_State* m_pkState;
		const VeInt32 m_i32Top;

	};

	struct stack_checker
	{
		stack_checker(lua_State* L) noexcept
			: m_pkState(L), m_i32Top(VeLua::gettop(L))
		{

		}

		~stack_checker() noexcept
		{
			VE_ASSERT(m_i32Top == VeLua::gettop(m_pkState));
		}

	private:
		lua_State* m_pkState;
		const VeInt32 m_i32Top;

	};

	template <class _Ty>
	struct default_value
	{
		static _Ty make() noexcept
		{
			return _Ty();
		}
	};

	template <>
	struct default_value<void>
	{
		static void make() noexcept
		{

		}
	};

	template <>
	struct default_value<bool>
	{
		static bool make() noexcept
		{
			return false;
		}
	};

	template <>
	struct default_value<const VeChar8*>
	{
		static const VeChar8* make() noexcept
		{
			return "";
		}
	};

	template <>
	struct default_value<VeFixedString>
	{
		static VeFixedString make() noexcept
		{
			return VeFixedString("");
		}
	};

	template <>
	struct default_value<std::tuple<>>
	{
		static std::tuple<> make() noexcept
		{
			return std::tuple<>();
		}
	};

	template <class _This, class... _Rest>
	struct default_value<std::tuple<_This, _Rest...>>
	{
		static std::tuple<_This, _Rest...> make() noexcept
		{
			return std::tuple_cat(std::make_tuple(default_value<_This>::make()),
				default_value<std::tuple<_Rest...>>::make());
		}
	};

	template <class _Ty>
	struct can_get : std::false_type
	{

	};

	template <class _Ty>
	struct can_push : std::false_type
	{

	};

	template <>
	struct can_get<void> : std::true_type
	{

	};

	template <>
	struct can_get<bool> : std::true_type
	{

	};

	template <>
	struct can_push<bool> : std::true_type
	{

	};

	template <>
	struct can_get<const VeChar8*> : std::true_type
	{

	};

	template <>
	struct can_get<VeFixedString> : std::true_type
	{

	};

	template <>
	struct can_push<const VeChar8*> : std::true_type
	{

	};

	template <>
	struct can_push<VeChar8*> : std::true_type
	{

	};

	template <>
	struct can_push<VeFixedString> : std::true_type
	{

	};

	template <>
	struct can_get<std::tuple<>> : std::true_type
	{

	};

	template <>
	struct can_push<std::tuple<>> : std::true_type
	{

	};

	template <class _This, class... _Rest>
	struct can_get<std::tuple<_This, _Rest...>>
		: VeIf<can_get<_This>::value && can_get<std::tuple<_Rest...>>::value,
		std::true_type, std::false_type>
	{

	};

	template <class _This, class... _Rest>
	struct can_push<std::tuple<_This, _Rest...>>
		: VeIf<can_push<_This>::value && can_push<std::tuple<_Rest...>>::value,
		std::true_type, std::false_type>
	{

	};

	template <class _Ty>
	struct param_getter
	{
		static constexpr VeInt32 stack_count = 0;

		static bool test(lua_State *L, VeInt32 idx) noexcept
		{
			static_assert(venus::can_get<_Ty>::value,
				"type is not supported for get in lua");
			return false;
		}

		static _Ty get(lua_State *L, VeInt32 idx) noexcept
		{
			static_assert(venus::can_get<_Ty>::value,
				"type is not supported for get in lua");
			return venus::default_value<_Ty>();
		}

	};

	template <class _Ty>
	struct param_pusher
	{
		static VeInt32 push(lua_State *L, _Ty val) noexcept
		{
			static_assert(venus::can_push<_Ty>::value,
				"type is not supported for push in lua");
			return 0;
		}
	};

	template <>
	struct param_getter<void>
	{
		static constexpr VeInt32 stack_count = 0;

		static bool test(lua_State *L, VeInt32 idx) noexcept
		{
			return true;
		}

		static void get(lua_State *L, VeInt32 idx) noexcept
		{

		}
	};

	template <>
	struct param_getter<bool>
	{
		static constexpr VeInt32 stack_count = 1;

		static bool test(lua_State *L, VeInt32 idx) noexcept
		{
			return (VeLua::type(L, idx) == VeLua::VAL_TBOOLEAN);
		}

		static bool get(lua_State *L, VeInt32 idx) noexcept
		{
			return VeLua::toboolean(L, idx);
		}
	};

	template <>
	struct param_pusher<bool>
	{
		static VeInt32 push(lua_State *L, bool val) noexcept
		{
			VeLua::pushboolean(L, val);
			return 1;
		}
	};

	template <>
	struct param_getter<const VeChar8*>
	{
		static constexpr VeInt32 stack_count = 1;

		static bool test(lua_State *L, VeInt32 idx) noexcept
		{
			return (VeLua::type(L, idx) == VeLua::VAL_TSTRING);
		}

		static const VeChar8* get(lua_State *L, VeInt32 idx) noexcept
		{
			return VeLua::tostring(L, idx);
		}
	};

	template <>
	struct param_getter<VeFixedString>
	{
		static constexpr VeInt32 stack_count = 1;

		static bool test(lua_State *L, VeInt32 idx) noexcept
		{
			return (VeLua::type(L, idx) == VeLua::VAL_TSTRING);
		}

		static VeFixedString get(lua_State *L, VeInt32 idx) noexcept
		{
			return VeLua::tostring(L, idx);
		}
	};

	template <>
	struct param_pusher<const VeChar8*>
	{
		static VeInt32 push(lua_State *L, const VeChar8* val) noexcept
		{
			VeLua::pushstring(L, val);
			return 1;
		}
	};

	template <>
	struct param_pusher<VeChar8*>
	{
		static VeInt32 push(lua_State *L, VeChar8* val) noexcept
		{
			VeLua::pushstring(L, val);
			return 1;
		}
	};

	template <>
	struct param_pusher<VeFixedString>
	{
		static VeInt32 push(lua_State *L, VeFixedString val) noexcept
		{
			VeLua::pushstring(L, val);
			return 1;
		}
	};

	template <>
	struct param_getter<std::tuple<>>
	{
		static constexpr VeInt32 stack_count = 0;

		static bool test(lua_State *L, VeInt32 idx) noexcept
		{
			return true;
		}

		static std::tuple<> get(lua_State *L, VeInt32 idx) noexcept
		{
			return std::tuple<>();
		}
	};

	template <class _This, class... _Rest>
	struct param_getter<std::tuple<_This, _Rest...>>
	{
		static constexpr VeInt32 stack_count = param_getter<_This>::stack_count + param_getter<std::tuple<_Rest...>>::stack_count;

		static bool test(lua_State *L, VeInt32 idx) noexcept
		{
			if (param_getter<_This>::test(L, idx))
			{
				return param_getter<std::tuple<_Rest...>>::test(L, idx + param_getter<_This>::stack_count);
			}
			return false;
		}

		static std::tuple<_This, _Rest...> get(
			lua_State *L, VeInt32 idx) noexcept
		{
			return std::tuple_cat(std::make_tuple(param_getter<_This>::get(L, idx)),
				param_getter<std::tuple<_Rest...>>::get(L, idx + param_getter<_This>::stack_count));
		}
	};

	template <VeSizeT idx, VeSizeT size, class... _Rest>
	struct tuple_pusher
	{
		static VeInt32 push(lua_State *L, std::tuple<_Rest...> tuple) noexcept
		{
			auto val = std::get<idx>(tuple);
			VeInt32 i = param_pusher<decltype(val)>::push(L, val);
			if (i >= 0)
			{
				VeInt32 j = tuple_pusher<idx + 1, size, _Rest...>::push(L, tuple);
				if (j >= 0)
				{
					return i + j;
				}
			}
			return -1;
		}
	};

	template <VeSizeT size, class... _Rest>
	struct tuple_pusher<size, size, _Rest...>
	{
		static VeInt32 push(lua_State *L, std::tuple<_Rest...> tuple) noexcept
		{
			return 0;
		}
	};

	template <class... _Rest>
	struct param_pusher<std::tuple<_Rest...>>
	{
		static VeInt32 push(lua_State *L, std::tuple<_Rest...> val) noexcept
		{

			return tuple_pusher<0, sizeof...(_Rest), _Rest...>::push(L, val);
		}
	};

	VE_LUA_INTEGER(VeInt8);
	VE_LUA_INTEGER(VeInt16);
	VE_LUA_INTEGER(VeInt32);
	VE_LUA_INTEGER(VeInt64);
	VE_LUA_INTEGER(VeUInt8);
	VE_LUA_INTEGER(VeUInt16);
	VE_LUA_INTEGER(VeUInt32);
	VE_LUA_INTEGER(VeUInt64);
	VE_LUA_FLOAT(VeFloat32);
	VE_LUA_FLOAT(VeFloat64);

	template<class... _Types>
	struct params_pusher;

	template<>
	struct params_pusher<>
	{
		static VeInt32 push(lua_State *L) noexcept
		{
			return 0;
		}
	};

	template<class _This, class... _Rest>
	struct params_pusher<_This, _Rest...>
	{
		static VeInt32 push(lua_State *L, _This val, _Rest... pak) noexcept
		{
			VeInt32 i = param_pusher<_This>::push(L, val);
			if (i >= 0)
			{
				VeInt32 j = params_pusher<_Rest...>::push(L, pak...);
				if (j >= 0)
				{
					return i + j;
				}
			}
			return -1;
		}
	};

	template <class _Ret = void, class... _Types>
	_Ret call_function(lua_State* L, const VeChar8* func,
		_Types... pak) noexcept
	{
		VE_LUA_STACK_HOLDER(L);
		if (VeLua::pushvalue(L, func) != 1)
		{
			ve_log.LogFormat(VeLog::TYPE_ERROR, "VeLua",
				"value %s is not existed", func);
			return default_value<_Ret>::make();
		}
		if (VeLua::type(L, -1) != VeLua::VAL_TFUNCTION)
		{
			ve_log.LogFormat(VeLog::TYPE_ERROR, "VeLua",
				"value %s is not a function", func);
			return default_value<_Ret>::make();
		}
		VeInt32 i32Params = params_pusher<_Types...>::push(L, pak...);
		if (i32Params < 0)
		{
			ve_log.LogFormat(VeLog::TYPE_ERROR, "VeLua",
				"call function %s without correct params", func);
			return default_value<_Ret>::make();
		}
		if (VeLua::pcall(L, i32Params, param_getter<_Ret>::stack_count))
		{
			ve_log.LogFormat(VeLog::TYPE_ERROR, "VeLua",
				VeLua::tostring(L, -1));
			return default_value<_Ret>::make();
		}
		if (param_getter<_Ret>::test(L, -param_getter<_Ret>::stack_count))
		{
			return param_getter<_Ret>::get(L, -param_getter<_Ret>::stack_count);
		}
		else
		{
			ve_log.LogFormat(VeLog::TYPE_ERROR, "VeLua",
				"call function %s with wrong return", func);
			return default_value<_Ret>::make();
		}
	}

	template <class _Ret = void, class... _Types>
	_Ret call_function(const VeChar8* func, _Types... pak) noexcept
	{
		return call_function<_Ret, _Types...>(
			ve_lua, func, pak...);
	}

	template <class... _Rets, class... _Types>
	std::tuple<_Rets...> call_func_mret(VeLua& lua, const VeChar8* func,
		_Types... pak) noexcept
	{
		return call_function<std::tuple<_Rets...>, _Types...>(lua, func, pak...);
	}

	template <class... _Rets, class... _Types>
	std::tuple<_Rets...> call_func_mret(const VeChar8* func, _Types... pak) noexcept
	{
		return call_function<std::tuple<_Rets...>, _Types...>(ve_lua, func, pak...);
	}

	class regist;
	typedef VePointer<regist> registration_ptr;

	class VE_POWER_API regist : public VeRefObject
	{
	public:
		regist() noexcept;

		virtual ~regist() noexcept;

	protected:
		virtual void _register(lua_State*) const noexcept = 0;

	private:
		friend class scope;
		registration_ptr m_spNext;
	};

	class VE_POWER_API scope : public VeMemObject
	{
	public:
		scope() noexcept;

		explicit scope(const registration_ptr& reg) noexcept;

		scope(const scope& other) noexcept;

		~scope() noexcept;

		scope& operator=(const scope& other) noexcept;

		scope& operator,(scope s) noexcept;

		void _register(lua_State* L) const noexcept;

	protected:
		registration_ptr m_spChain;

	};

	class VE_POWER_API namespace_ : public scope
	{
	public:
		explicit namespace_(const VeChar8* pcName) noexcept;

		namespace_& operator[](scope s) noexcept;

	};

	class VE_POWER_API module : public VeMemObject
	{
	public:
		module(lua_State *L, const VeChar8* pcName = nullptr) noexcept;

		void operator[](scope s) noexcept;

	private:
		lua_State* m_pkState = nullptr;
		const VeChar8* m_pcName = nullptr;

	};

	template<class... _Types>
	struct param_paker;

	template<>
	struct param_paker<>
	{
		static constexpr bool empty = true;
		typedef param_paker<> base;
	};

	template<class _This, class... _Rest>
	struct param_paker<_This, _Rest...>
	{
		static constexpr bool empty = false;
		typedef param_paker<_Rest...> base;
		typedef _This type;
	};

	template<class _Ret, class... _Types>
	_Ret call_func(std::function<_Ret(_Types...)>& func, _Types... pak) noexcept
	{
		return func(pak...);
	}

	template<VeInt32 bias, class _Ret, class _Func, class _Pak, class... _Types>
	struct caller;

	template<VeInt32 bias, class _Ret, class _Func, class _Pak, class... _Types>
	struct caller_func
	{
		static _Ret call(lua_State *L, _Func& func, _Types... pak) noexcept
		{
			return call_func(func, pak...);
		}
	};

	template<VeInt32 bias, class _Ret, class _Func, class _Pak, class... _Types>
	struct caller_iter
	{
		static _Ret call(lua_State *L, _Func& func, _Types... pak) noexcept
		{
			return caller<bias + param_getter<typename _Pak::type>::stack_count,
				_Ret, _Func, typename _Pak::base, _Types..., typename _Pak::type>::call(
				L, func, pak..., param_getter<typename _Pak::type>::get(L, bias));
		}
	};

	template<VeInt32 bias, class _Ret, class _Func, class _Pak, class... _Types>
	struct caller
		: VeIf<_Pak::empty, caller_func<bias, _Ret, _Func, _Pak, _Types...>
		, caller_iter<bias, _Ret, _Func, _Pak, _Types...>>::type
	{

	};

	template <VeInt32 bias, class... _Types>
	struct params_tester;

	template <VeInt32 bias>
	struct params_tester<bias>
	{
		static constexpr VeInt32 count() noexcept
		{
			return 0;
		}

		static bool test(lua_State *L) noexcept
		{
			return true;
		}
	};

	template <VeInt32 bias, class _This, class... _Rest>
	struct params_tester<bias, _This, _Rest...>
	{
		static constexpr VeInt32 count() noexcept
		{
			return param_getter<_This>::stack_count + params_tester<bias + param_getter<_This>::stack_count, _Rest...>::count();
		}

		static bool test(lua_State *L) noexcept
		{
			if (param_getter<_This>::test(L, bias))
			{
				return params_tester<bias + param_getter<_This>::stack_count, _Rest...>::test(L);
			}
			return false;
		}
	};

	template <VeInt32 bias, class _Ret, class... _Types>
	struct invoker
	{
		static VeInt32 invoke(lua_State *L, std::function<_Ret(_Types...)>& func) noexcept
		{
			if (VeLua::gettop(L) == (params_tester<bias, _Types...>::count() + bias - 1)
				&& params_tester<bias, _Types...>::test(L))
			{
				return param_pusher<_Ret>::push(L, caller<bias, _Ret, std::function<_Ret(_Types...)>, param_paker<_Types...>>::call(L, func));
			}
			return -1;
		}

	};

	class function : public VeRefObject
	{
	public:
		virtual VeInt32 call(lua_State *L) noexcept = 0;

		VePointer<function> next;

	};

	template<class _Ret, class... _Types>
	class function_normal : public function
	{
	public:
		function_normal(std::function<_Ret(_Types...)> f)
			: func(f)
		{

		}

		virtual VeInt32 call(lua_State *L) noexcept
		{
			VeInt32 ret = invoker<1, _Ret, _Types...>::invoke(L, func);
			if (ret < 0 && next)
			{
				return next->call(L);
			}
			return ret;
		}


		std::function<_Ret(_Types...)> func;

	};

	template<class _Ret, class... _Types>
	function* create_function_normal(std::function<_Ret(_Types...)> f) noexcept
	{
		return VE_NEW function_normal<_Ret, _Types...>(f);
	}

	inline VeInt32 destructor_normal(lua_State *L) noexcept
	{
		VE_ASSERT(VeLua::type(L, -1) == VeLua::VAL_TUSERDATA);
		void* data = VeLua::touserdata(L, -1);
		VE_SAFE_DELETE(*(function**)data);
		return 0;
	}

	template <class _Func>
	class func_regist : public regist
	{
	public:
		func_regist(const VeChar8* name, _Func func) noexcept
			: m_pcName(name), m_kFunc(func)
		{

		}

		virtual void _register(lua_State *L) const noexcept
		{
			VeLua::pushstring(L, m_pcName);
			if (VeLua::rawget(L, -3) == VeLua::VAL_TUSERDATA)
			{
				void* data = VeLua::touserdata(L, -1);
				function* pkFunc = create_function_normal(m_kFunc);
				pkFunc->next = *(function**)data;
				*(function**)data = pkFunc;
			}
			else
			{
				VeLua::pop(L, 1);
				*(function**)VeLua::newuserdata(L, sizeof(function*)) = create_function_normal(m_kFunc);
				VeLua::newtable(L);
				VeLua::pushstring(L, "__gc");
				VeLua::pushcfunction(L, destructor_normal);
				VeLua::rawset(L, -3);
				VeLua::setmetatable(L, -2);

				VeLua::pushstring(L, m_pcName);
				VeLua::pushvalue(L, -2);
				VeLua::rawset(L, -5);

				VeLua::pushstring(L, m_pcName);
				VeLua::pushvalue(L, -2);
				VeLua::pushstring(L, m_pcName);
				VeLua::pushcclosure(L, entry_point, 2);
				VeLua::rawset(L, -4);
			}			
			VeLua::pop(L, 1);
		}

		static VeInt32 entry_point(lua_State *L) noexcept
		{
			VE_ASSERT(VeLua::type(L, VeLua::upvalueindex(1)) == VeLua::VAL_TUSERDATA);
			VeInt32 ret = (*(function**)VeLua::touserdata(L, VeLua::upvalueindex(1)))->call(L);
			if (ret >= 0) return ret;
			ve_log.LogFormat(VeLog::TYPE_ERROR, "VeLua",
				"call function %s without correct overload",
				VeLua::tostring(L, VeLua::upvalueindex(2)));
			return 0;
		}

		const VeChar8* m_pcName;
		_Func m_kFunc;

	};

	template <class _Ret, class... _Types>
	scope def(const VeChar8* name, _Ret (*func)(_Types...)) noexcept
	{
		return scope(VE_NEW func_regist<std::function<_Ret(_Types...)>>(name, func));
	}

	template <class _Func>
	scope def(const VeChar8* name, _Func func) noexcept
	{
		return scope(VE_NEW func_regist<_Func>(name, func));
	}

}
