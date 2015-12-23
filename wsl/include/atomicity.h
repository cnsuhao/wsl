
#ifndef __WSL_ATOMICITY_H__
#define __WSL_ATOMICITY_H__

#include <stdint.h>
#include "macros.h"
#include "meta.h"


#if defined(__SUNPRO_CC) && defined(__sparc)
#include <sys/atomic.h>
#endif

#if !defined(_WIN32) || defined (__CYGWIN__)
#include <sched.h>
#endif

#if defined(_MSC_VER)
#include <Windows.h>
#include <intrin.h>
#undef max
#undef min
#endif

namespace wbl
{
namespace atomicity
{

#if defined(__ICC)
template<typename must_be_int = int>
int32_t faa32(int32_t* x, int32_t inc)
{
	asm volatile("lock xadd %0,%1"
			: "=r" (inc), "=m" (*x)
			: "0" (inc)
			: "memory");
	return inc;
}
#if defined(__x86_64)
template<typename must_be_int = int>
int64_t faa64(int64_t* x, int64_t inc)
{
	asm volatile("lock xadd %0,%1"
			: "=r" (inc), "=m" (*x)
			: "0" (inc)
			: "memory");
	return inc;
}
#endif
#endif

#if defined(__GNUC__)
namespace
{
#if defined(__i386)
	inline int32_t cmpxchg32_i386(volatile void *ptr, int32_t oldv, int32_t newv)
	{
		asm volatile("lock\n cmpxchgl %1,%2" : "=a" (oldv) : "q" (newv), "m" (*(int32_t *)ptr),"0" (oldv)
				: "memory");
		return oldv;
	}
	inline bool cas32_i386(volatile void *ptr, int32_t oldv, int32_t newv)
   	{
		return cmpxchg32_i386((ptr), (oldv), (newv)) == (oldv);
	}
	inline bool cas64_i386(volatile int64_t *addr,
			int64_t oldv, int64_t newv)
	{
		uint32_t *po = (uint32_t *) &oldv; 
		uint32_t *pn = (uint32_t *) &newv; 
		bool result;
		__asm__ __volatile__ ("" ::: "memory"); // Optimization barrier
#if __PIC__
		__asm__ __volatile__("pushl %%ebx;"           // Save %ebx on the stack
				"movl %5, %%ebx;"        // Move the proper value into %ebx
				"lock; cmpxchg8b (%6);"  // Perform the exchange
				"setz %7; "
				"popl %%ebx;"            // Restore %ebx
				: "=d" (po[1]), "=a" (po[0])
				: "0" (po[1]),  "1" (po[0]),
				"c" (pn[1]),  "r" (pn[0]),
				"r" (addr), "m" (result)
				:  "cc", "memory");
#else
		__asm__ __volatile__("lock\n cmpxchg8b %0; setz %1"
				: "=m"(*addr), "=q"(result)
				: "m"(*addr), "d" (po[1]), "a" (po[0]),
				"c" (pn[1]), "b" (pn[0]) : "memory");
#endif
		return result;
	}
#elif defined(__x86_64)
	struct double_val_type
	{
		int64_t v1;
		int64_t v2;
	};

