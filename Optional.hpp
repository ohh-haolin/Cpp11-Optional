#include <iostream>
#include <type_traits>
#include <functional>

namespace {
	struct NullptrException
	{	
		const char* msg;

		constexpr NullptrException(const char* _ = nullptr) : msg(_)
		{	}
		
		constexpr const char* what()
		{
			return msg;
		}
	};

	template <typename F, typename... Args>
	struct is_invocable : 
		::std::is_constructible<
			::std::function<void(Args ...)>, 
			::std::reference_wrapper<typename std::remove_reference<F>::type>
		>
	{	};

	template<typename T>
	struct Optional
	{
		T* _ptr;
		using NullptrException = ::NullptrException;

		static constexpr NullptrException nullptrException = NullptrException("Optional: Null-Pointer Excption");

		Optional(bool, T* _ = nullptr) : _ptr(_)
		{	}

		Optional(T* _)	: _ptr(_)
		{
			if(!_ptr) throw nullptrException;
		}

		~Optional()
		{
			if(_ptr) delete _ptr;
		}

		T* operator->()
		{
			if(!_ptr) throw nullptrException;
			return _ptr;
		}

		T& operator*()
		{
			if(!_ptr) throw nullptrException;
			return *_ptr;
		}

		static Optional empty() 
		{
			return Optional(new T{});
		}

		static Optional of(T* x) 
		{
			return Optional(x);
		}

		static Optional of(const T& x)  
		{
			return Optional(new T{x});
		}

		static Optional ofNullable(T* x = nullptr) 
		{
			return Optional(true, x);
		}

		static Optional ofNullable(const T& x)  
		{
			return Optional(new T{x});
		}

		T& get()
		{
			if(!_ptr) throw nullptrException;
			return *_ptr;
		}

		bool isPresent()
		{
			return _ptr ? true : false;
		}

		/*
		*param: 必须传入一个可以使用(T*)的东西
		*/
		template<typename T_>
		void ifPresent(T_ fun)
		{
			static_assert(is_invocable<T_, T*>::value, "Optional::ifPresent only accept the callable whose parameter type is T*");
			if(fun && _ptr) fun(_ptr);
		}


		/*
		*param: 必须传入一个可以使用(T*)的东西，并返回bool
		*/
		template<typename T_>
		Optional filter(T_ fun)
		{
			static_assert(is_invocable<T_, T*>::value, "Optional::filter only accept the invocable whose parameter type is T*");
			static_assert(::std::is_same<decltype(fun(_ptr)), bool>::value, "Optional::filter only accept the callable whose return type is bool");

			if(fun && _ptr && fun(_ptr))
			{
				return *this;
			}else
			{
				return empty();
			}
		}

		/*
		*param: 必须传入一个可以使用(T*)的东西，并拥有返回值
		*/
		template<typename T_>
		auto map(T_ fun) -> Optional<decltype(fun(_ptr))>
		{
			static_assert(is_invocable<T_, T*>::value, "Optional::map only accept the invocable whose parameter type is T*");
			static_assert(!::std::is_void<decltype(fun(_ptr))>::value, "Optional::map only accept the callable whose return type is not void");

			if(fun && _ptr)
			{
				return Optional<decltype(fun(_ptr))>::ofNullable(fun(_ptr));
			}else
			{
				return Optional<decltype(fun(_ptr))>::empty();
			}
		}
		
		Optional&  orElse(Optional& other)
		{
			return _ptr ? *this : other;
		}

		T& orElseGet(Optional& other)
		{
			return _ptr ? *_ptr : other.get();
		}

		template<typename T_>
		T* orElseThrow(T_& t)
		{
			if(_ptr) return _ptr;
			else	 throw t;
		}
	};
}