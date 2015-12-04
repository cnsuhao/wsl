#ifndef _WSL_FUNCTOR_H_
#define _WSL_FUNCTOR_H_
//////////////////////////////////////////////////////////////////////////
//		
//			一些常用函数 仿函数等
//
//////////////////////////////////////////////////////////////////////////

#include <functional>
#include <utility>
#include <string>
#include "macros.h"


namespace wsl
{
	
//////////////////////////////////////////////////////////////////////////
// memory utility

/** delete operation
 */
struct checked_delete
{
	template < typename T >
	void operator()(T *p) const
	{
		try{
			delete p;
		}catch (...) {
		}
	}
};

/** delete array operation
 */
struct checked_array_delete
{
	template < typename T >
	void operator()(T *p) const
	{
		try{
			delete[] p;
		}catch (...) {
		}
	}
};

//////////////////////////////////////////////////////////////////////////
// pointer compare

/** pointer less compare
 */
struct ptr_less
{
	template < typename Ptr >
	bool operator()(const Ptr l, const Ptr r) const
	{
		return *l < *r;
	}
};

/** pointer greater compare
 */
struct ptr_greater
{
	template < typename Ptr >
	bool operator()(const Ptr l, const Ptr r) const
	{
		return *l > *r;
	}
};

/** pointer equal compare
 */
struct ptr_equal
{
	template < typename Ptr >
	bool operator()(const Ptr l, const Ptr r) const
	{
		return *l == *r;
	}
};

/** pointer less equal compare
 */
struct ptr_less_equal
{
	template < typename Ptr >
	bool operator()(const Ptr l, const Ptr r) const
	{
		return *l <= *r;
	}
};

/** pointer greater equal compare
 */
struct ptr_greater_equal
{
	template < typename Ptr >
	bool operator()(const Ptr l, const Ptr r) const
	{
		return *l >= *r;
	}
};

//////////////////////////////////////////////////////////////////////////
// pointer & reference

/** get reference from pointer
 */
struct ptr2ref
{
	template < typename T >
	T& operator()(T *p) const
	{
		WSL_ASSERT(p != 0);
		return *p;
	}

	template < typename SmartPtrT >
	typename SmartPtrT::element_type& operator()(SmartPtrT& p) const
	{
		return *p;
	}

	template < typename SmartPtrT >
	const typename SmartPtrT::element_type& operator()(const SmartPtrT& p) const
	{
		return *p;
	}
};

/** get pointer from reference 
 */
struct ref2ptr
{
	template < typename T >
	T* operator()(T& p) const
	{
		return &p;
	}
};

typedef ref2ptr	addressof;

//////////////////////////////////////////////////////////////////////////
// pair selector

/** select std::pair's first field
 */
struct select1st
{
	template < typename F, typename S >
	F& operator()(std::pair<F, S>& p) const
	{
		return p.first;
	}
	
	template < typename F, typename S >
	const F& operator()(const std::pair<F, S>& p) const
	{
		return p.first;
	}
};

/** select std::pair's second field
 */
struct select2nd
{
	template < typename F, typename S >
	S& operator()(std::pair<F, S>& p) const
	{
		return p.second;
	}

	template < typename F, typename S >
	const S& operator()(const std::pair<F, S>& p) const
	{
		return p.second;
	}
};

/** less compare for std::pair's first field
 */
struct less_1st
{
	template < typename F, typename S1, typename S2 >
	bool operator()(const std::pair<F, S1>& p1, const std::pair<F, S2>& p2) const
	{
		return p1.first < p2.first;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const F& f) const
	{
		return p1.first < f;
	}

	template < typename F, typename S >
	bool operator()(const F& f, const std::pair<F, S>& p2) const
	{
		return f < p2.first;
	}
};

/** less compare for std::pair's second field
 */
struct less_2nd
{
	template < typename F1, typename F2, typename S >
	bool operator()(const std::pair<F1, S>& p1, const std::pair<F2, S>& p2) const
	{
		return p1.second < p2.second;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const S& s) const
	{
		return p1.second < s;
	}