	inline bool cas128_x64(volatile void * vaddr, const void * poldv, const void * pnewv)
	{
		bool result;
		volatile double_val_type * addr = (volatile double_val_type *) vaddr;
		const uint64_t *po = (const uint64_t *) poldv; 
		const uint64_t *pn = (const uint64_t *) pnewv; 
		__asm__ __volatile__ ("" ::: "memory"); // Optimization barrier
		__asm__ __volatile__("lock\n cmpxchg16b %0; setz %1"
				: "=m"(*addr), "=q"(result)
				: "m"(*addr), "d" (po[1]), "a" (po[0]),
				"c" (pn[1]), "b" (pn[0]) : "memory");
		return result;
	}
#endif
}
#endif

// atomic functions only work on integers

/** @brief Add a value to a variable, atomically.
 *
 *  Implementation is heavily platform-dependent.
 *  @param ptr Pointer to a 32-bit signed integer.
 *  @param addend Value to add.
 */
inline int32_t fetch_and_add_32(volatile int32_t* ptr, int32_t addend)
{
#if defined(__ICC)	//x86 version
	return _InterlockedExchangeAdd((void*)ptr, addend);
#elif defined(__ECC)	//IA-64 version
	return _InterlockedExchangeAdd((void*)ptr, addend);
#elif defined(__ICL) || defined(_MSC_VER)
	return _InterlockedExchangeAdd(reinterpret_cast<volatile long*>(ptr),
			addend);
#elif defined(__GNUC__) && defined(__x86_64)
	return __sync_fetch_and_add(ptr, addend);
#elif defined(__GNUC__) && defined(__i386) &&			\
	(defined(__i686) || defined(__pentium4) || defined(__athlon))
	return __sync_fetch_and_add(ptr, addend);
#elif defined(__GNUC__) && defined(__i386)
	volatile int32_t before, after;
	do
	{
		before = *ptr;
		after = before + addend;
	} while (cmpxchg32_i386(ptr, before,
				after) != before);
	return before;
#elif defined(__SUNPRO_CC) && defined(__sparc)
	volatile int32_t before, after;
	do
	{
		before = *ptr;
		after = before + addend;
	} while (atomic_cas_32((volatile unsigned int*)ptr, before,
				after) != before);
	return before;
#else	//fallback, slow
#pragma message("slow fetch_and_add_32")
	int32_t res;
#pragma omp critical
	{
		res = *ptr;
		*(ptr) += addend;
	}
	return res;
#endif
}

/** @brief Add a value to a variable, atomically.
 *
 *  Implementation is heavily platform-dependent.
 *  @param ptr Pointer to a 64-bit signed integer.
 *  @param addend Value to add.
 */
inline int64_t fetch_and_add_64(volatile int64_t* ptr, int64_t addend)
{
#if defined(__ICC) && defined(__x86_64)	//x86 version
	return faa64<int>((int64_t*)ptr, addend);
#elif defined(__ECC)	//IA-64 version
	return _InterlockedExchangeAdd64((void*)ptr, addend);
#elif defined(__ICL) || defined(_MSC_VER)
#ifndef _WIN64
	_GLIBCXX_PARALLEL_ASSERT(false);	//not available in this case
	return 0;
#else
	return _InterlockedExchangeAdd64(ptr, addend);
#endif
#elif defined(__GNUC__) && defined(__x86_64)
	return __sync_fetch_and_add(ptr, addend);
#elif defined(__GNUC__) && defined(__i386) &&			\
	(defined(__i686) || defined(__pentium4) || defined(__athlon))
	return __sync_fetch_and_add(ptr, addend);
#elif defined(__GNUC__) && defined(__i386)
	volatile int64_t before, after;
	do
	{
		before = *ptr;
		after = before + addend;
	} while (!cas64_i386(ptr, before, after));
	return before;
#elif defined(__SUNPRO_CC) && defined(__sparc)
	volatile int64_t before, after;
	do
	{
		before = *ptr;
		after = before + addend;
	} while (atomic_cas_64((volatile unsigned long long*)ptr, before,
				after) != before);
	return before;
#else	//fallback, slow
#if defined(__GNUC__) && defined(__i386)
	// XXX doesn't work with -march=native
	//#warning "please compile with -march=i686 or better"
#endif
#pragma message("slow fetch_and_add_64")
	int64_t res;
#pragma omp critical
	{
		res = *ptr;
		*(ptr) += addend;
	}
	return res;
#endif
}

namespace
{
	template <typename T>
	inline T fetch_and_add_impl(volatile T* ptr, T addend, int2type<4> dummy)
	{
		return (T) fetch_and_add_32((volatile int32_t*) ptr, (int32_t)addend);
	}

