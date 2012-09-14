 %module Procedural
 %include "std_map.i"
 %include "std_string.i" 
 %{ 
 #include "Ogre.h"
 #include "Procedural.h"
 #include "LuaTests.h"
 using namespace Procedural;
 #include <hash_map>
 %}
 %include "OgrePlatform.h"
 %include "OgrePrerequisites.h"
 %include "OgreVector2.h"
 %include "OgreVector3.h"
 %include "OgreVector4.h"
 %include "OgreQuaternion.h"
 %include "ProceduralPlatform.h"
 %include "ProceduralTriangleBuffer.h"
 %include "ProceduralMeshGenerator.h"
 %include "LuaTests.h"
 %template (sg) Procedural::MeshGenerator<Procedural::SphereGenerator>;
 %include "ProceduralSphereGenerator.h"
 