	template < typename F, typename S >
	bool operator()(const S& s, const std::pair<F, S>& p2) const
	{
		return s < p2.second;
	}
};

/** greater compare for std::pair's first field
 */
struct greater_1st
{
	template < typename F, typename S1, typename S2 >
	bool operator()(const std::pair<F, S1>& p1, const std::pair<F, S2>& p2) const
	{
		return p1.first > p2.first;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const F& f) const
	{
		return p1.first > f;
	}

	template < typename F, typename S >
	bool operator()(const F& f, const std::pair<F, S>& p2) const
	{
		return f > p2.first;
	}
};

/** greater compare for std::pair's second field
 */
struct greater_2nd
{
	template < typename F1, typename F2, typename S >
	bool operator()(const std::pair<F1, S>& p1, const std::pair<F2, S>& p2) const
	{
		return p1.second > p2.second;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const S& s) const
	{
		return p1.second > s;
	}

	template < typename F, typename S >
	bool operator()(const S& s, const std::pair<F, S>& p2) const
	{
		return s > p2.second;
	}
};

/** equal compare for std::pair's first field
 */
struct equal_1st
{
	template < typename F, typename S1, typename S2 >
	bool operator()(const std::pair<F, S1>& p1, const std::pair<F, S2>& p2) const
	{
		return p1.first == p2.first;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const F& f) const
	{
		return p1.first == f;
	}

	template < typename F, typename S >
	bool operator()(const F& f, const std::pair<F, S>& p2) const
	{
		return f == p2.first;
	}
};

/** equal compare for std::pair's second field
 */
struct equal_2nd
{
	template < typename F1, typename F2, typename S >
	bool operator()(const std::pair<F1, S>& p1, const std::pair<F2, S>& p2) const
	{
		return p1.second == p2.second;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const S& s) const
	{
		return p1.second == s;
	}

	template < typename F, typename S >
	bool operator()(const S& s, const std::pair<F, S>& p2) const
	{
		return s == p2.second;
	}
};

/** less equal compare for std::pair's first field
 */
struct less_equal_1st
{
	template < typename F, typename S1, typename S2 >
	bool operator()(const std::pair<F, S1>& p1, const std::pair<F, S2>& p2) const
	{
		return p1.first <= p2.first;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const F& f) const
	{
		return p1.first <= f;
	}

	template < typename F, typename S >
	bool operator()(const F& f, const std::pair<F, S>& p2) const
	{
		return f <= p2.first;
	}
};

/** less equal compare for std::pair's second field
 */
struct less_equal_2nd
{
	template < typename F1, typename F2, typename S >
	bool operator()(const std::pair<F1, S>& p1, const std::pair<F2, S>& p2) const
	{
		return p1.second <= p2.second;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const S& s) const
	{
		return p1.second <= s;
	}

	template < typename F, typename S >
	bool operator()(const S& s, const std::pair<F, S>& p2) const
	{
		return s <= p2.second;
	}
};

/** greater equal compare for std::pair's first field
 */
struct greater_equal_1st
{
	template < typename F, typename S1, typename S2 >
	bool operator()(const std::pair<F, S1>& p1, const std::pair<F, S2>& p2) const
	{
		return p1.first >= p2.first;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const F& f) const
	{
		return p1.first >= f;
	}

	template < typename F, typename S >
	bool operator()(const F& f, const std::pair<F, S>& p2) const
	{
		return f >= p2.first;
	}
};

/** greater equal compare for std::pair's second field
 */
struct greater_equal_2nd
{
	template < typename F1, typename F2, typename S >
	bool operator()(const std::pair<F1, S>& p1, const std::pair<F2, S>& p2) const
	{
		return p1.second >= p2.second;
	}

	template < typename F, typename S >
	bool operator()(const std::pair<F, S>& p1, const S& s) const
	{
		return p1.second >= s;
	}