	template <typename T>
	inline T fetch_and_add_impl(volatile T* ptr, T addend, int2type<8> dummy)
	{
		return (T)fetch_and_add_64((volatile int64_t*) ptr, (int64_t)addend);
	}
}

/** @brief Add a value to a variable, atomically.
 *
 *  Implementation is heavily platform-dependent.
 *  @param ptr Pointer to a signed integer.
 *  @param addend Value to add.
 */
template<typename T>
inline T fetch_and_add(volatile T* ptr, T addend)
{
	return fetch_and_add_impl(ptr, addend, int2type<sizeof(T)>());
	/*
	if (sizeof(T) == sizeof(int32_t))
		return (T)fetch_and_add_32((volatile int32_t*) ptr, (int32_t)addend);
	else if (sizeof(T) == sizeof(int64_t))
		return (T)fetch_and_add_64((volatile int64_t*) ptr, (int64_t)addend);
	else
		WBL_ASSERT(false);
		*/
}


#if defined(__ICC)

template<typename must_be_int = int>
inline int32_t cas32(volatile int32_t* ptr, int32_t old, int32_t nw)
{
	int32_t before;
	__asm__ __volatile__("lock; cmpxchgl %1,%2"
			: "=a"(before)
			: "q"(nw), "m"(*(volatile long long*)(ptr)), "0"(old)
			: "memory");
	return before;
}

#if defined(__x86_64)
template<typename must_be_int = int>
inline int64_t cas64(volatile int64_t *ptr, int64_t old, int64_t nw)
{
	int64_t before;
	__asm__ __volatile__("lock; cmpxchgq %1,%2"
			: "=a"(before)
			: "q"(nw), "m"(*(volatile long long*)(ptr)), "0"(old)
			: "memory");
	return before;
}
#endif

#endif

/** @brief Compare @c *ptr and @c comparand. If equal, let @c
 * *ptr=replacement and return @c true, return @c false otherwise.
 *
 *  Implementation is heavily platform-dependent.
 *  @param ptr Pointer to 32-bit signed integer.
 *  @param comparand Compare value.
 *  @param replacement Replacement value.
 */
inline bool compare_and_swap_32(volatile int32_t* ptr, int32_t comparand, int32_t replacement)
{
#if defined(__ICC)	//x86 version
	return _InterlockedCompareExchange((void*)ptr, replacement,
			comparand) == comparand;
#elif defined(__ECC)	//IA-64 version
	return _InterlockedCompareExchange((void*)ptr, replacement,
			comparand) == comparand;
#elif defined(__ICL) || defined(_MSC_VER)
	return _InterlockedCompareExchange(reinterpret_cast<volatile long*>(ptr),
			replacement, comparand) == comparand;
#elif defined(__GNUC__) && defined(__x86_64)
	return __sync_bool_compare_and_swap(ptr, comparand, replacement);
#elif defined(__GNUC__) && defined(__i386) &&			\
	(defined(__i686) || defined(__pentium4) || defined(__athlon))
	return __sync_bool_compare_and_swap(ptr, comparand, replacement);
#elif defined(__GNUC__) && defined(__i386)
	return cas32_i386(ptr, comparand, replacement);
#elif defined(__SUNPRO_CC) && defined(__sparc)
	return atomic_cas_32((volatile unsigned int*)ptr, comparand,
			replacement) == comparand;
#else
#pragma message("slow compare_and_swap_32")
	bool res = false;
#pragma omp critical
	{
		if (*ptr == comparand)
		{
			*ptr = replacement;
			res = true;
		}
	}
	return res;
#endif
}

/** @brief Compare @c *ptr and @c comparand. If equal, let @c
 * *ptr=replacement and return @c true, return @c false otherwise.
 *
 *  Implementation is heavily platform-dependent.
 *  @param ptr Pointer to 64-bit signed integer.
 *  @param comparand Compare value.
 *  @param replacement Replacement value.
 */
inline bool compare_and_swap_64(volatile int64_t* ptr, int64_t comparand, int64_t replacement)
{
#if defined(__ICC) && defined(__x86_64)	//x86 version
	return cas64<int>(ptr, comparand, replacement) == comparand;
#elif defined(__ECC)	//IA-64 version
	return _InterlockedCompareExchange64((void*)ptr, replacement,
			comparand) == comparand;
#elif defined(__ICL) || defined(_MSC_VER)
#ifndef _WIN64
	_GLIBCXX_PARALLEL_ASSERT(false);	//not available in this case
	return 0;
#else
	return _InterlockedCompareExchange64(ptr, replacement,
			comparand) == comparand;
#endif

#elif defined(__GNUC__) && defined(__x86_64)
	return __sync_bool_compare_and_swap(ptr, comparand, replacement);
#elif defined(__GNUC__) && defined(__i386) &&			\
	(defined(__i686) || defined(__pentium4) || defined(__athlon))
	return __sync_bool_compare_and_swap(ptr, comparand, replacement);
#elif defined(__GNUC__) && defined(__i386)
	return cas64_i386(ptr, comparand, replacement);
#elif defined(__SUNPRO_CC) && defined(__sparc)
	return atomic_cas_64((volatile unsigned long long*)ptr,
			comparand, replacement) == comparand;
#else
#if defined(__GNUC__) && defined(__i386)
	// XXX -march=native
	//#warning "please compile with -march=i686 or better"
#endif
#pragma message("slow compare_and_swap_64")
	bool res = false;
#pragma omp critical
	{
		if (*ptr == comparand)
		{
			*ptr = replacement;
			res = true;
		}
	}
	return res;
#endif
}

namespace
{
	template <typename T>
	inline bool compare_and_swap_impl(volatile T* ptr, T comparand, T replacement, int2type<4> dummy)
	{
		return compare_and_swap_32((volatile int32_t*) ptr,
				*(const int32_t *) ((void *) &comparand),
				*(const int32_t *) ((void *) &replacement));
	}

