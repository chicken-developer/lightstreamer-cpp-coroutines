//
// Created by quynh on 1/3/2023.
//

#ifndef POCORO_CONFIG_HPP
#define POCORO_CONFIG_HPP

/////////////////////////////////////////////////////////////////////////////
// Compiler Detection

#if defined(_MSC_VER)
# define POCORO_COMPILER_MSVC _MSC_FULL_VER
#else
# define POCORO_COMPILER_MSVC 0
#endif

#if defined(__clang__)
# define POCORO_COMPILER_CLANG (__clang_major__ * 10000 + \
                                 __clang_minor__ * 100 + \
                                 __clang_patchlevel__)
#else
# define POCORO_COMPILER_CLANG 0
#endif

#if defined(__GNUC__)
# define POCORO_COMPILER_GCC (__GNUC__ * 10000 + \
                               __GNUC_MINOR__ * 100 + \
                               __GNUC_PATCHLEVEL__)
#else
# define POCORO_COMPILER_GCC 0
#endif

/// \def POCORO_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER
/// Defined to 1 if the compiler supports returning a coroutine_handle from
/// the await_suspend() method as a way of transferring execution
/// to another coroutine with a guaranteed tail-call.
#if POCORO_COMPILER_CLANG
# if __clang_major__ >= 7
#  define POCORO_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER 1
# endif
#endif
#ifndef POCORO_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER
# define POCORO_COMPILER_SUPPORTS_SYMMETRIC_TRANSFER 0
#endif

#if POCORO_COMPILER_MSVC
# define POCORO_ASSUME(X) __assume(X)
#else
# define POCORO_ASSUME(X)
#endif

#if POCORO_COMPILER_MSVC
# define POCORO_NOINLINE __declspec(noinline)
#elif POCORO_COMPILER_CLANG || POCORO_COMPILER_GCC
# define POCORO_NOINLINE __attribute__((noinline))
#else
# define POCORO_NOINLINE
#endif

#if POCORO_COMPILER_MSVC
# define POCORO_FORCE_INLINE __forceinline
#elif POCORO_COMPILER_CLANG
# define POCORO_FORCE_INLINE __attribute__((always_inline))
#else
# define POCORO_FORCE_INLINE inline
#endif

/////////////////////////////////////////////////////////////////////////////
// OS Detection

/// \def POCORO_OS_WINNT
/// Defined to non-zero if the target platform is a WindowsNT variant.
/// 0x0500 - Windows 2000
/// 0x0501 - Windows XP/Server 2003
/// 0x0502 - Windows XP SP2/Server 2003 SP1
/// 0x0600 - Windows Vista/Server 2008
/// 0x0601 - Windows 7
/// 0x0602 - Windows 8
/// 0x0603 - Windows 8.1
/// 0x0A00 - Windows 10
#if defined(_WIN32_WINNT) || defined(_WIN32)
# if !defined(_WIN32_WINNT)
// Default to targeting Windows 10 if not defined.
#  define _WIN32_WINNT 0x0A00
# endif
# define POCORO_OS_WINNT _WIN32_WINNT
#else
# define POCORO_OS_WINNT 0
#endif

#if defined(__linux__)
# define POCORO_OS_LINUX 1
#else
# define POCORO_OS_LINUX 0
#endif

/////////////////////////////////////////////////////////////////////////////
// CPU Detection

/// \def POCORO_CPU_X86
/// Defined to 1 if target CPU is of x86 family.
#if POCORO_COMPILER_MSVC
# if defined(_M_IX86)
#  define POCORO_CPU_X86 1
# endif
#elif POCORO_COMPILER_GCC || POCORO_COMPILER_CLANG
# if defined(__i386__)
#  define POCORO_CPU_X86 1
# endif
#endif
#if !defined(POCORO_CPU_X86)
# define POCORO_CPU_X86 0
#endif

/// \def POCORO_CPU_X64
/// Defined to 1 if the target CPU is x64 family.
#if POCORO_COMPILER_MSVC
# if defined(_M_X64)
#  define POCORO_CPU_X64 1
# endif
#elif POCORO_COMPILER_GCC || POCORO_COMPILER_CLANG
# if defined(__x86_64__)
#  define POCORO_CPU_X64 1
# endif
#endif
#if !defined(POCORO_CPU_X64)
# define POCORO_CPU_X64 0
#endif

/// \def POCORO_CPU_32BIT
/// Defined if compiling for a 32-bit CPU architecture.
#if POCORO_CPU_X86
# define POCORO_CPU_32BIT 1
#else
# define POCORO_CPU_32BIT 0
#endif

/// \def POCORO_CPU_64BIT
/// Defined if compiling for a 64-bit CPU architecture.
#if POCORO_CPU_X64
# define POCORO_CPU_64BIT 1
#else
# define POCORO_CPU_64BIT 0
#endif

#if POCORO_COMPILER_MSVC
# define POCORO_CPU_CACHE_LINE std::hardware_destructive_interference_size
#else
// On most architectures we can assume a 64-byte cache line.
# define POCORO_CPU_CACHE_LINE 64
#endif

#endif //POCORO_CONFIG_HPP