	template < typename F, typename S >
	bool operator()(const S& s, const std::pair<F, S>& p2) const
	{
		return s >= p2.second;
	}
};

//////////////////////////////////////////////////////////////////////////
// hash
inline size_t hash_value(bool v)			{ return static_cast<std::size_t>(v); }
inline size_t hash_value(char v)			{ return static_cast<std::size_t>(v); } 
inline size_t hash_value(unsigned char v)	{ return static_cast<std::size_t>(v); }
inline size_t hash_value(signed char v)	{ return static_cast<std::size_t>(v); } 
inline size_t hash_value(short v)			{ return static_cast<std::size_t>(v); } 
inline size_t hash_value(unsigned short v)	{ return static_cast<std::size_t>(v); } 
inline size_t hash_value(int v)			{ return static_cast<std::size_t>(v); } 
inline size_t hash_value(unsigned int v)	{ return static_cast<std::size_t>(v); } 
inline size_t hash_value(long v)			{ return static_cast<std::size_t>(v); } 
inline size_t hash_value(unsigned long v)	{ return static_cast<std::size_t>(v); } 
inline size_t hash_value(wchar_t v)		{ return static_cast<std::size_t>(v); } 
#if __GLIBC_HAVE_LONG_LONG || HAVE_LONG_LONG
inline size_t hash_value(long long v)			{ return static_cast<std::size_t>(v); } 
inline size_t hash_value(unsigned long long v)	{ return static_cast<std::size_t>(v); } 
#endif

template <class T>
inline void hash_combine(size_t& seed, T const& v);

template <class It>
inline size_t hash_range(It first, It last)
{
	size_t seed = 0;
	for(; first != last; ++first)
		hash_combine(seed, *first);
	return seed;
}

inline size_t hash_value(const wchar_t * s)
{
	size_t seed = 0;
	for(; *s != L'\0'; ++s)
		hash_combine(seed, *s);
	return seed;
}

inline size_t hash_value(const char * s)
{
	size_t seed = 0;
	for(; *s != '\0'; ++s)
		hash_combine(seed, *s);
	return seed;
}

template < typename F, typename S >
inline size_t hash_value(const std::pair<F, S>& v)
{
	size_t seed = 0;
	hash_combine(seed, v.first);
	hash_combine(seed, v.second);
	return seed;
}

template< class T, unsigned N >
inline size_t hash_value(const T (&array)[N])
{
	return hash_range(array, array + N);
}

template< class T, unsigned N >
inline size_t hash_value(T (&array)[N])
{
	return hash_range(array, array + N);
}

template < typename Ch, typename Tr, typename A >
inline size_t hash_value(const std::basic_string<Ch, Tr, A>& s)
{
	return hash_range(s.begin(), s.end());
}

template <class T>
inline void hash_combine(size_t& seed, T const& v)
{
	seed ^= hash_value(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template < typename T >
struct hash
{
	size_t operator()(const T& v) const
	{
		return hash_value(v);
	}
};

//////////////////////////////////////////////////////////////////////////
// compose
template <class _Operation1, class _Operation2>
class unary_compose : public std::unary_function<typename _Operation2::argument_type,
	typename _Operation1::result_type>
{
protected:
	_Operation1 _M_fn1;
	_Operation2 _M_fn2;
public:
	unary_compose(const _Operation1& __x, const _Operation2& __y)
		: _M_fn1(__x), _M_fn2(__y)
	{}

	typename _Operation1::result_type operator()(const typename _Operation2::argument_type& __x) const 
	{
		return _M_fn1(_M_fn2(__x));
	}
};

template <class _Operation1, class _Operation2>
inline unary_compose<_Operation1,_Operation2> compose1(const _Operation1& __fn1, 
		const _Operation2& __fn2)
{
	return unary_compose<_Operation1,_Operation2>(__fn1, __fn2);
}

template <class _Operation1, class _Operation2, class _Operation3>
class binary_compose : public std::unary_function<typename _Operation2::argument_type,
	typename _Operation1::result_type>
{
protected:
	_Operation1 _M_fn1;
	_Operation2 _M_fn2;
	_Operation3 _M_fn3;
public:
	binary_compose(const _Operation1& __x, const _Operation2& __y, const _Operation3& __z)
		: _M_fn1(__x), _M_fn2(__y), _M_fn3(__z) 
	{}

	typename _Operation1::result_type operator()(const typename _Operation2::argument_type& __x) const
	{
		return _M_fn1(_M_fn2(__x), _M_fn3(__x));
	}
};

template <class _Operation1, class _Operation2, class _Operation3>
inline binary_compose<_Operation1, _Operation2, _Operation3> compose2(const _Operation1& __fn1,
		const _Operation2& __fn2, const _Operation3& __fn3)
{
	return binary_compose<_Operation1,_Operation2,_Operation3>(__fn1, __fn2, __fn3);
}
}

#endif