	template <typename T>
	inline bool compare_and_swap_impl(volatile T* ptr, T comparand, T replacement, int2type<8> dummy)
	{
		return compare_and_swap_64((volatile int64_t*) ptr,
				*(const int64_t *) ((void *) &comparand),
				*(const int64_t *) ((void *) &replacement));
	}
#if defined(__GNUC__) && defined(__x86_64)
	template <typename T>
	inline bool compare_and_swap_impl(volatile T* ptr, T comparand, T replacement, int2type<16> dummy)
	{
		return cas128_x64(ptr, &comparand, &replacement);
	}
#endif
}

/** @brief Compare @c *ptr and @c comparand. If equal, let @c
 * *ptr=replacement and return @c true, return @c false otherwise.
 *
 *  Implementation is heavily platform-dependent.
 *  @param ptr Pointer to signed integer.
 *  @param comparand Compare value.
 *  @param replacement Replacement value. */
template <typename T>
inline bool compare_and_swap(volatile T* ptr, T comparand, T replacement)
{
	return compare_and_swap_impl<T>(ptr, comparand, replacement, int2type<sizeof(T)>());
	/*
	if (sizeof(T) == sizeof(int32_t))
		return compare_and_swap_32((volatile int32_t*) ptr, (int32_t)comparand, (int32_t)replacement);
	else if (sizeof(T) == sizeof(int64_t))
		return compare_and_swap_64((volatile int64_t*) ptr, (int64_t)comparand, (int64_t)replacement);
	else
		WBL_ASSERT(false);
		*/
}

/** @brief Yield the control to another thread, without waiting for
  the end to the time slice. */
inline void yield()
{
#if defined (_WIN32) && !defined (__CYGWIN__)
	Sleep(0);
#else
	sched_yield();
#endif
}

}
}

#endif

