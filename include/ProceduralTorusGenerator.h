#ifndef PROCEDURAL_TORUS_GENERATOR_INCLUDED
#define PROCEDURAL_TORUS_GENERATOR_INCLUDED
#include "Ogre.h"
#include "ProceduralMeshGenerator.h"

namespace Procedural
{

class TorusGenerator : public MeshGenerator
{
public:
	void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax);
};
}
#endif