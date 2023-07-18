#ifndef __STDDEF_H_
#define __STDDEF_H_

#define NULL 0

typedef signed char	   int8_t;
typedef unsigned char	   uint8_t;
typedef signed short int   int16_t;
typedef unsigned short int uint16_t;
typedef signed int	   int32_t;
typedef unsigned int	   uint32_t;
typedef int		   intptr_t;
#if __WORDSIZE == 64
typedef signed long int	  int64_t;
typedef unsigned long int uint64_t;
typedef int64_t		  ptrdiff_t;
#else
__extension__ typedef signed long long int   int64_t;
__extension__ typedef unsigned long long int uint64_t;
typedef int32_t				     ptrdiff_t;
#endif
typedef int64_t	 intmax_t;
typedef uint64_t uintmax_t;

typedef int64_t	 ssize_t;
typedef uint64_t size_t;

#endif
