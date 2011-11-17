#ifndef PROCEDURAL_PLATFORM_INCLUDED
#define PROCEDURAL_PLATFORM_INCLUDED

#if defined( __SYMBIAN32__ )
#   define PROCEDURAL_PLATFORM PROCEDURAL_PLATFORM_SYMBIAN
#elif defined( __WIN32__ ) || defined( _WIN32 )
#   define PROCEDURAL_PLATFORM PROCEDURAL_PLATFORM_WIN32
#elif defined( __APPLE_CC__)
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 30000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 30000
#       define PROCEDURAL_PLATFORM PROCEDURAL_PLATFORM_IPHONE
#   else
#       define PROCEDURAL_PLATFORM PROCEDURAL_PLATFORM_APPLE
#   endif
#elif defined(linux) && defined(__arm__)
#   define PROCEDURAL_PLATFORM PROCEDURAL_PLATFORM_TEGRA2
#elif defined(__ANDROID__)
#	define PROCEDURAL_PLATFORM PROCEDURAL_PLATFORM_ANDROID
#else
#   define PROCEDURAL_PLATFORM PROCEDURAL_PLATFORM_LINUX
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS || OGRE_PLATFORM == OGRE_PLATFORM_IPHONE || OGRE_PLATFORM == OGRE_PLATFORM_ANDROID || OGRE_PLATFORM == OGRE_PLATFORM_TEGRA2
#   define _ProceduralExport
#elif PROCEDURAL_PLATFORM == PROCEDURAL_PLATFORM_WIN32 && defined(OgreProcedural_SHARED)
 #      if defined( OgreProcedural_EXPORTS )
 #          define _ProceduralExport __declspec( dllexport )
 #      else
 #           if defined( __MINGW32__ )
 #               define _ProceduralExport
 #           else
 #              define _ProceduralExport __declspec( dllimport )
 #           endif
#       endif
#else
 #   define _ProceduralExport
#endif
#endif
