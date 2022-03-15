#ifndef ML_DEFS_H
#define ML_DEFS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(_MSC_VER)
#	if defined(ML_SHARED)
#		define ML_API __declspec(dllexport)
#	elif define(ML_STATIC)
#		define ML_API
#	else
#		define ML_API __declspec(dllimport)
#	endif
#else
#	define ML_API
#endif

