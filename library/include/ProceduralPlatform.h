#ifndef PROCEDURAL_PLATFORM_INCLUDED
#define PROCEDURAL_PLATFORM_INCLUDED

#      if defined( PROCEDURAL_NONCLIENT_BUILD )
#          define _ProceduralExport __declspec( dllexport )
#      else
#           if defined( __MINGW32__ )
#               define _ProceduralExport
#           else
#              define _ProceduralExport __declspec( dllimport )
#           endif
#      endif
#endif