#ifndef PROCEDURAL_CYLINDER_GENERATOR_INCLUDED
#define PROCEDURAL_CYLINDER_GENERATOR_INCLUDED

#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class CylinderGenerator : public MeshGenerator
{
public:	
	void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax);
};
}

#endif