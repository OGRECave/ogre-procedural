#ifndef PROCEDURAL_MESH_GENERATOR_INCLUDED
#define PROCEDURAL_MESH_GENERATOR_INCLUDED
#include "Ogre.h"
namespace Procedural
{
class MeshGenerator
{
public:
	Ogre::MeshPtr realizeMesh();
	virtual void addToManualObject(Ogre::ManualObject* manual, int& offset, float& boundingRadius, Ogre::Vector3& AABBmin, Ogre::Vector3& AABBmax);
};

}
#endif