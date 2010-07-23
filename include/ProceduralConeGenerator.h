#ifndef PROCEDURAL_CONE_GENERATOR_INCLUDED
#define PROCEDURAL_CONE_GENERATOR_INCLUDED

#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class ConeGenerator : public MeshGenerator
{
public:
	void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax);
};
}

#endif