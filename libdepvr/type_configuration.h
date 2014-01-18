#pragma once

#ifndef _CRT_SECURE_NO_DEPRECATE
	#define _CRT_SECURE_NO_DEPRECATE
#endif

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#ifndef __BADA__
	#include <crtdbg.h>
	#endif
#endif

#if !defined( _SECURE_SCL )
	#define _SECURE_SCL 0
#endif

#include <limits>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

#if defined ( __BADA__ )
	#include "type_bada.h"
#elif defined ( WIN32 )
	#include "tytpe_win32.h"
#elif defined ( __ANDROID__ )
	#include "type_android.h"
#elif defined ( __iOS__ )
	#include "type_ios.h"
#elif defined ( __MACOSX__ )
	#include "type_macos.h"
#else
	#error "Unknown platform"
#endif

#if !( defined( FCCB ) && defined( FOURCC ) && defined( FRCC64 ) )
#define FCCB(B)								( ( B ) << 8 )
#define FOURCC(B1,B2,B3,B4)					( (uint32_t)B1 | FCCB( B2 | FCCB( B3 | FCCB( B4 ) ) ) )
#define FRCC64(B1,B2,B3,B4,B5,B6,B7,B8)		( ( (uint64_t)FOURCC( (B5), (B6), (B7), (B8) ) << 32 ) | FOURCC( (B1), (B2), (B3), (B4) ) )
#endif
