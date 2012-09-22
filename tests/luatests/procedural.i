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
 %include "ProceduralShapeGeneratorsBase.h"
 %include "ProceduralPathGeneratorsBase.h"
 %template(bs1) Procedural::BaseSpline2< Procedural::CubicHermiteSpline2>;
 %template(bs2) Procedural::BaseSpline2< Procedural::CatmullRomSpline2>;
 %template(bs3) Procedural::BaseSpline2< Procedural::KochanekBartelsSpline2>;
 %template(bs4) Procedural::BaseSpline2< Procedural::RoundedCornerSpline2>;
 %template(bs5) Procedural::BaseSpline2< Procedural::BezierCurve2>;
 %template(bs6) Procedural::BaseSpline3< Procedural::CubicHermiteSpline3>;
 %template(bs7) Procedural::BaseSpline3< Procedural::CatmullRomSpline3>;
 %template(bs8) Procedural::BaseSpline3< Procedural::RoundedCornerSpline3>;
 %template(bs9) Procedural::BaseSpline3< Procedural::BezierCurve3>; 
 %include "ProceduralShapeGenerators.h"
 %include "ProceduralPathGenerators.h" 
 %template (mg1) Procedural::MeshGenerator<Procedural::SphereGenerator>;
 %template (mg2) Procedural::MeshGenerator<Procedural::BoxGenerator>;
 %template (mg3) Procedural::MeshGenerator<Procedural::CapsuleGenerator>;
 %template (mg4) Procedural::MeshGenerator<Procedural::CylinderGenerator>;
 %template (mg5) Procedural::MeshGenerator<Procedural::IcoSphereGenerator>;
 %template (mg6) Procedural::MeshGenerator<Procedural::RoundedBoxGenerator>;
 %template (mg7) Procedural::MeshGenerator<Procedural::TorusGenerator>;
 %template (mg8) Procedural::MeshGenerator<Procedural::TorusKnotGenerator>;
 %template (mg9) Procedural::MeshGenerator<Procedural::TubeGenerator>;
 %template (mg10) Procedural::MeshGenerator<Procedural::PlaneGenerator>;
 %template (mg11) Procedural::MeshGenerator<Procedural::Extruder>;
 %template (mg12) Procedural::MeshGenerator<Procedural::ConeGenerator>;
 %template (mg13) Procedural::MeshGenerator<Procedural::Lathe>;
 %template (mg14) Procedural::MeshGenerator<Procedural::Triangulator>;
 %template (mg15) Procedural::MeshGenerator<Procedural::Boolean>;
 %template (mg16) Procedural::MeshGenerator<Procedural::SpringGenerator>;
 %include "ProceduralSphereGenerator.h"
 %include "ProceduralBoxGenerator.h"
 %include "ProceduralCapsuleGenerator.h"
 %include "ProceduralConeGenerator.h"
 %include "ProceduralCylinderGenerator.h"
 %include "ProceduralIcoSphereGenerator.h"
 %include "ProceduralRoundedBoxGenerator.h"
 %include "ProceduralTorusGenerator.h"
 %include "ProceduralTorusKnotGenerator.h"
 %include "ProceduralTubeGenerator.h"
 %include "ProceduralPlaneGenerator.h"
 %include "ProceduralExtruder.h"
 %include "ProceduralLathe.h"
 %include "ProceduralShape.h"
 %include "ProceduralMultiShape.h"
 %include "ProceduralPath.h"
 %include "ProceduralTriangulator.h"
 %include "ProceduralTrack.h"
 %include "ProceduralBoolean.h"
 %include "ProceduralSpringGenerator.h"
 %include "ProceduralSVG.h"
 %include "ProceduralDebugRendering.h" 