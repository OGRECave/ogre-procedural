%module Ogre
%include "std_map.i"
 %include "std_string.i"
 %include "std_vector.i"
 %{ 
 #include "Ogre.h" 
 #include <hash_map>
 %}
 %include "OgrePlatform.h"
 %include "OgrePrerequisites.h"
 %include "OgreVector2.h"
 %include "OgreVector3.h"
 %include "OgreVector4.h"
 %include "OgreQuaternion.h"
 %include "OgreMath.h" 