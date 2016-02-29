
#ifndef __WSL_MACROS_H__
#define __WSL_MACROS_H__

#define chSTR(x)     #x
#define chSTR2(x)    chSTR(x)
#define chMSG(desc)  message(__FILE__ "(" chSTR2(__LINE__) "):" desc)

#ifdef __linux__
#define HAVE_EPOLL 1
#endif

#if (defined(__APPLE__) && defined(MAC_OS_X_VERSION_10_6)) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)
#define HAVE_KQUEUE 1
#endif

#if !defined(HAVE_EPOLL) && !defined(HAVE_KQUEUE)
#define HAVE_SELECT
#endif

#ifdef _DEBUG
# define WSL_DEBUG_ONLY(x)	x
#else
# define WSL_DEBUG_ONLY(x)
#endif

#define WSL_JOIN(x,y)		WSL_DO_JOIN(x,y)
#define WSL_DO_JOIN(x,y)	WSL_DO_JOIN2(x,y)
#define WSL_DO_JOIN2(x,y)	x##y

#define BEGIN_COMMENT	/##*
#define END_COMMENT		*##/
#define LINE_COMMENT	/##/

#define _WSL_ASSERT_FAIL(file, line, expr)	{							\
		char sz[128];													\
		sprintf(sz, "File %hs, line %d : %hs",							\
			file, line, expr);											\
		::MessageBoxA(NULL, sz,											\
			"Asserting Failed", MB_OK | MB_ICONERROR);					\
		DebugBreak();													\
	}

// define WSL_ASSERT
#ifdef ATLASSERT
# define WSL_ASSERT	ATLASSERT
#else
# ifdef ASSERT
#  define WSL_ASSERT	ASSERT
# else
#  ifdef _WIN32
#   ifdef _DEBUG
#    include <windows.h>
#    define WSL_ASSERT(x)	{ if (!(x)) _WSL_ASSERT_FAIL(__FILE__, __LINE__, #x) }
#   else
#    define WSL_ASSERT(x)
#   endif
#  else
#   include <assert.h>
#   define WSL_ASSERT	assert
#  endif
# endif
#endif

// define WSL_VERIFY
#ifdef VERIFY
# define WSL_VERIFY VERIFY
#else
# ifndef NDEBUG
#  define WSL_VERIFY WSL_ASSERT
# else
#  define WSL_VERIFY(x)	x
# endif
#endif

#ifndef HAVE_SOCKADDR_IN_SIN_LEN
#	if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)	// BSD
#		define HAVE_SOCKADDR_IN_SIN_LEN	1
#	endif
#endif

#ifndef HAVE_SOCKADDR_SA_LEN
#	if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__DragonFly__)	// BSD
#		define HAVE_SOCKADDR_SA_LEN	1
#	endif
#endif

#if HAVE_SOCKADDR_IN_SIN_LEN
#	define SET_SOCKADDR_IN_LENGTH(x)	x.sin_len = sizeof(x)
#	define GET_SOCKADDR_IN_LENGTH(x)	x.sin_len
#else
#	define SET_SOCKADDR_IN_LENGTH(x)	
#	define GET_SOCKADDR_IN_LENGTH(x)	sizeof(x)
#endif

#if HAVE_SOCKADDR_SA_LEN
#	define SET_SOCKADDR_LENGTH(x)	x.sa_len = sizeof(x)
#	define GET_SOCKADDR_LENGTH(x)	x.sa_len
#else
#	define SET_SOCKADDR_LENGTH(x)	
#	define GET_SOCKADDR_LENGTH(x)	sizeof(x)
#endif

#ifdef __GNUC__
#	define WSL_ATTRIBUTE_PACKED	 __attribute__((packed))
#else
#	define WSL_ATTRIBUTE_PACKED
#endif

#if _MSC_FULL_VER > 100000000
#  define WSL_MSVC_FULL_VER _MSC_FULL_VER
#else
#  define WSL_MSVC_FULL_VER (_MSC_FULL_VER * 10)
#endif

#if defined(_MSC_VER) && defined(WSL_MSVC_FULL_VER) && (WSL_MSVC_FULL_VER >=140050215)
#   define WSL_IS_POD(T) (__is_pod(T) && __has_trivial_constructor(T))
#elif defined(__GNUC__) && ((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 3) && !defined(__GCCXML__)))
#   define WSL_IS_POD(T) __is_pod(T)
#elif defined(__ghs__) && (__GHS_VERSION_NUMBER >= 600)
#   define WSL_IS_POD(T) __is_pod(T)
#elif defined(__CODEGEARC__)
#   define WSL_IS_POD(T) __is_pod(T)
#elif defined(__DMC__) && (__DMC__ >= 0x848)
#   define WSL_IS_POD(T) (__typeinfo(T) & 0x800)
#endif

#endif

