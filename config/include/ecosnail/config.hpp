#pragma once

#ifdef __cplusplus
	#if __cplusplus != 201103L && __cplusplus != 201402L
		#warning ecosnail::config requires C++11 (or later) support
	#endif
#else
	#warning ecosnail::config requires a C++ compiler, supporting C++ or later
#endif

#include <ecosnail/config/readers.hpp>
#include <ecosnail/config/writers.hpp>
#include <ecosnail/config/macros.hpp>
#include <ecosnail/config/fields.hpp>
